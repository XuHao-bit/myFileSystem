#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "FileManager.h"
using namespace std;

class Execute
{
public:
	vector<string> args;
	string errmsg;
	char paths[28];
	Inode* curDir;
	Directory nowDir;

public:
	Execute();
	~Execute();

	void FFormat();	//格式化文件卷

	/* 目录相关 */
	bool Ls();		//列目录
	void Mkdir();	//创建目录

	/* 文件相关 */
	void FCreat();	//新建文件
	void FOpen();	//打开文件
	void FClose();	//关闭文件
	void FRead();	//读文件
	void FWrite();	//写文件
	void FLSeek();	//定位文件读写指针
	void FDelete();	//删除文件

	/* 指令相关 */
	void GetCmd(const string& lineInput);
	bool JudgeArgNum(const string& cmd, int num);
	void GetPaths(const string& path);
};
