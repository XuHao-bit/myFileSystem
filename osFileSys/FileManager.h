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
	//InodeTable* m_InodeTable;//�ڴ�Inode��
	//OpenFileTable* m_OpenFileTable;//ϵͳ���ļ���
};



/*	Ŀ¼�����ݽṹ	*/
class DirectoryItem
{
public:
	static const int DIR_ITEM_SIZE = 28;

public:
	char directoryName[DIR_ITEM_SIZE];
	int nDiskInode;//���inode��
};


/*	Ŀ¼���ݽṹ		*/
class Directory
{
	int dirNum;
	DirectoryItem dItem[16];
};