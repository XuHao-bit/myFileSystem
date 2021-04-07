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
		封装c语言库fwrite,fread
		参数：
			a)offset: 相对于whence的偏移量
			b)whence: SEEK_SET(文件开头)、SEEK_CUR(文件当前位置)、SEEK_END(文件末尾)
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