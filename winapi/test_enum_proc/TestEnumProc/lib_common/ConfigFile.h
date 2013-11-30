#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_

#include "stdafx.h"
#include "BaseFunc.h"

class CConfigFile
{
public:
	explicit CConfigFile(const char* sFilePath);
	~CConfigFile();

public:
	bool isFileExist() { return BaseFunc::isFileExists(m_file_path); }
	bool getStrValue(const char* sSection, const char* sKey, char* sValue, int nBufCnt = MAX_PATH);
	bool getIntValue(const char* sSection, const char* sKey, int& nValue);
	bool setStrValue(const char* sSection, const char* sKey, const char* sValue);
	bool setIntValue(const char* sSection, const char* sKey, int sValue);

private:
	char m_file_path[MAX_PATH];
};

#endif // _CONFIG_FILE_H_