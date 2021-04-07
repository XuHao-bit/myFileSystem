#pragma once
#include "Inode.h"
//#include "FileSystem.h"
//#include "OpenFileTable.h"

class DirectoryItem;

class FileManager
{
public:
	enum DirectorySearchMode
	{
		OPEN = 0,
		CREATE = 1,
		DELETE = 2
	};

public:
	FileManager();
	~FileManager();

public:
	Inode* rootDirInode;
	//FileSystem* m_FileSystem;
	//InodeTable* m_InodeTable;//内存Inode表
	//OpenFileTable* m_OpenFileTable;//系统打开文件表
};



/*	目录项数据结构	*/
class DirectoryItem
{
public:
	static const int DIR_ITEM_SIZE = 28;

public:
	char directoryName[DIR_ITEM_SIZE];
	int nDiskInode;//外存inode号
};


/*	目录数据结构		*/
class Directory
{
	int dirNum;
	DirectoryItem dItem[16];
};