#include "Utility.h"

void Utility::DWordCopy(int* src, int* dst, int count)
{
	while (count--)
	{
		*dst++ = *src++;
	}
	return;
}