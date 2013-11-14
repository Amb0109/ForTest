#include "stdafx.h"
#include "LogFile.h"

CLogFile::CLogFile(const char *sFilePath, const char* sMod/* = NULL */)
{
	if (sMod == NULL)
		m_log_file = fopen(sFilePath, "a+");
	else
		m_log_file = fopen(sFilePath, sMod);
}

CLogFile::~CLogFile()
{
	if (m_log_file != NULL)
		fclose(m_log_file);
}

bool CLogFile::Write(const char* sLogStr)
{
	if (m_log_file == NULL)
		return false;

	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);

	CAutoLock _LOCK(&m_log_lock);
	fprintf(
		m_log_file,
		"[%04d/%02d/%02d %02d:%02d:%02d] %s\n",
		SysTime.wYear,
		SysTime.wMonth,
		SysTime.wDay,
		SysTime.wHour,
		SysTime.wMinute,
		SysTime.wSecond,
		sLogStr);

	fflush(m_log_file);
#ifdef _DEBUG

	printf(
		"[%04d/%02d/%02d %02d:%02d:%02d] %s\n",
		SysTime.wYear,
		SysTime.wMonth,
		SysTime.wDay,
		SysTime.wHour,
		SysTime.wMinute,
		SysTime.wSecond,
		sLogStr);
#endif

	m_log_lock.unLock();
	return true;
}

bool CLogFile::fWrite(const char* sFormat, ...)
{
	char _log_buff[10240];
	va_list args;
	va_start(args, sFormat);
	vsprintf(_log_buff, sFormat, args);

	return Write(_log_buff);
}

CDailyLog::CDailyLog(const char *sLogDir, const char* sLogTag)
{
	if(sLogDir != NULL) m_log_dir = sLogDir;
	if(sLogDir != NULL) m_log_tag = sLogTag;

	m_log_file = NULL;
	locateLogFile();
}

CDailyLog::~CDailyLog()
{
	if (m_log_file != NULL)
		fclose(m_log_file);
}

bool CDailyLog::Write(const char* sLogStr)
{
	CAutoLock _LOCK(&m_log_lock);
	
	if (!locateLogFile())
		return false;

	fprintf(
		m_log_file,
		"[%02d:%02d:%02d] %s\n",
		m_log_date.wHour,
		m_log_date.wMinute,
		m_log_date.wSecond,
		sLogStr);

	fflush(m_log_file);
#ifdef _DEBUG

	printf(
		"[%02d:%02d:%02d] %s\n",
		m_log_date.wHour,
		m_log_date.wMinute,
		m_log_date.wSecond,
		sLogStr);
#endif
	return true;
}

bool CDailyLog::locateLogFile()
{
	CAutoLock _LOCK(&m_log_lock);

	SYSTEMTIME now_sys_time = m_log_date;
	GetLocalTime(&m_log_date);

	if (m_log_file == NULL)
		goto _CREATE_NEW_LOGFILE;

	if (m_log_date.wDay != now_sys_time.wDay
		|| m_log_date.wMonth != now_sys_time.wMonth
		|| m_log_date.wYear != now_sys_time.wYear)
	{
		goto _CREATE_NEW_LOGFILE;
	}
	else
	{
		return true;
	}

_CREATE_NEW_LOGFILE:
	char s_log_file[MAX_PATH];
	sprintf(s_log_file, "%s%s_%d_%d_%d.log",
		m_log_dir.c_str(),
		m_log_tag.c_str(),
		m_log_date.wYear,
		m_log_date.wMonth,
		m_log_date.wDay);

	m_log_file = fopen(s_log_file, "a+");

	if (m_log_file != NULL)
		return true;
	else
		return false;
}

bool CDailyLog::fWrite(const char* sFormat, ...)
{
	char _log_buff[10240];
	va_list args;
	va_start(args, sFormat);
	vsprintf(_log_buff, sFormat, args);

	return Write(_log_buff);
}
