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
		//��ʼ�������������
		buf = &(this->mBuf[i]);	
		buf->b_dev = -1;
		buf->b_addr = this->myBuffer[i];

		//��ʼ��NODEV���У���buf����˫��ѭ������ͷ����ǰ�棩
		buf->b_back = &(this->bFreeList);
		buf->b_forw = this->bFreeList.b_forw;
		this->bFreeList.b_forw->b_back = buf;
		this->bFreeList.b_forw = buf;

		//��ʼ�����ɶ���
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

	//�õ��豸��devTable����Ϊ����һ̨�豸������ֱ�Ӹ�ֵ
	dp = dDriver->dTable;

loop:
	//�����豸���豸����
	for (buf = dp->b_forw; buf != (Buffer*)dp; buf = buf->b_forw)
	{
		if (buf->b_nblk != nBlock) continue;
		//cli
	
		//sti
		this->NotAvail(buf);
		return buf;
	}

	//�����豸������, �������ɶ���
	if (this->bFreeList.av_forw == &(this->bFreeList))
	{
		//���ɶ���Ϊ��
		goto loop;
		//return NULL;
	}

	//ȡ���ɶ��е�һ�����п�
	buf = this->bFreeList.av_forw;
	this->NotAvail(buf);

	//����û�������ӳ�д
	if (buf->flags & Buffer::B_DELWRI)
	{
		dDriver->BWrite(buf->b_addr,BUFFER_SIZE,buf->b_nblk*FileSystem::SECTOR_SIZE);
		goto loop;
	}

	buf->flags = Buffer::B_BUSY;

	//��ԭ�豸�����г��
	buf->b_back->b_forw = buf->b_forw;
	buf->b_forw->b_back = buf->b_back;

	//�����µ��豸����
	buf->b_forw = dp->b_forw;
	buf->b_back = (Buffer*)dp;
	dp->b_forw->b_back = buf;
	dp->b_forw = buf;

	buf->b_dev = 1;//��Ϊdevֻ��һ��������û���õ�b_dev������ԣ�������㸳ֵ
	buf->b_nblk = nBlock;
	
	return buf;
}

/* ��buffer�������ɶ���ĩβ */
void BufferManager::Brelse(Buffer* buf)
{
	//�����־λ
	buf->flags &= ~(Buffer::B_WANTED | Buffer::B_BUSY | Buffer::B_ASYNC);

	//���뵽ĩβ
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

	//������豸�������ҵ�
	if (buf->flags & Buffer::B_DONE)
	{
		return buf;
	}

	//��֮��ȡ
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

/* �����ɶ���ȡ�� */
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