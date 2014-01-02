#include "CCUtils.h"
#include <cstdio>
#include <string>

unsigned char* getFileData( const char* pszFileName, const char* pszMode, unsigned long * pSize )
{
	unsigned char * pBuffer = NULL;
	*pSize = 0;
	do
	{
		// read the file from hardware
		FILE *fp = fopen(pszFileName, pszMode);
		if (fp == NULL) break;

		fseek(fp,0,SEEK_END);
		*pSize = ftell(fp);
		fseek(fp,0,SEEK_SET);
		pBuffer = new unsigned char[*pSize];
		*pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
		fclose(fp);
	} while (0);

	if (! pBuffer)
	{
		std::string msg = "Get data from file(";
		msg.append(pszFileName).append(") failed!");
	}
	return pBuffer;
}