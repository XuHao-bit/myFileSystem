#pragma once

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

};