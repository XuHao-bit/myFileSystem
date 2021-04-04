#pragma once
#include "Inode.h"
#include "FileSystem.h"
#include "OpenFileTable.h"
#include "InodeTable.h"

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
	InodeTable* m_InodeTable;//内存Inode表
	OpenFileTable* m_OpenFileTable;//系统打开文件表
};
