#pragma once
#include "Inode.h"

class InodeTable
{
public:
	static const int NINODE = 100; //�ڴ�inode������

public:
	InodeTable();
	~InodeTable();

public:
	Inode m_Inode[NINODE];

};