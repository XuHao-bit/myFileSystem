#pragma once
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include "Inode.h"
#include "DeviceDriver.h"
#include "Utility.h"
#include "BufferManager.h"
#include "FileManager.h"

class SuperBlock
{
public:
	SuperBlock();
	~SuperBlock();

public:
	/*�Կ���Inode���й���*/
	int s_isize;//
	int s_ninode;//number of free Inode
	int s_inode[100];
	int s_ilock;//lock inode 

	/*�Կ������ݿ���й���*/
	int s_fsize;
	int s_nfree;
	int s_free[100];
	int s_flock;

	//����ģ�except padding���Ҳ�ȷ���ò��õĵ����ο���unix v6++��Դ����
	int	s_fmod;			/* �ڴ���super block�������޸ı�־����ζ����Ҫ��������Ӧ��Super Block */
	int	s_ronly;		/* ���ļ�ϵͳֻ�ܶ��� */
	int	s_time;			/* ���һ�θ���ʱ�� */
	int padding[47];		/* ʹsuperblock�ܹ�ǡ��ռ��������*/

};

class FileSystem 
{
public:
	/* ��̬���� */
	//����ı����ο���unix v6++����
	static const int NMOUNT = 5;
	
	static const int SECTOR_SIZE = 512;

	static const int SUPER_BLOCK_START_SECTOR = 200;	//superblock��ʼ������
	
	static const int ROOTINO = 0;	//�ļ�ϵͳ��Ŀ¼�����inode���

	static const int INODE_NUMBER_PER_SECTOR = 8;	//ÿ���������Inode�ĸ�����512/64 = 8��
	static const int INODE_ZONE_START_SECTOR = 202;	//���inode����ʼ������
	static const int INODE_ZONE_SIZE = 1024 - 202;	//���inodeռ�������Ĵ�С
	static const int INODE_NUMBERS = INODE_ZONE_SIZE * INODE_NUMBER_PER_SECTOR;	//���Inode������

	static const int DATA_ZONE_START_SECTOR = 1024;		//�ļ�����������ʼ������
	static const int DATA_ZONE_END_SECTOR = 18000 - 1;	//�ļ��������Ľ���������
	static const int DATA_ZONE_SIZE = 18000 - DATA_ZONE_START_SECTOR;	//�ļ�������ռ��������С

public:
	BufferManager* bManager;
	DeviceDriver* dDriver;
	SuperBlock* sBlock;

public:
	FileSystem();
	~FileSystem();

	/* ����˼�� */
	void Initialize();

	void LoadSuperBlock();

	/* �����ڴ�SuperBlock���ڴ渱�������� */
	void Update();

	/* 
		�����漰SuperBlock��Ĺ��ܣ�
		1. �Կ���Inode�Ĺ���
		2. �Կ������ݿ�Ĺ���
		�����������ˣ�

		��������Ǹ��ٻ�������е�
	*/

	/* �ڴ洢�豸dev�Ϸ���һ������inode */
	Inode* IAlloc();

	/* �ͷŴ洢�豸dev�ϱ��Ϊnumber�����INode��һ������ɾ���ļ��� */
	void IFree(int number);

	/* �ڴ洢�豸dev�Ϸ�����д��̿� */
	Buffer* Alloc();
	
	/* �ͷŴ洢�豸dev�ϱ��Ϊblkno�Ĵ��̿�  */
	void Free(int blkno);

	/* ��ʽ��superblock */
	void FormatSuperBlock();

	/* ��ʽ�������ļ�ϵͳ */
	void FormatFileSystem();

private:
	/* ����豸dev�ϱ��blkno�Ĵ��̿��Ƿ����������̿��� */
	bool BadBlock(SuperBlock* spb, short dev, int blkno);




};

/* ���inode��ÿ����С64bytes */
class DiskInode
{
public:
	DiskInode();
	~DiskInode();

public:
	unsigned int d_mode;
	int		d_nlink;

	short	d_uid;//owner's identifier
	short	d_gid;//owner's group identifier

	int		d_size;
	int		d_addr[10];

	int		d_atime;//last access time
	int		d_mtime;//last modify time

};
