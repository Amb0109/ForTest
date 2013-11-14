#include "stdafx.h"
#include "BaseFunc.h"
#include "ConfigFile.h"

CConfigFile::CConfigFile(const char* sFilePath)
{
	BaseFunc::getFullPath(m_file_path, sFilePath);
}

CConfigFile::~CConfigFile()
{
}

bool CConfigFile::getStrValue(const char* sSection, const char* sKey, char* sValue, int nBufCnt/* = MAX_PATH */)
{
	if(::GetPrivateProfileString(sSection, sKey, "", sValue, nBufCnt, m_file_path) == 0)
		return false;
	else
		return true;
}

bool CConfigFile::getIntValue(const char* sSection, const char* sKey, int& nValue)
{
	char sBuffer[32] = {0};
	if(::GetPrivateProfileString(sSection, sKey, "", sBuffer, sizeof(sBuffer), m_file_path) == 0)
		return false;
	else
	{
		nValue = BaseFunc::strToInteger(sBuffer);
		return true;
	}
}

bool CConfigFile::setStrValue(const char* sSection, const char* sKey, const char* sValue)
{
	if(!::WritePrivateProfileString(sSection, sKey, sValue, m_file_path))
		return false;
	else
		return true;
}

bool CConfigFile::setIntValue(const char* sSection, const char* sKey, int sValue)
{
	char sTmpBuff[32];
	sprintf(sTmpBuff, "%d", sValue);

	return setStrValue(sSection, sKey, sTmpBuff);
}
