#pragma once
class Block
{
	DirectoryItem dItem[16];
};

struct DirectoryItem
{
	char directoryName[28];
	int nDiskInode;//Õ‚¥Êinode∫≈
};
