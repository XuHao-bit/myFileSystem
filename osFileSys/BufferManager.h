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