#pragma once
#include "Inode.h"

class File
{
public:
	enum FileFlags
	{

	};

public:
	File();
	~File();

public:
	unsigned int f_flag; 
	int		f_count; //process number of this file
	Inode*	f_inode; //the memory Inode pointer of this file
	int		f_offset;//location of this file pointer

};
