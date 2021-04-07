#pragma once

#include "File.h"

class OpenFileTable; //ϵͳ���ļ���
class InodeTable; //�ڴ�Inode��

class OpenFileTable
{
public:
	static const int NFILE = 100;	/* ���ļ����ƿ�File�ṹ������ */
public:
	OpenFileTable();
	~OpenFileTable();

	/* ��ϵͳ���ļ����з���һ�����е�File�ṹ */
	File* FAlloc();

	/* �Դ��ļ����ƿ�File�ṹ�����ü���count��1�������ü���countΪ0�����ͷ�File�ṹ��*/
	void CloseF(File* pFile);

	/* ��ʽ�� */
	void Format();

public:
	File m_File[NFILE];
};


class InodeTable
{
public:
	static const int NINODE = 100; //�ڴ�inode������

public:
	InodeTable();
	~InodeTable();

public:
	/*
	 * @comment ��ʼ����g_FileSystem���������
	 */
	void Initialize();
	/*
	 * @comment �������Inode��Ż�ȡ��Ӧ
	 * Inode�������Inode�Ѿ����ڴ��У��������������ظ��ڴ�Inode��
	 * ��������ڴ��У���������ڴ�����������ظ��ڴ�Inode
	 */
	Inode* IGet(int inumber);

	/*
	 * @comment ���ٸ��ڴ�Inode�����ü����������Inode�Ѿ�û��Ŀ¼��ָ������
	 * ���޽������ø�Inode�����ͷŴ��ļ�ռ�õĴ��̿顣
	 */
	void IPut(Inode* pNode);

	/*
	 * @comment �����б��޸Ĺ����ڴ�Inode���µ���Ӧ���Inode��
	 */
	void UpdateInodeTable();

	/*
	 * @comment ����豸dev�ϱ��Ϊinumber�����inode�Ƿ����ڴ濽����
	 * ������򷵻ظ��ڴ�Inode���ڴ�Inode���е�����
	 */
	int IsLoaded(int inumber);

	/*
	 * @comment ���ڴ�Inode����Ѱ��һ�����е��ڴ�Inode
	 */
	Inode* GetFreeInode();

public:
	Inode m_Inode[NINODE];
	FileSystem* m_FileSystem;
};
