#include <stdio.h>
#include "Execute.h"

using namespace std;

Execute Exec;
/*DeviceDriver g_DeviceDriver;
BufferManager g_BufferManager;
OpenFileTable g_OpenFileTable;
InodeTable g_InodeTable;
FileSystem g_FileSystem;
SuperBlock g_SuperBlock;*/
FileManager g_FileManager;

int main()
{
	//vector<string> args;
	string cmd;
	//string arg1, arg2, arg3;
	string lineInput;

	while (1)
	{
		cout << "[Host root]# ";
		getline(cin, lineInput);

		Exec.GetCmd(lineInput);
		cmd = Exec.args[0];
		
		if (cmd == "help")
		{

		}
		else if (cmd == "ls")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.Ls();
			}
			else continue;
		}
		else if (cmd == "mkdir")
		{
			if (Exec.JudgeArgNum(cmd, 1))	
			{
				Exec.Mkdir();
			}
			else continue;
		}
		else if (cmd == "fcreat")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.FCreat();
			}
			else continue;
		}
		else if (cmd == "fopen")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.FOpen();
			}
			else continue;
		}
		else if (cmd == "fclose")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.FClose();
			}
			else continue;

		}
		else if (cmd == "fread")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.FRead();
			}
			else continue;
		}
		else if (cmd == "fwrite")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.FWrite();
			}
			else continue;
		}
		else if (cmd == "flseek")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.FLSeek();
			}
			else continue;
		}
		else if (cmd == "fdelete")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.FDelete();
			}
			else continue;
		}
		else if (cmd == "fformat")
		{
			if (Exec.JudgeArgNum(cmd, 1))	//ls参数最多为1
			{
				Exec.FFormat();
			}
			else continue;
		}
		else
		{

		}

		cout << lineInput<<endl;
	}

	return 0;
}