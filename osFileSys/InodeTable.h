#pragma once
#include "Inode.h"

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
	 * @comment ����ָ���豸��dev�����Inode��Ż�ȡ��Ӧ
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