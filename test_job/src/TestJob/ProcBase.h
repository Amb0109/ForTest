#ifndef _SUB_PROCESS_H_
#define _SUB_PROCESS_H_

#include "stdafx.h"

class CProcBase
{
public:
	CProcBase();
	~CProcBase();

	bool			Create(const char* sCmdLine);
	bool			CreateAsUser(HANDLE hToken, const char* sCmdLine);
	int				Run(long nMilliSeconds = INFTIME);
	unsigned		GetExitCode();
	bool			Terminate();

	bool	SetInputFile(const char* sFilePath = NULL);
	bool	SetOutputFile(const char* sFilePath = NULL);
	bool	SetErrorFile(const char* sFilePath = NULL);

	PROCESS_INFORMATION& getProcInfo() {return m_proc_info;}

protected:
	STARTUPINFO				m_start_info;
	PROCESS_INFORMATION		m_proc_info;

	bool	m_use_input_file;
	bool	m_use_output_file;
	bool	m_use_error_file;
};

#endif // _SUB_PROCESS_H_