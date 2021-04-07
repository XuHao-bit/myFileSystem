#include "Inode.h"


Inode::Inode() 
{
	this->i_flag = 0;
	this->i_mode = 0;
	this->i_count = 0;
	this->i_nlink = 0;
	this->i_number = -1;
	this->i_uid = -1;
	this->i_gid = -1;
	this->i_size = 0;
	this->i_lastr = -1;
	//Utility::memset(i_addr, 0, sizeof(i_addr));
}

Inode::~Inode() 
{

}
