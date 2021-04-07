#include "Execute.h"

extern FileManager g_FileManager;


Execute::Execute()
{
	errmsg = "";
}
Execute::~Execute(){}


/* 指令相关 */
void Execute::GetCmd(const string& lineInput)
{
	this->args.clear();

	string str;
	stringstream strStream(lineInput);

	while (strStream >> str)
	{
		this->args.push_back(str);
	}

	return;
}

bool Execute::JudgeArgNum(const string& cmd, int num)
{
	if (args.size() - 1 > num)
	{
		cout << cmd << "的参数个数错误，应该为<=" << num - 1 << "个\n";
		return false;
	}
	return true;
}

void Execute::GetPaths(const string& path)
{

}

/* 目录相关 */
bool Execute::Ls()
{
	string path = this->args[1];
	char nowpath[28];
	int nowidx = 0;
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] != '/')
		{
			if (nowidx >= 27)
			{
				errmsg = "目录名长度超过限制!";
				return false;
			}
			nowpath[nowidx++] = path[i];
		}
		else
		{
			nowpath[nowidx++] = '\0';

			//如果是根目录
			if (nowidx == 1)
			{
				curDir = g_FileManager.rootDirInode;
			}
			else
			{

			}


			nowidx = 0;
		}
	}

	return true;
}

void Execute::Mkdir()
{

}

void Execute::FCreat()
{

}

void Execute::FOpen()
{

}

void Execute::FClose()
{

}

void Execute::FRead()
{

}

void Execute::FWrite()
{

}

void Execute::FLSeek()
{


}

void Execute::FDelete()
{

}

void Execute::FFormat()
{

}
