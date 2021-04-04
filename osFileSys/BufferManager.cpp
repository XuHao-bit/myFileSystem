#include "BufferManager.h"

extern DeviceDriver g_DeviceDriver;

BufferManager::BufferManager()
{
	dDriver = &g_DeviceDriver;
}
BufferManager::~BufferManager(){}

Buffer* BufferManager::GetBlock(int nBlock)
{
	Buffer* pBuf;
	dDriver->BRead(pBuf->b_addr, FileSystem::SECTOR_SIZE, nBlock * FileSystem::SUPER_BLOCK_SECTOR_NUMBER);
	return pBuf;
}

void BufferManager::Brealse(Buffer* buf)
{

}