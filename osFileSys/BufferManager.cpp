#include "BufferManager.h"

extern DeviceDriver g_DeviceDriver;

BufferManager::BufferManager()
{
	dDriver = &g_DeviceDriver;
}
BufferManager::~BufferManager()
{
	dDriver = NULL;
}

void BufferManager::Initialize()
{
	int i;
	Buffer* buf;

	this->bFreeList.av_back = this->bFreeList.av_forw = &(this->bFreeList);
	this->bFreeList.b_back = this->bFreeList.b_forw = &(this->bFreeList);

	for (i = 0; i < NBUF; i++)
	{
		//初始化缓存控制数组
		buf = &(this->mBuf[i]);	
		buf->b_dev = -1;
		buf->b_addr = this->myBuffer[i];

		//初始化NODEV队列（将buf插入双向循环队列头结点的前面）
		buf->b_back = &(this->bFreeList);
		buf->b_forw = this->bFreeList.b_forw;
		this->bFreeList.b_forw->b_back = buf;
		this->bFreeList.b_forw = buf;

		//初始化自由队列
		buf->av_back = &(this->bFreeList);
		buf->av_forw = this->bFreeList.av_forw;
		this->bFreeList.av_forw->av_back = buf;
		this->bFreeList.av_forw = buf;

		buf->flags = 0;
		Brelse(buf);
	}
	return;
}


Buffer* BufferManager::GetBlock(int nBlock)
{
	DevTable* dp;
	Buffer* buf;

	//得到设备的devTable，因为仅有一台设备，所以直接赋值
	dp = dDriver->dTable;

loop:
	//遍历设备的设备队列
	for (buf = dp->b_forw; buf != (Buffer*)dp; buf = buf->b_forw)
	{
		if (buf->b_nblk != nBlock) continue;
		//cli
	
		//sti
		this->NotAvail(buf);
		return buf;
	}

	//不在设备队列中, 查找自由队列
	if (this->bFreeList.av_forw == &(this->bFreeList))
	{
		//自由队列为空
		goto loop;
		//return NULL;
	}

	//取自由队列第一个空闲块
	buf = this->bFreeList.av_forw;
	this->NotAvail(buf);

	//如果该缓存块是延迟写
	if (buf->flags & Buffer::B_DELWRI)
	{
		dDriver->BWrite(buf->b_addr,BUFFER_SIZE,buf->b_nblk*FileSystem::SECTOR_SIZE);
		goto loop;
	}

	buf->flags = Buffer::B_BUSY;

	//从原设备队列中抽出
	buf->b_back->b_forw = buf->b_forw;
	buf->b_forw->b_back = buf->b_back;

	//加入新的设备队列
	buf->b_forw = dp->b_forw;
	buf->b_back = (Buffer*)dp;
	dp->b_forw->b_back = buf;
	dp->b_forw = buf;

	buf->b_dev = 1;//因为dev只有一个，所以没有用到b_dev这个属性，这里随便赋值
	buf->b_nblk = nBlock;
	
	return buf;
}

/* 将buffer插入自由队列末尾 */
void BufferManager::Brelse(Buffer* buf)
{
	//清楚标志位
	buf->flags &= ~(Buffer::B_WANTED | Buffer::B_BUSY | Buffer::B_ASYNC);

	//插入到末尾
	(this->bFreeList.av_back)->av_forw = buf;
	buf->av_back = this->bFreeList.av_back;
	buf->av_forw = &(this->bFreeList);
	this->bFreeList.av_back = buf;

	return;
}

Buffer* BufferManager::Bread(int nBlock)
{
	Buffer* buf;
	buf = this->GetBlock(nBlock);

	//如果在设备队列中找到
	if (buf->flags & Buffer::B_DONE)
	{
		return buf;
	}

	//反之读取
	dDriver->BRead(buf->b_addr, BUFFER_SIZE, nBlock * FileSystem::SECTOR_SIZE);
	buf->flags |= Buffer::B_DONE;

	return buf;
}

void BufferManager::Bwrite(Buffer* buf)
{
	buf->flags &= ~(Buffer::B_READ | Buffer::B_DONE | Buffer::B_ERROR | Buffer::B_DELWRI);
	buf->b_wcount = BUFFER_SIZE;

	dDriver->BWrite(buf->b_addr, buf->b_wcount, buf->b_nblk * FileSystem::SECTOR_SIZE);

	this->Brelse(buf);
	return;
}

/* 从自由队列取出 */
void BufferManager::NotAvail(Buffer* buf)
{
	//cli
	buf->av_back->av_forw = buf->av_forw;
	buf->av_forw->av_back = buf->av_back;
	buf->flags |= Buffer::B_BUSY;
	//sti
	return;
}

Buffer& BufferManager::GetBFreeList()
{
	return this->bFreeList;
}

void BufferManager::ClrBuf(Buffer* buf)
{
	int* pInt = (int*)buf->b_addr;
	for (unsigned int i = 0; i < BUFFER_SIZE / sizeof(int); i++)
	{
		pInt[i] = 0;
	}
	return;
}