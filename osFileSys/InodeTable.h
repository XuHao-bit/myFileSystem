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
	Inode m_Inode[NINODE];

};