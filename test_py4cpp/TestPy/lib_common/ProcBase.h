#ifndef _SUB_PROCESS_H_
#define _SUB_PROCESS_H_

#include "stdafx.h"
#include "PipeBase.h"

class CProcBase
{
	friend class CPipeObject;

public:
	CProcBase();
	~CProcBase();

	virtual bool	create(const char* sCmdLine, const char* sWorkPath = NULL);
	int				run(long nMilliSeconds = INF_TIME);
	int				getExitCode();
	bool			terminate();

	bool			setInputDevice(DeviceHandleT& hDevice);
	bool			setOutputDevice(DeviceHandleT& hDevice);
	bool			setErrorDevice(DeviceHandleT& hDevice);

	void			resetInputDevice();
	void			resetOutputDevice();
	void			resetErrorDevice();

	bool			setInputFile(const char* sFilePath);
	bool			setOutputFile(const char* sFilePath);
	bool			setErrorFile(const char* sFilePath);

	int				getRunTime();
	int				getRunMemory();

	ProcessHandleT		getProcessHandle();
	ProcessHandleT		getThreadHandle();

protected:
	STARTUPINFO				m_start_info;
	PROCESS_INFORMATION		m_proc_info;

	DeviceHandleT			m_org_std_input;
	DeviceHandleT			m_org_std_output;
	DeviceHandleT			m_org_std_error;

	bool					m_is_created;
	bool					m_is_input_redirect;
	bool					m_is_output_redirect;
	bool					m_is_error_redirect;
};

class CPipeProc
{
public:
	bool	initPipe();

	int		run(long nMilliSeconds = INF_TIME);
	int		send(const char* sBuff, int nBuffSize);
	int		recv(char* sBuff, int nBuffSize);

	bool	setInputPipe();
	bool	setOutputPipe();
	bool	setErrorPipe();

	CProcBase&	getProcess(){ return m_process; }

protected:
	CProcBase	m_process;

	CReadPipe	m_read_pipe;
	CWritePipe	m_write_pipe;
};


#endif // _SUB_PROCESS_H_