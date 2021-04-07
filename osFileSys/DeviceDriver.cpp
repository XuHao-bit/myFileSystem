#include "DeviceDriver.h"

extern DevTable g_DevTable;

DeviceDriver::DeviceDriver()
{
	fp = fopen(DISK_FILE_NAME, "rb+");
	dTable = &g_DevTable;
}

DeviceDriver::~DeviceDriver()
{
	if (fp)
	{
		fclose(fp);
	}
	dTable = NULL;
}

void DeviceDriver::BWrite(const void* buf, unsigned size,
	int offset = -1, int whence = SEEK_SET)
{
	if (offset >= 0)
	{
		fseek(fp, offset, whence);
	}
	fwrite(buf, size, 1, fp);
}

void DeviceDriver::BRead(void* buf, unsigned size,
	int offset = -1, int whence = SEEK_SET)
{
	if (offset >= 0)
	{
		fseek(fp, offset, whence);
	}
	fread(buf, size, 1, fp);
}

void DeviceDriver::OpenFile()
{
	fp = fopen(DISK_FILE_NAME, "wb+");//变成空文件
	if (NULL == fp)
	{
		printf("打开文件%s失败", DISK_FILE_NAME);
		exit(-1);
	}
}

int DeviceDriver::GetDevTable()
{

}