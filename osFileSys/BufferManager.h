#pragma once
#include "Buffer.h"
#include "DeviceDriver.h"
#include "FileSystem.h"
class Buffer
{
public:
	enum BufferFlag
	{
		B_WRITE = 0x1,		/* 写操作。将缓存中的信息写到硬盘上去 */
		B_READ = 0x2,		/* 读操作。从盘读取信息到缓存中 */
		B_DONE = 0x4,		/* I/O操作结束 */
		B_ERROR = 0x8,		/* I/O因出错而终止 */
		B_BUSY = 0x10,		/* 相应缓存正在使用中 */
		B_WANTED = 0x20,	/* 有进程正在等待使用该buf管理的资源，清B_BUSY标志时，要唤醒这种进程 */
		B_ASYNC = 0x40,		/* 异步I/O，不需要等待其结束 */
		B_DELWRI = 0x80		/* 延迟写，在相应缓存要移做他用时，再将其内容写到相应块设备上 */
	};

public:
	unsigned int flags;	//缓存控制块标志位

	int padding;
	Buffer* b_forw;
	Buffer* b_back;
	Buffer* av_forw;
	Buffer* av_back;

	short b_dev;	//主次设备号
	int b_wcount;	//需传送字节数
	unsigned char* b_addr;
	int b_nblk;		//磁盘逻辑块号
	int b_error;	//I/O出错信息
	int b_resid;	//I/O出错尚未传送的字节数
};


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

