#ifndef _SAND_BOX_H_
#define _SAND_BOX_H_

#include "stdafx.h"
#include "ProcBase.h"


class CSandBox
{
public:
	enum {
		PROC_STATUS_ERROR_STATUS = -1,

		PROC_STATUS_PROC_EXIT = 0,
		PROC_STATUS_PROC_RUNNING,
		PROC_STATUS_TIME_LIMITED,
		PROC_STATUS_MEMORY_LIMITED,
		PROC_STATUS_RUNTIME_ERROR
	};

	//enum {
	//	RE_STATUS_ACCESS_VIOLATION,
	//	RE_STATUS_ARRAY_BOUNDS_EXCEEDED,
	//	RE_STATUS_BREAKPOINT,
	//	RE_STATUS_CONTROL_C_EXIT,
	//	RE_STATUS_DATATYPE_MISALIGNMENT,
	//	RE_STATUS_FLOAT_DENORMAL_OPERAND,
	//	RE_STATUS_FLOAT_DIVIDE_BY_ZERO,
	//	RE_STATUS_FLOAT_INEXACT_RESULT,
	//	RE_STATUS_FLOAT_INVALID_OPERATION,
	//	RE_STATUS_FLOAT_MULTIPLE_FAULTS,
	//	RE_STATUS_FLOAT_MULTIPLE_TRAPS,
	//	RE_STATUS_FLOAT_OVERFLOW,
	//	RE_STATUS_FLOAT_STACK_CHECK,
	//	RE_STATUS_FLOAT_UNDERFLOW,
	//	RE_STATUS_GUARD_PAGE_VIOLATION,
	//	RE_STATUS_ILLEGAL_INSTRUCTION,
	//	RE_STATUS_IN_PAGE_ERROR,
	//	RE_STATUS_INVALID_DISPOSITION,
	//	RE_STATUS_INTEGER_DIVIDE_BY_ZERO,
	//	RE_STATUS_INTEGER_OVERFLOW,
	//	RE_STATUS_NONCONTINUABLE_EXCEPTION,
	//	RE_STATUS_PRIVILEGED_INSTRUCTION,
	//	RE_STATUS_REG_NAT_CONSUMPTION,
	//	RE_STATUS_SINGLE_STEP,
	//	RE_STATUS_STACK_OVERFLOW
	//};

public:
	CSandBox();
	~CSandBox();

public:
	bool			init();
	virtual bool	setLimit(int tiLimit, int memLimit);
	bool			assignProcess(CProcBase& hProc);
	int				getStatus(int nMilliSeconds = INF_TIME);
	int				getExitCode();
	bool			close();

protected:
	bool			setBaseLimit(int tiLimit, int memLimit);
	bool			setBaseUILimit();
	bool			setCallBack();

	static long		getIocpKey();

private:
	JobHandleT					m_job_obj;
	IocpHandleT					m_job_iocp;

	CProcBase*				m_process;

	long					m_job_iocp_key;
};


#endif // _SAND_BOX_H_