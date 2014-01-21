#ifndef __CC_UTILS_H__
#define __CC_UTILS_H__

#include "CCMacro.h"
#include <string>

namespace CC {

static inline char convertSplash(char ch)
{
	ch = (ch == '\\' ? '/' : ch);
	return ch;
}

unsigned char* getFileBuf(const char* pszFileName, const char* pszMode, unsigned long * pSize);


std::wstring mbcsToUnicode(const char* pMbcs);
std::string unicodeToMbcs(const wchar_t* pUnicode);
std::wstring utf8ToUnicode(const char* pUtf8);
std::string unicodeToUtf8(const wchar_t* pUnicode);
std::string mbcsToUtf8(const char* pMbcs);
std::string utf8ToMbcs(const char* pUtf8);


} // namespace

#endif //__CC_UTILS_H__