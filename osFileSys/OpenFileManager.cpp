#include "OpenFileManager.h"

//#include "FileSystem.h"

//InodeTable g_InodeTable;
//extern FileSystem g_FileSystem;
//extern BufferManager g_BufferManager;

OpenFileTable::OpenFileTable() {}

OpenFileTable::~OpenFileTable() {}


InodeTable::InodeTable()
{
	//nothing to do here
}

InodeTable::~InodeTable()
{
	//nothing to do here
}

void InodeTable::Initialize()
{
	/* 获取对g_FileSystem的引用 */

}

Inode* InodeTable::IGet(int inumber)
{
	Inode* pInode;
	//User& u = Kernel::Instance().GetUser();

	while (true)
	{
		/* 检查指定设备dev中编号为inumber的外存Inode是否有内存拷贝 */
		int index = this->IsLoaded(inumber);
		if (index >= 0)	/* 找到内存拷贝 */
		{
			pInode = &(this->m_Inode[index]);
			/* 如果该内存Inode被上锁 */
			if (pInode->i_flag & Inode::ILOCK)
			{
				/* 增设IWANT标志，然后睡眠 */
				pInode->i_flag |= Inode::IWANT;

				//u.u_procp->Sleep((unsigned long)& pInode, ProcessManager::PINOD);

				/* 回到while循环，需要重新搜索，因为该内存Inode可能已经失效 */
				continue;
			}

			/* 如果该内存Inode用于连接子文件系统，查找该Inode对应的Mount装配块 */
			if (pInode->i_flag & Inode::IMOUNT)
			{
				continue;
			}

			/*
			 * 程序执行到这里表示：内存Inode高速缓存中找到相应内存Inode，
			 * 增加其引用计数，增设ILOCK标志并返回之
			 */
			pInode->i_count++;
			pInode->i_flag |= Inode::ILOCK;
			return pInode;
		}
		else	/* 没有Inode的内存拷贝，则分配一个空闲内存Inode */
		{
			pInode = this->GetFreeInode();
			/* 若内存Inode表已满，分配空闲Inode失败 */
			if (NULL == pInode)
			{
				return NULL;
			}
			else	/* 分配空闲Inode成功，将外存Inode读入新分配的内存Inode */
			{
				/* 设置新的设备号、外存Inode编号，增加引用计数，对索引节点上锁 */
				pInode->i_dev = 1;
				pInode->i_number = inumber;
				pInode->i_flag = Inode::ILOCK;
				pInode->i_count++;
				pInode->i_lastr = -1;

				BufferManager* bm = m_FileSystem->bManager;
				/* 将该外存Inode读入缓冲区 */
				Buffer* pBuf = bm->Bread(FileSystem::INODE_ZONE_START_SECTOR + inumber / FileSystem::INODE_NUMBER_PER_SECTOR);

				/* 如果发生I/O错误 */
				if (pBuf->flags & Buffer::B_ERROR)
				{
					/* 释放缓存 */
					bm->Brelse(pBuf);
					/* 释放占据的内存Inode */
					this->IPut(pInode);
					return NULL;
				}

				/* 将缓冲区中的外存Inode信息拷贝到新分配的内存Inode中 */
				pInode->ICopy(pBuf, inumber);
				/* 释放缓存 */
				bm->Brelse(pBuf);
				return pInode;
			}
		}
	}
	return NULL;	/* GCC likes it! */
}
