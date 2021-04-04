#include "FileSystem.h"

extern DeviceDriver g_DeviceDriver;
extern BufferManager g_BufferManager;
extern SuperBlock g_SuperBlock;


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

		dDriver->BRead(buf,512,(SUPER_BLOCK_SECTOR_NUMBER+i)*SECTOR_SIZE);
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
	int i;
	SuperBlock* sb;

	//for(i = 0;i)
}

Inode* FileSystem::IAlloc()//important!!
{
	int nInode;	//分配的inode号
	Inode* pInode;

	if (sBlock->s_ninode > 0)
	{
		nInode = sBlock->s_inode[--sBlock->s_ninode];
		
	}
	else 
	{

	}
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
		int* newBuf = (int*)malloc(101*sizeof(int));
		if (newBuf == NULL) exit(1);

		dDriver->BRead(newBuf, 101*sizeof(int), (sBlock->s_free[0]) * SECTOR_SIZE);

		sBlock->s_nfree = *newBuf++;
		memcpy(sBlock->s_free, newBuf, sizeof(sBlock->s_free));
		free(newBuf);
	}

	pBuf = ;
	if (pBuf)
	{

	}
	sBlock->s_fmod = 1;
	return pBuf;
}

void FileSystem::Free(int blkno)
{
	if (sBlock->s_nfree >= 100)
	{
		//将s_free和s_nfree写入回收的盘块

		sBlock->s_nfree = 0;
	}

	sBlock->s_free[sBlock->s_nfree++] = blkno;
	sBlock->s_fmod = 1;
}
