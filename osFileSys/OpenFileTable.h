#pragma once

#include "File.h"

class OpenFileTable; //系统打开文件表
class InodeTable; //内存Inode表

class OpenFileTable
{
public:
	static const int NFILE = 100;	/* 打开文件控制块File结构的数量 */
public:
	OpenFileTable();
	~OpenFileTable();

public:
	File m_File[NFILE];
};

class InodeTable
{
public:
	static const int NINODE = 100; //内存inode的数量

public:
	InodeTable();
	~InodeTable();

public:
	Inode m_Inode[NINODE];

};
