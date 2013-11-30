#include <Windows.h>
#include "Common.h"
#include "Player.h"

CPlayer::CPlayer()
{
	g_hChildStd_IN_Rd = NULL;
	g_hChildStd_IN_Wr = NULL;
	g_hChildStd_OUT_Rd = NULL;
	g_hChildStd_OUT_Wr = NULL;
}

CPlayer::~CPlayer()
{
	killSubProcess();
}

bool CPlayer::initPlayer()
{
	SECURITY_ATTRIBUTES saAttr;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0) ) 
		return false;

	if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
		return false;

	if (! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) 
		return false;

	if ( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
		return false;

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	return true;
}

bool CPlayer::runSubProcess(const char* cmdLine)
{
	if (g_hChildStd_OUT_Wr == NULL) return false;
	if (g_hChildStd_IN_Rd == NULL) return false;

	char szCmdLine[1024];
	strcpy(szCmdLine, cmdLine);

	BOOL bRet = CreateProcess(
		NULL,
		szCmdLine,
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&siStartInfo,
		&piProcInfo);

	return bRet;
}

size_t CPlayer::Send(const char* buff)
{
	DWORD nWritten = 0;
	char tmpBuff[1024];
	memset(tmpBuff, 0, sizeof(tmpBuff));
	strcpy(tmpBuff, buff);
	strcat(tmpBuff, "\n");
	size_t size = strlen(tmpBuff);
	WriteFile(g_hChildStd_IN_Wr, tmpBuff, (DWORD)size, &nWritten, NULL);

	return nWritten;
}

size_t CPlayer::Recv(char* buff)
{
	DWORD nRead = 0;
	char tmpBuff[1024];
	memset(tmpBuff, 0, sizeof(tmpBuff));
	ReadFile(g_hChildStd_OUT_Rd, tmpBuff, 1024, &nRead, NULL);
	strcpy(buff, tmpBuff);

	return sizeof(buff);
}

DWORD CPlayer::getPlayerTime()
{
	return GetProcessTime(piProcInfo.hProcess);
}

DWORD CPlayer::getPlayerMemory()
{
	return GetProcessMemory(piProcInfo.hProcess);
}

bool CPlayer::killSubProcess()
{
	TerminateProcess(piProcInfo.hProcess, 0);
	return true;
}