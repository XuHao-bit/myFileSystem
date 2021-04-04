#pragma once
#include <stdio.h>

class DeviceDriver
{
public:
	const char* DISK_FILE_NAME = "myDevice.img";

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
};