#pragma once

class Utility
{
public:
	/* 以src为源地址，dst为目的地址，复制count个双字 */
	static void DWordCopy(int* src, int* dst, int count);
};