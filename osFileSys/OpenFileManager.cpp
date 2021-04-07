#include "OpenFileManager.h"

//#include "FileSystem.h"

//InodeTable g_InodeTable;
//extern FileSystem g_FileSystem;
//extern BufferManager g_BufferManager;

OpenFileTable::OpenFileTable() {}

OpenFileTable::~OpenFileTable() {}


InodeTable::InodeTable()
{
	//nothing to do here
}

InodeTable::~InodeTable()
{
	//nothing to do here
}

void InodeTable::Initialize()
{
	/* ��ȡ��g_FileSystem������ */

}

Inode* InodeTable::IGet(int inumber)
{
	Inode* pInode;
	//User& u = Kernel::Instance().GetUser();

	while (true)
	{
		/* ���ָ���豸dev�б��Ϊinumber�����Inode�Ƿ����ڴ濽�� */
		int index = this->IsLoaded(inumber);
		if (index >= 0)	/* �ҵ��ڴ濽�� */
		{
			pInode = &(this->m_Inode[index]);
			/* ������ڴ�Inode������ */
			if (pInode->i_flag & Inode::ILOCK)
			{
				/* ����IWANT��־��Ȼ��˯�� */
				pInode->i_flag |= Inode::IWANT;

				//u.u_procp->Sleep((unsigned long)& pInode, ProcessManager::PINOD);

				/* �ص�whileѭ������Ҫ������������Ϊ���ڴ�Inode�����Ѿ�ʧЧ */
				continue;
			}

			/* ������ڴ�Inode�����������ļ�ϵͳ�����Ҹ�Inode��Ӧ��Mountװ��� */
			if (pInode->i_flag & Inode::IMOUNT)
			{
				continue;
			}

			/*
			 * ����ִ�е������ʾ���ڴ�Inode���ٻ������ҵ���Ӧ�ڴ�Inode��
			 * ���������ü���������ILOCK��־������֮
			 */
			pInode->i_count++;
			pInode->i_flag |= Inode::ILOCK;
			return pInode;
		}
		else	/* û��Inode���ڴ濽���������һ�������ڴ�Inode */
		{
			pInode = this->GetFreeInode();
			/* ���ڴ�Inode���������������Inodeʧ�� */
			if (NULL == pInode)
			{
				return NULL;
			}
			else	/* �������Inode�ɹ��������Inode�����·�����ڴ�Inode */
			{
				/* �����µ��豸�š����Inode��ţ��������ü������������ڵ����� */
				pInode->i_dev = 1;
				pInode->i_number = inumber;
				pInode->i_flag = Inode::ILOCK;
				pInode->i_count++;
				pInode->i_lastr = -1;

				BufferManager* bm = m_FileSystem->bManager;
				/* �������Inode���뻺���� */
				Buffer* pBuf = bm->Bread(FileSystem::INODE_ZONE_START_SECTOR + inumber / FileSystem::INODE_NUMBER_PER_SECTOR);

				/* �������I/O���� */
				if (pBuf->flags & Buffer::B_ERROR)
				{
					/* �ͷŻ��� */
					bm->Brelse(pBuf);
					/* �ͷ�ռ�ݵ��ڴ�Inode */
					this->IPut(pInode);
					return NULL;
				}

				/* ���������е����Inode��Ϣ�������·�����ڴ�Inode�� */
				pInode->ICopy(pBuf, inumber);
				/* �ͷŻ��� */
				bm->Brelse(pBuf);
				return pInode;
			}
		}
	}
	return NULL;	/* GCC likes it! */
}
