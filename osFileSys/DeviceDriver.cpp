#include "DeviceDriver.h"

DeviceDriver::DeviceDriver()
{
	fp = fopen(DISK_FILE_NAME, "rb+");
}

DeviceDriver::~DeviceDriver()
{
	if (fp)
	{
		fclose(fp);
	}
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
