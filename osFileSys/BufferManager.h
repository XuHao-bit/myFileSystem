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

	Buffer* GetBlock(int nBlock);	//∂¡»°nBlock
	void Brealse(Buffer* buf);		// Õ∑≈buf
};