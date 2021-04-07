#pragma once
#include "Buffer.h"
#include "DeviceDriver.h"
#include "FileSystem.h"
class Buffer
{
public:
	enum BufferFlag
	{
		B_WRITE = 0x1,		/* д�������������е���Ϣд��Ӳ����ȥ */
		B_READ = 0x2,		/* �����������̶�ȡ��Ϣ�������� */
		B_DONE = 0x4,		/* I/O�������� */
		B_ERROR = 0x8,		/* I/O��������ֹ */
		B_BUSY = 0x10,		/* ��Ӧ��������ʹ���� */
		B_WANTED = 0x20,	/* �н������ڵȴ�ʹ�ø�buf�������Դ����B_BUSY��־ʱ��Ҫ�������ֽ��� */
		B_ASYNC = 0x40,		/* �첽I/O������Ҫ�ȴ������ */
		B_DELWRI = 0x80		/* �ӳ�д������Ӧ����Ҫ��������ʱ���ٽ�������д����Ӧ���豸�� */
	};

public:
	unsigned int flags;	//������ƿ��־λ

	int padding;
	Buffer* b_forw;
	Buffer* b_back;
	Buffer* av_forw;
	Buffer* av_back;

	short b_dev;	//�����豸��
	int b_wcount;	//�贫���ֽ���
	unsigned char* b_addr;
	int b_nblk;		//�����߼����
	int b_error;	//I/O������Ϣ
	int b_resid;	//I/O������δ���͵��ֽ���
};


class BufferManager
{
public:
	static const int NBUF = 15;
	static const int BUFFER_SIZE = 512;

public:
	BufferManager();
	~BufferManager();

	void Initialize();	//������ƿ���еĳ�ʼ��
	Buffer* GetBlock(int nBlock);	//��ȡnBlock
	void Brelse(Buffer* buf);		//�ͷ�buf

	Buffer* Bread(int nBlock);	//��һ���̿�
	void Bwrite(Buffer* buf);	//дһ����̿�

	void ClrBuf(Buffer* buf);	//��buf���
	Buffer& GetBFreeList();		//����private����bFreelist

private:
	void NotAvail(Buffer* buf);	//�����ɶ���ժ�»�����ƿ�

private:
	Buffer bFreeList;	//���ɶ���
	Buffer mBuf[NBUF];	//�����������
	unsigned char myBuffer[NBUF][BUFFER_SIZE];	//ʵ�ʻ�����
	DeviceDriver* dDriver;	//�豸����������ʵ��ֻ��һ���豸��
};

