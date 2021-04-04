#pragma once

#include "File.h"

class OpenFileTable; //ϵͳ���ļ���
class InodeTable; //�ڴ�Inode��

class OpenFileTable
{
public:
	static const int NFILE = 100;	/* ���ļ����ƿ�File�ṹ������ */
public:
	OpenFileTable();
	~OpenFileTable();

public:
	File m_File[NFILE];
};

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
