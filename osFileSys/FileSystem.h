#pragma once
#include "Inode.h"

class SuperBlock
{
public:
	SuperBlock();
	~SuperBlock();

public:
	/*对空闲Inode进行管理*/
	int s_isize;//
	int s_ninode;//number of free Inode
	int s_inode[100];
	int s_ilock;//lock inode 

	/*对空闲数据块进行管理*/
	int s_fsize;
	int s_nfree;
	int s_free[100];
	int s_flock;

	//下面的（except padding）我不确定用不用的到，参考的unix v6++的源代码
	int	s_fmod;			/* 内存中super block副本被修改标志，意味着需要更新外存对应的Super Block */
	int	s_ronly;		/* 本文件系统只能读出 */
	int	s_time;			/* 最近一次更新时间 */
	int padding[47];		/* 使superblock能够恰好占用两个块*/

};

class FileSystem 
{
public:
	/* 静态常量 */
	//下面的变量参考自unix v6++代码
	static const int NMOUNT = 5;
	
	static const int SUPER_BLOCK_SECTOR_NUMBER = 200;	//superblock起始扇区号
	
	static const int ROOTINO = 0;	//文件系统根目录的外存inode编号

	static const int INODE_NUMBER_PER_SECTOR = 8;	//每个扇区外存Inode的个数（512/64 = 8）
	static const int INODE_ZONE_START_SECTOR = 202;	//外存inode的起始扇区号
	static const int INODE_ZONE_SIZE = 1024 - 202;	//外存inode占用扇区的大小

	static const int DATA_ZONE_START_SECTOR = 1024;		//文件数据区的起始扇区号
	static const int DATA_ZONE_END_SECTOR = 18000 - 1;	//文件数据区的结束扇区号
	static const int DATA_ZONE_SIZE = 18000 - DATA_ZONE_START_SECTOR;	//文件数据区占用扇区大小

public:
	FileSystem();
	~FileSystem();

	/* 顾名思义 */
	void Initialize();

	void LoadSuperBlock();

	/* 根据文件存储设备的设备号dev获取SuperBlock */
	SuperBlock* GetSuperBlock(short dev);//作用未知？？

	/* 更新内存SuperBlock的内存副本到磁盘 */
	void Update();//

	/* 
		以下涉及SuperBlock块的功能：
		1. 对空闲Inode的管理
		2. 对空闲数据块的管理
		（以上理解错了）

		下面好像是高速缓存队列中的
	*/

	/* 在存储设备dev上分配一个空闲inode */
	Inode* IAlloc(short dev);

	/* 释放存储设备dev上编号为number的外存INode，一般用于删除文件。 */
	void IFree(short dev, int number);

	/* 在存储设备dev上分配空闲磁盘块 */
	// Buf* Alloc(short dev);
	
	/* 释放存储设备dev上编号为blkno的磁盘块  */
	void Free(short dev, int blkno);

	/* 查找文件系统装配表，搜索指定Inode对应的Mount装配块 */
	//Mount* GetMount(Inode* pInode);

private:
	/* 检查设备dev上编号blkno的磁盘块是否属于数据盘块区 */
	bool BadBlock(SuperBlock* spb, short dev, int blkno);

public:
	//Mount m_Mount[NMOUNT];		/* 文件系统装配块表，Mount[0]用于根文件系统 */

private:
	//BufferManager* m_BufferManager;		/* FileSystem类需要缓存管理模块(BufferManager)提供的接口 */
	int updlock;				/* Update()函数的锁，该函数用于同步内存各个SuperBlock副本以及，
								被修改过的内存Inode。任一时刻只允许一个进程调用该函数 */


};

/* 外存inode，每个大小64bytes */
class DiskInode
{
public:
	DiskInode();
	~DiskInode();

public:
	unsigned int d_mode;
	int		d_nlink;

	short	d_uid;//owner's identifier
	short	d_gid;//owner's group identifier

	int		d_size;
	int		d_addr[10];

	int		d_atime;//last access time
	int		d_mtime;//last modify time

};
