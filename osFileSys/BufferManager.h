#pragma once
#include "Buffer.h"
#include "DeviceDriver.h"
#include "FileSystem.h"

class BufferManager
{
public:
	static const int NBUF = 15;
	static const int BUFFER_SIZE = 512;

public:
	BufferManager();
	~BufferManager();

	void Initialize();	//缓存控制块队列的初始化
	Buffer* GetBlock(int nBlock);	//读取nBlock
	void Brelse(Buffer* buf);		//释放buf

	Buffer* Bread(int nBlock);	//读一块盘块
	void Bwrite(Buffer* buf);	//写一块磁盘块

	void ClrBuf(Buffer* buf);	//将buf清空
	Buffer& GetBFreeList();		//访问private属性bFreelist

private:
	void NotAvail(Buffer* buf);	//从自由队列摘下缓存控制块

private:
	Buffer bFreeList;	//自由队列
	Buffer mBuf[NBUF];	//缓存控制数组
	unsigned char myBuffer[NBUF][BUFFER_SIZE];	//实际缓冲区
	DeviceDriver* dDriver;	//设备管理驱动（实际只有一块设备）
};