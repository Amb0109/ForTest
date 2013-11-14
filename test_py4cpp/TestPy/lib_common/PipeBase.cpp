#include "stdafx.h"
#include "PipeBase.h"

CPipeObject::CPipeObject()
{
	m_read_port = NULL;
	m_write_port = NULL;
}

CPipeObject::~CPipeObject()
{
	closeReadPort();
	closeWritePort();
}

bool CPipeObject::init()
{
	SECURITY_ATTRIBUTES saAttr = {0};
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	BOOL b_ret = ::CreatePipe(
		&m_read_port,
		&m_write_port,
		&saAttr,
		NULL);

	if (b_ret) return true;
	else return false;
}

PipeHandleT& CPipeObject::getReadPort()
{
	return m_read_port;
}

PipeHandleT& CPipeObject::getWritePort()
{
	return m_write_port;
}

void CPipeObject::closeReadPort()
{
	if (m_read_port != NULL)
		::CloseHandle(m_read_port);
	m_read_port = NULL;
}

void CPipeObject::closeWritePort()
{
	if (m_write_port != NULL)
		::CloseHandle(m_write_port);
	m_write_port = NULL;
}

bool CReadPipe::init()
{
	if ( !m_pipe.init() )
		return false;
	
	if ( !::SetHandleInformation(m_pipe.getReadPort(), HANDLE_FLAG_INHERIT, 0) )
		return false;

	return true;
}

int CReadPipe::recv(char* sBuff, int nBuffSize)
{
	PipeHandleT& pipe_read_port = m_pipe.getReadPort();
	DWORD nRecvSize = 0;
	bool b_ret = !!::ReadFile(pipe_read_port, sBuff, nBuffSize, &nRecvSize, NULL);
	
	if (!b_ret)
		return ERR_SIZE;

	return nRecvSize;
}

bool CWritePipe::init()
{
	if ( !m_pipe.init() )
		return false;

	if ( !SetHandleInformation(m_pipe.getWritePort(), HANDLE_FLAG_INHERIT, 0) )
		return false;

	return true;
}


int CWritePipe::send(const char* sBuff, int nBuffSize)
{
	PipeHandleT& pipe_write_port = m_pipe.getWritePort();
	DWORD nSendSize = 0;
	BOOL b_ret = WriteFile(pipe_write_port, sBuff, nBuffSize, &nSendSize, NULL);

	if (!b_ret)
		return ERR_SIZE;

	return nSendSize;
}