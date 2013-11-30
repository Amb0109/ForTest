#ifndef _PIPE_BASE_H_
#define _PIPE_BASE_H_

#include "stdafx.h"

class CReadPipe;
class CWritePipe;
class CPipeProc;

class CPipeObject
{
	friend class CReadPipe;
	friend class CWritePipe;
	
	friend class CPipeProc;

public:
	CPipeObject();
	~CPipeObject();

protected:
	virtual bool	init();

	PipeHandleT&			getReadPort();
	PipeHandleT&			getWritePort();

	void			closeReadPort();
	void			closeWritePort();

private:
	PipeHandleT			m_read_port;
	PipeHandleT			m_write_port;
};

class CReadPipe
{
public:
	CReadPipe() {}
	~CReadPipe() {}

public:
	virtual bool	init();
	virtual int		recv(char* sBuff, int nBuffSize);
	CPipeObject&	getPipe() {return m_pipe;}

private:
	CPipeObject		m_pipe;
};

class CWritePipe
{
public:
	CWritePipe() {}
	~CWritePipe() {}

public:
	virtual bool	init();
	virtual int		send(const char* sBuff, int nBuffSize);
	CPipeObject&	getPipe() {return m_pipe;}

private:
	CPipeObject		m_pipe;
};

#endif // _PIPE_BASE_H_