#ifndef _BASE_FUNCTION_H_
#define _BASE_FUNCTION_H_

#include "stdafx.h"
#include "./libs/md5.h"

class BaseFunc
{
public:
	static bool		isFileExists(const char* sPath);
	static void		getFullPath(char* sFullPath, const char* sFilePath);
	static bool		getParentPath(char* sDestPath, const char* sPath);
	static void		getModuleFilePath(char* sFilePath);
	static void		setCurrentDirectory(const char* sDirPath);
	static void		getCurrentDirectory(char* sDirPath);
	static void		getSafePath(char* sDstPath, const char* sSrcPath);

	static bool		newDirectory(const char* sDirPath);
	static bool		delDirectory(const char* sDirPath);
//	static bool		copyDirectory(char* sDstDir, const char* sSrcDir);
	static bool		clearDirectory(const char* sDirPath);
	static bool		delFile(const char* sFileName);
	static bool		copyFile(const char* sDstFile, const char* sSrcFile);

	static int		getFileContent(std::string& sFileContent, const char* sFilePath);

public:
	static int		getSysError();
	static void		sleep(long nMilliSeconds = 0);
	static int		getProcessTime(const ProcessHandleT hProcess);
	static int		getProcessMemory(const ProcessHandleT hProcess);

public:
	/// 'A' -> 10
	static int		hexCharToDec(char HexChar);
	/// '/' -> "2F"
	static void		charToHexCode(char* sDst, unsigned char srcChar);

	static int		strToInteger(const char* sIntString);

	static void		addBackSlash(std::string& sDstStr, const std::string& sSrcStr);

	static void		strFormat(std::string& sResult, const char* sStrFmt, ...);

//	/// "D%3A%2FProjects%2F" - > "D:/Projects/"
//	static int		decAsciiStr(char* sDst, const char* sSrc, int bufSize);
//	static int		decAsciiStrFile(const char* fDstFile, const char* sSrc);
//	/// "D:/Projects/" -> "D%3A%2FProjects%2F" 
//	static bool		EncAsciiStr(char* sDst, const char* sSrc, int bufSize);

public:
	static bool		MD5File(const char* sFileName, char* sMD5Digest);
	static void		MD5String(const char* sStrBuff, char* sMD5Digest);
	static void		MD5Data(const void* sBuff, int nBuffLen, char* sMD5Digest);

public:
	static void		ConvertGBKToUtf8(std::string& sUTF8Str, const std::string& sGBKStr);
	static void		ConvertUtf8ToGBK(std::string& strUtf8);
};

#endif // _BASE_FUNCTION_H_