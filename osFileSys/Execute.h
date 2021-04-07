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

	void FFormat();	//��ʽ���ļ���

	/* Ŀ¼��� */
	bool Ls();		//��Ŀ¼
	void Mkdir();	//����Ŀ¼

	/* �ļ���� */
	void FCreat();	//�½��ļ�
	void FOpen();	//���ļ�
	void FClose();	//�ر��ļ�
	void FRead();	//���ļ�
	void FWrite();	//д�ļ�
	void FLSeek();	//��λ�ļ���дָ��
	void FDelete();	//ɾ���ļ�

	/* ָ����� */
	void GetCmd(const string& lineInput);
	bool JudgeArgNum(const string& cmd, int num);
	void GetPaths(const string& path);
};
