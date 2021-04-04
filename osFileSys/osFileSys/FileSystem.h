#pragma once

class SuperBlock
{
public:
	SuperBlock();
	~SuperBlock();

public:
	/*对空闲Inode进行管理*/
	int s_isize;//
	int s_ninode;//number of free Inode
	int s_inode[100];
	int s_ilock;//lock inode 

	/*对空闲数据块进行管理*/
	int s_fsize;
	int s_nfree;
	int s_free[100];
	int s_flock;

};