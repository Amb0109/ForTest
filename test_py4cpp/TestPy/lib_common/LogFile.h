#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include "stdafx.h"
#include "SafeLock.h"

//LogÎÄ¼þÀà
class CLogFile
{
public:
	CLogFile(const char *sFilePath, const char* sMod = NULL);
	~CLogFile();

	virtual bool fWrite(const char* sFormat, ...);
	virtual bool Write(const char* sLogStr);

private:
	FILE*		m_log_file;
	CSafeLock	m_log_lock;
};

class CDailyLog
{
public:
	CDailyLog(const char *sLogDir, const char* sLogTag);
	~CDailyLog();

	virtual bool fWrite(const char* sFormat, ...);
	virtual bool Write(const char* sLogStr);

protected:
	bool	locateLogFile();

private:
	FILE*		m_log_file;
	CSafeLock	m_log_lock;

	SYSTEMTIME	m_log_date;

	std::string		m_log_dir;
	std::string		m_log_tag;
};

#endif // _LOG_FILE_H_
