#pragma once
#include "Buffer.h"
#include "DeviceDriver.h"
#include "FileSystem.h"

class BufferManager
{
public:
	DeviceDriver* dDriver;

public:
	BufferManager();
	~BufferManager();

	Buffer* GetBlock(int nBlock);	//��ȡnBlock
	void Brealse(Buffer* buf);		//�ͷ�buf
};