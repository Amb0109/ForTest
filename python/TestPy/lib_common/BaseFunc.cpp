#include "stdafx.h"
#include "BaseFunc.h"


bool BaseFunc::isFileExists(const char* sPath)
{
	return (_access(sPath, 0)) != -1;
}

void BaseFunc::getFullPath(char* sFullPath, const char* sFilePath)
{
	_fullpath(sFullPath, sFilePath, MAX_PATH);
}

void BaseFunc::setCurrentDirectory(const char* sDirPath)
{
	::SetCurrentDirectory(sDirPath);
}

void BaseFunc::getCurrentDirectory(char* sDirPath)
{
	char dir_path[MAX_PATH];
	::GetModuleFileName(NULL, sDirPath, MAX_PATH);
	getSafePath(dir_path, sDirPath);
	getParentPath(sDirPath, dir_path);
}

void BaseFunc::getModuleFilePath(char* sFilePath)
{
	char file_path[MAX_PATH];
	::GetModuleFileName(NULL, file_path, MAX_PATH);
	getSafePath(sFilePath, file_path);
}

void BaseFunc::getSafePath(char* sDstPath, const char* sSrcPath)
{
	for (int i=0; sSrcPath[i]; ++i)
	{
		if (sSrcPath[i] == '\\')
			sDstPath[i] = '/';
		else
			sDstPath[i] = sSrcPath[i];
		sDstPath[i+1] = '\0';
	}
}

bool BaseFunc::delDirectory(const char* sDirPath)
{
	bool b_ret = true;
	b_ret = b_ret && clearDirectory(sDirPath);
	b_ret = b_ret && ::RemoveDirectory(sDirPath);
	return b_ret;
}

bool BaseFunc::clearDirectory(const char* sDirPath)
{
	bool b_ret = true;

	char dir_path[MAX_PATH];
	char* ptr_fn = NULL;

	WIN32_FIND_DATA find_data;
	HANDLE h_find;

	strcpy(dir_path, sDirPath);
	ptr_fn = dir_path + strlen(dir_path);
	strcpy(ptr_fn, "*.*");
	
	h_find = ::FindFirstFile(dir_path, &find_data);
	while (INVALID_HANDLE_VALUE != h_find)
	{
		if (strcmp(find_data.cFileName, ".") == 0
			|| strcmp(find_data.cFileName, "..") == 0)
				goto find_end;

		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			strcpy(ptr_fn, find_data.cFileName);
			::PathAddBackslash(dir_path);
			delDirectory(dir_path);
		} else if (find_data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
			strcpy(ptr_fn, find_data.cFileName);
			::DeleteFile(dir_path);
		}
		
find_end:
		if( !::FindNextFile(h_find, &find_data) )
			break;
	}
	::FindClose(h_find);

	return b_ret;
}

//bool BaseFunc::copyDirectory(char* sDstDir, const char* sSrcDir)
//{
//	bool b_ret = true;
//	return b_ret;
//}

bool BaseFunc::delFile(const char* sFileName)
{
	bool b_ret = false;
	if (::DeleteFile(sFileName))
		b_ret = true;
	return b_ret;
}

bool BaseFunc::copyFile(const char* sDstFile, const char* sSrcFile)
{
	bool b_ret = true;
	while (true)
	{
		b_ret = b_ret && ::CopyFile(sSrcFile, sDstFile, FALSE);
		if (!b_ret
			&& (::GetLastError() == ERROR_SHARING_VIOLATION
			|| ::GetLastError() == ERROR_ACCESS_DENIED))
		{
			b_ret = true;
			::Sleep(RETRYDELAY);
			continue;
		}
		break;
	}
	return b_ret;
}

int BaseFunc::getProcessTime(const ProcessHandleT hProcess)
{
	unsigned long dwTimeUsage = 0;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;

	if(GetProcessTimes(hProcess, &CreationTime, &ExitTime, &KernelTime, &UserTime))
	{
		dwTimeUsage = (KernelTime.dwHighDateTime + UserTime.dwHighDateTime) * 1000 + 
			(KernelTime.dwLowDateTime + UserTime.dwLowDateTime) / 10000;
	}
	return dwTimeUsage;
}

int BaseFunc::getProcessMemory(const ProcessHandleT hProcess)
{
	int dwMemory = 0;
	PROCESS_MEMORY_COUNTERS pmc;

	ZeroMemory(&pmc, sizeof(pmc));
	if(GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		dwMemory = (DWORD)pmc.PeakWorkingSetSize / 1024;
	return dwMemory;
}


int BaseFunc::hexCharToDec( char HexChar )
{
	if (HexChar >= 'A' && HexChar <= 'F')
		return int(HexChar - 'A' + 10);
	else if (HexChar >= 'a' && HexChar <= 'f')
		return int(HexChar - 'a' + 10);
	else if(HexChar >= '0' && HexChar <= '9')
		return int(HexChar - '0');
	else return -1;
}

void BaseFunc::charToHexCode( char* sDst, unsigned char srcChar )
{
	unsigned char tmpChar = '\0';
	tmpChar = srcChar % 16;
	if (tmpChar < 10) sDst[1] = tmpChar + '0';
	else sDst[1] = tmpChar - 10 + 'A';
	tmpChar = srcChar / 16;
	if (tmpChar < 10) sDst[0] = tmpChar + '0';
	else sDst[0] = tmpChar - 10 + 'A';
}


//int BaseFunc::decAsciiStr(char* sDst, const char* sSrc, int bufSize)
//{
//	int write_size = 0;
//	for(int i=0; sSrc[i];)
//	{
//		if(write_size >= bufSize) return bufSize;
//
//		if (sSrc[i] == '%')
//		{
//			char tsrc;
//			tsrc = sSrc[i+1];
//			short hret = hexCharToDec(tsrc);
//			if(hret < 0) goto simple_char;
//			tsrc = sSrc[i+2];
//			short lret = hexCharToDec(tsrc);
//			if(lret < 0) goto simple_char;
//
//			sDst[write_size] = char(hret*16+lret);
//			i+=3;
//			write_size+=1;
//			continue;
//
//		}
//
//simple_char:
//		sDst[write_size] = sSrc[i];
//		i+=1;
//		write_size+=1;
//	}
//	return true;
//}
//
//int BaseFunc::decAsciiStrFile(const char* fDstFile, const char* sSrc)
//{
//	FILE* p_file = fopen(fDstFile, "wb");
//	if (p_file == NULL) return 0;
//
//	int write_size = 0;
//	for(int i=0; sSrc[i];)
//	{
//		if (sSrc[i] == '%')
//		{
//			char tsrc;
//			tsrc = sSrc[i+1];
//			short hret = hexCharToDec(tsrc);
//			if(hret < 0) goto simple_char;
//			tsrc = sSrc[i+2];
//			short lret = hexCharToDec(tsrc);
//			if(lret < 0) goto simple_char;
//
//			fputc(char(hret*16+lret), p_file);
//			i+=3;
//			write_size++;
//			continue;
//
//		}
//
//simple_char:
//		fputc(sSrc[i], p_file);
//		i+=1;
//		write_size++;
//	}
//	fclose(p_file);
//	return write_size;
//}

//void BaseFunc::EncAsciiStr(char* sDst, const char* sSrc, int bufSize)
//{
//	;
//}


bool BaseFunc::MD5File(const char* sFileName, char* sMD5Digest)
{
	unsigned char btDigest[16];
	bool b_ret = !!MDFile(const_cast<char*>(sFileName), reinterpret_cast<char*>(btDigest));
	for (int i = 0; i < 16; i++)
		charToHexCode(sMD5Digest + i * 2, btDigest[i]);
	sMD5Digest[32] = '\0';

	return b_ret;
}

void BaseFunc::MD5String(const char* sStrBuff, char* sMD5Digest)
{
	unsigned char btDigest[16];
	MDString(const_cast<char*>(sStrBuff), reinterpret_cast<char*>(btDigest));
	for (int i = 0; i < 16; i++)
		charToHexCode(sMD5Digest + i * 2, btDigest[i]);
	sMD5Digest[32] = '\0';
}

void BaseFunc::MD5Data(const void* sBuff, int nBuffLen, char* sMD5Digest)
{
	unsigned char btDigest[16]; 
	MDData(reinterpret_cast<char*>(const_cast<void*>(sBuff)),
		nBuffLen, reinterpret_cast<char*>(btDigest));
	for (int i = 0; i < 16; i++)
		charToHexCode(sMD5Digest + i * 2, btDigest[i]);
	sMD5Digest[32] = '\0';
}

void BaseFunc::sleep(long nMilliSeconds /*= 0*/)
{
	::Sleep(nMilliSeconds);
}


int BaseFunc::getSysError()
{
	return ::GetLastError();
}

bool BaseFunc::getParentPath(char* sDestPath, const char* sPath)
{
	if (sDestPath == NULL)
		return false;

	strcpy(sDestPath, sPath);
	
	if (sDestPath[0] == '\0')
		return true;

	size_t len = strlen(sDestPath);
	
	if (sDestPath[len-1] == '/')
		sDestPath[len-1] = '\0';

	char* end_pos = strrchr(sDestPath, '/');
	if(end_pos)
	{
		end_pos[0] = '\0';
		return true;
	}
	else
	{
		sDestPath[0] = '\0';
		return false;
	}
}

bool BaseFunc::newDirectory(const char* sDirPath)
{
	if(sDirPath == NULL || sDirPath[0] == 0)
		return false;

	if( !::CreateDirectory(sDirPath, NULL) )
	{
		if ( ERROR_ALREADY_EXISTS == ::GetLastError() )
			return true;

		char s_tmp_path[MAX_PATH];
		getParentPath(s_tmp_path, sDirPath);
		if( !newDirectory(s_tmp_path) )
			return false;
		return !!::CreateDirectory(sDirPath, NULL);
	}
	return true;
}

void BaseFunc::addBackSlash(std::string& sDstStr, const std::string& sSrcStr)
{
	sDstStr.clear();

	size_t len = sSrcStr.length();
	for (size_t i=0; i<len; i++)
	{
		switch (sSrcStr[i])
		{
		case '\0':
		case '\'':
		case '\"':
		case '\b':
		case '\n':
		case '\r':
		case '\t':
		case '\\':
			sDstStr.insert(sDstStr.end(), '\\');
			break;
		}

		sDstStr.insert(sDstStr.end(), sSrcStr[i]);
	}
}

int BaseFunc::strToInteger( const char* sIntString )
{
	int b_ret = 0;
	bool is_negative = false;
	
	int _pos = 0;
	do
	{
		if (sIntString[_pos] == '-') {
			++_pos;
			is_negative = false;
			break;
		} else if(isdigit(sIntString[_pos])) {
			break;
		}
	} while(++_pos);

	while(isdigit(sIntString[_pos])){
		b_ret *= 10;
		b_ret += sIntString[_pos]-'0';
		_pos++;
	}
	if(is_negative) b_ret = -b_ret;
	return b_ret;
}

int BaseFunc::getFileContent(std::string& sFileContent, const char* sFilePath)
{
	FILE* p_file = fopen(sFilePath, "rb");
	if (p_file == NULL) return -1;

	sFileContent.clear();
	char _buff[1024];
	size_t _len = 0;
	size_t _total_len = 0;
	while (_len = fread(_buff, 1, 1024, p_file) )
	{
		_total_len += _len;
		sFileContent.append(_buff, _len);
	}

	fclose(p_file);
	return (int)_total_len;
}

void BaseFunc::strFormat(std::string& sResult, const char* sStrFmt, ...)
{
	int _size = 512;
	sResult.clear();
	va_list ap;
	while (true) {
		sResult.resize(_size);
		va_start(ap, sStrFmt);
		int _len = vsnprintf(
			const_cast<char*>(sResult.c_str()),
			_size, sStrFmt, ap);
		va_end(ap);
		if (_len >= 0 && _len < _size) {
			sResult.resize(_len);
			return;
		}
		if (_len >= 0)
			_size = _len + 1;
		else
			_size *= 2;
	}
}

void BaseFunc::ConvertGBKToUtf8(std::string& sUTF8Str, const std::string& sGBKStr)
{
	sUTF8Str.clear();

	int _len = ::MultiByteToWideChar(CP_ACP, 0, sGBKStr.c_str(), -1, NULL, 0);
	wchar_t* _tmp_buff = new wchar_t[_len*2+2];
	::MultiByteToWideChar(CP_ACP, 0, sGBKStr.c_str(), -1, _tmp_buff, _len);
	
	_len = ::WideCharToMultiByte(CP_UTF8, 0, _tmp_buff, -1, NULL, 0, NULL, NULL); 
	sUTF8Str.resize(_len+1);
	::WideCharToMultiByte (CP_UTF8, 0, _tmp_buff, -1,
		const_cast<LPSTR>(sUTF8Str.c_str()), _len, NULL, NULL);
	delete[] _tmp_buff;
}

void BaseFunc::ConvertUtf8ToGBK(std::string& strUtf8) {
	//int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL,0);
	//char* wszGBK = new char[len*2+2];
	//memset(wszGBK, 0, len * 2 + 2);
	//MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);

	//len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	//char *szGBK=new char[len + 1];
	//memset(szGBK, 0, len + 1);
	//WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL,NULL);

	//strUtf8 = szGBK;
	//delete[] szGBK;
	//delete[] wszGBK;
}