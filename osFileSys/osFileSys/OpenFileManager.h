#pragma once

#include "File.h"

class OpenFileTable; //ϵͳ���ļ���

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


