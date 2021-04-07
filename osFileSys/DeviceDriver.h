#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "BufferManager.h"

class DevTable;

class DeviceDriver
{
public:
	const char* DISK_FILE_NAME = "myDevice.img";
	DevTable* dTable;

private:
	FILE* fp;

public:
	DeviceDriver();
	~DeviceDriver();

	/* 
		��װc���Կ�fwrite,fread
		������
			a)offset: �����whence��ƫ����
			b)whence: SEEK_SET(�ļ���ͷ)��SEEK_CUR(�ļ���ǰλ��)��SEEK_END(�ļ�ĩβ)
	*/
	void BWrite(const void* buf, unsigned size,
		int offset = -1, int whence = SEEK_SET);

	void BRead(void* buf, unsigned int size,
		int offset = -1, int whence = SEEK_SET);

	void OpenFile();

	int GetDevTable();
};

class DevTable
{
public:
	DevTable();
	~DevTable();
public:
	int d_active;
	int d_errcnt;
	Buffer* b_forw;
	Buffer* b_back;
	Buffer* d_actf;
	Buffer* d_actl;
};