#pragma once
#include "Inode.h"

class InodeTable
{
public:
	static const int NINODE = 100; //内存inode的数量

public:
	InodeTable();
	~InodeTable();

public:
	/*
	 * @comment 初始化对g_FileSystem对象的引用
	 */
	void Initialize();
	/*
	 * @comment 根据指定设备号dev，外存Inode编号获取对应
	 * Inode。如果该Inode已经在内存中，对其上锁并返回该内存Inode，
	 * 如果不在内存中，则将其读入内存后上锁并返回该内存Inode
	 */
	Inode* IGet(int inumber);
	/*
	 * @comment 减少该内存Inode的引用计数，如果此Inode已经没有目录项指向它，
	 * 且无进程引用该Inode，则释放此文件占用的磁盘块。
	 */
	void IPut(Inode* pNode);

	/*
	 * @comment 将所有被修改过的内存Inode更新到对应外存Inode中
	 */
	void UpdateInodeTable();

	/*
	 * @comment 检查设备dev上编号为inumber的外存inode是否有内存拷贝，
	 * 如果有则返回该内存Inode在内存Inode表中的索引
	 */
	int IsLoaded(int inumber);
	/*
	 * @comment 在内存Inode表中寻找一个空闲的内存Inode
	 */
	Inode* GetFreeInode();

public:
	Inode m_Inode[NINODE];
	FileSystem* m_FileSystem;
};