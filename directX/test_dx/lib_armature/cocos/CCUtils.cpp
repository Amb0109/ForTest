#include "CCUtils.h"
#include <cstdio>
#include <cassert>
#include <windows.h>

namespace CC {

unsigned char* getFileBuf( const char* pszFileName, const char* pszMode, unsigned long * pSize )
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

	if (!pBuffer)
	{
		std::string msg = "Get data from file(";
		msg.append(pszFileName).append(") failed!");
		printf("%s", msg.c_str());
	}
	return pBuffer;
}

std::wstring mbcsToUnicode(const char* pMbcs)
{
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	int nChar = MultiByteToWideChar(codepage, 0, pMbcs, -1, NULL,0);

	int nSize = nChar * sizeof(wchar_t) + 1;
	wchar_t* pwchBuffer = new wchar_t[nSize];

	nChar = MultiByteToWideChar(codepage, 0, pMbcs, -1, pwchBuffer, nSize);
	if(nChar == 0)
	{
		delete[] pwchBuffer;
		return L"";
	}

	pwchBuffer[nChar]=0;
	std::wstring wsRet(pwchBuffer);
	delete[] pwchBuffer;

	return wsRet;
}

std::string unicodeToMbcs(const wchar_t* pUnicode)
{
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	int nChar = WideCharToMultiByte(codepage, 0, pUnicode, -1, 0, 0, 0, 0);
	int nSize = nChar * sizeof(char) + 1;

	char* pchBuffer = new char[nSize];

	nChar = WideCharToMultiByte(codepage, 0, pUnicode, -1, pchBuffer, nSize, 0, 0);
	if( nChar == 0 )
	{
		delete[] pchBuffer;
		return NULL;
	}

	pchBuffer[nChar]= 0;
	std::string sRet(pchBuffer);
	delete[] pchBuffer;

	return sRet;
}

std::string mbcsToUtf8(const char* pMbcs)
{
	assert(pMbcs);
	return unicodeToUtf8(mbcsToUnicode(pMbcs).c_str());
}

std::string utf8ToMbcs(const char* pUtf8)
{
	assert(pUtf8);
	return unicodeToMbcs(utf8ToUnicode(pUtf8).c_str());
}

std::wstring utf8ToUnicode(const char* pUtf8)
{
	assert(pUtf8);
	int nChar = MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, NULL, 0);

	int nSize = nChar*sizeof(wchar_t) + 1;
	wchar_t* pwchBuffer = new wchar_t[nSize];

	nChar = MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, pwchBuffer, nSize);
	if(nChar == 0)
	{
		delete []pwchBuffer;
		return L"";
	}

	pwchBuffer[nChar] = 0;
	std::wstring wsRet(pwchBuffer);
	delete[] pwchBuffer;

	return wsRet;
}

std::string unicodeToUtf8(const wchar_t* pUnicode)
{
	assert(pUnicode);
	int nChar = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, 0, 0, 0, 0);

	int nSize = nChar*sizeof(char) + 1;
	char* pchBuffer = new char[nSize];

	nChar = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, pchBuffer, nSize, 0, 0);
	if( nChar == 0 )
	{
		delete[] pchBuffer;
		return "";
	}

	pchBuffer[nChar]= 0;
	std::string sRet(pchBuffer);
	delete[] pchBuffer;

	return sRet;
}

} // namespace