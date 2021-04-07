#include "FileSystem.h"

extern DeviceDriver g_DeviceDriver;
extern BufferManager g_BufferManager;
extern SuperBlock g_SuperBlock;
extern InodeTable g_InodeTable;

/*==============================class SuperBlock===================================*/
/* 系统全局超级块SuperBlock对象 */
SuperBlock g_spb;

SuperBlock::SuperBlock()
{
	
}

SuperBlock::~SuperBlock(){}

/*==============================class FileSystem===================================*/
FileSystem::FileSystem(){}

FileSystem::~FileSystem(){}

void FileSystem::FormatSuperBlock()
{
	sBlock->s_isize = FileSystem::INODE_ZONE_SIZE;
	sBlock->s_ninode = 0;
	sBlock->s_ilock = 0;

	sBlock->s_fsize = FileSystem::DATA_ZONE_SIZE;
	sBlock->s_nfree = 0;
	sBlock->s_free[0] = -1;
	sBlock->s_flock = 0;

	sBlock->s_fmod = 0;
	sBlock->s_ronly = 0;
	sBlock->s_time = 0xAABBCCDD;
	sBlock->padding[46] = 0x473C2B1A;
}

void FileSystem::FormatFileSystem()
{
	FormatSuperBlock();
	dDriver->OpenFile();	//将原文件变为空文件

	dDriver->BWrite(sBlock, sizeof(sBlock), SUPER_BLOCK_START_SECTOR *SECTOR_SIZE);	//从200写入

	//diskinode初始化
	DiskInode root, empty;
	root.d_mode |= Inode::IALLOC | Inode::IFDIR;
	root.d_nlink = 1;
	dDriver->BWrite(&root, sizeof(root), INODE_ZONE_START_SECTOR * SECTOR_SIZE);

	sBlock->s_ninode = 0;
	for (int i = 1; i < FileSystem::INODE_NUMBERS; i++)	//将root的预留
	{
		if (sBlock->s_ninode <= 100)
		{
			sBlock->s_inode[sBlock->s_ninode++] = i + INODE_ZONE_START_SECTOR;
		}
		dDriver->BWrite(&empty, sizeof(empty), 0, SEEK_CUR);
	}


	sBlock->s_nfree = 0;
	//磁盘文件数据区初始化
	char fBlock[SECTOR_SIZE], fBlock1[SECTOR_SIZE];
	for (int j = 0; j < FileSystem::DATA_ZONE_SIZE; j++)
	{
		if (sBlock->s_nfree <= 100)
		{
			dDriver->BWrite(fBlock1, sizeof(fBlock1), (DATA_ZONE_START_SECTOR + j) * SECTOR_SIZE);
		}
		else
		{
			memcpy(fBlock, sBlock->s_free, 101 * sizeof(int));
			dDriver->BWrite(fBlock, sizeof(fBlock), (DATA_ZONE_START_SECTOR + j) * SECTOR_SIZE);
			sBlock->s_nfree = 0;
			memset(sBlock->s_free, 0, sizeof(sBlock->s_free));
		}
		sBlock->s_free[sBlock->s_nfree++] = j + DATA_ZONE_START_SECTOR;
	}

	time((time_t*)& sBlock->s_time);

	dDriver->BWrite(sBlock, sizeof(sBlock), SUPER_BLOCK_START_SECTOR * SECTOR_SIZE);
}

void FileSystem::Initialize()
{
	//this->updlock = 0;
}

void FileSystem::LoadSuperBlock()
{
	char* buf;	//事实上，就是分配512字节的大小

	for (int i = 0; i < 2; i++)
	{
		buf = (char*)malloc(sizeof(char) * 512);
		if (buf == NULL) exit(1);

		int* p = (int*)& g_spb + i * 128;

		dDriver->BRead(buf,512,(SUPER_BLOCK_START_SECTOR +i)*SECTOR_SIZE);
		Utility::DWordCopy((int*)buf, p, 128);

		free(buf);
		buf = NULL;
	}

	g_spb.s_flock = 0;
	g_spb.s_ilock = 0;
	g_spb.s_ronly = 0;
	g_spb.s_time = 0;//并没有引入时间的概念
}

void FileSystem::Update()
{
	
}

Inode* FileSystem::IAlloc()
{
	int nInode;	//分配的inode号
	Inode* pInode;
	Buffer* pBuf;

	if (sBlock->s_ninode <= 0)
	{
		nInode = -1;

		//一次读磁盘Inode，搜索空闲inode
		for (int i = 0; i < sBlock->s_isize; i++)
		{
			pBuf = this->bManager->Bread(FileSystem::INODE_ZONE_START_SECTOR + i);

			//获取缓冲区首地址
			int* p = (int*)pBuf->b_addr;

			//检查该缓冲区的每个diskInode的mode，若！=0表示已经被占用
			for (int j = 0; j < FileSystem::INODE_NUMBER_PER_SECTOR; j++)
			{
				++nInode;//记录inode号

				int mode = *(p + j * sizeof(DiskInode) / sizeof(int));
				if (mode != 0)
				{
					continue;
				}

				//如果diskinode的mode=0，继续判断内存inode
				if (g_InodeTable.IsLoaded(nInode) == -1) 
				{
					sBlock->s_inode[sBlock->s_ninode++] = nInode;
					if (sBlock->s_ninode >= 100)
					{
						break;
					}
				}
			}

			//前往下一个inode的盘块搜索
			this->bManager->Brelse(pBuf);

			if (sBlock->s_ninode >= 100)
			{
				break;
			}
		}

		sBlock->s_ilock = 0;

		if (sBlock->s_ninode <= 0)
		{
			printf("No Space On Device!");
			return NULL;
		}
	}

	while (true)
	{
		nInode = sBlock->s_inode[--sBlock->s_ninode];

		//将空闲Inode读入内存
		pInode = g_InodeTable.IGet(nInode);
		if (NULL == pInode)
		{
			return NULL;
		}

		if (0 == pInode->i_mode)
		{
			pInode->Clean();
			sBlock->s_fmod = 1;
			return pInode;
		}
		else
		{
			g_InodeTable.IPut(pInode);
			continue;
		}
	}
	return NULL;
}

void FileSystem::IFree(int number)
{
	if (sBlock->s_ninode < 100)
	{
		sBlock->s_inode[sBlock->s_ninode++] = number;
		sBlock->s_fmod = 1;
	}
	else
		return;
}

Buffer* FileSystem::Alloc()
{
	int nBlock;	//分配的盘块号
	Buffer* pBuf;

	nBlock = sBlock->s_free[--sBlock->s_nfree];
	if (nBlock <= 0)
	{
		sBlock->s_nfree = 0;
		return NULL;
	}

	//如果分配完，将free[0]的信息(前101个)填入SuperBlock
	if (sBlock->s_nfree <= 0)	
	{
		pBuf = bManager->GetBlock(nBlock);
		int* p = (int*)pBuf->b_addr;
		sBlock->s_nfree = *p++;
		memcpy(sBlock->s_free, p, sizeof(sBlock->s_free));
		bManager->Brelse(pBuf);
	}

	pBuf = bManager->GetBlock(nBlock);
	if (pBuf)
	{
		bManager->ClrBuf(pBuf);
	}

	sBlock->s_fmod = 1;
	return pBuf;
}

void FileSystem::Free(int nBlock)
{
	Buffer* pBuf;

	if (sBlock->s_nfree >= 100)
	{
		//将s_free和s_nfree写入回收的盘块
		pBuf = bManager->GetBlock(nBlock);
		int* p = (int*)pBuf->b_addr;
		*p++ = sBlock->s_nfree;
		memcpy(p, sBlock->s_free, sizeof(sBlock->s_free));
		bManager->Bwrite(pBuf);

		sBlock->s_nfree = 0;
	}

	sBlock->s_free[sBlock->s_nfree++] = nBlock;
	sBlock->s_fmod = 1;
}
