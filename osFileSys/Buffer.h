#pragma once
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