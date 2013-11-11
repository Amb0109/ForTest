#ifndef _OTHELLO_PLAYER_H_
#define _OTHELLO_PLAYER_H_


class CPlayer
{
private:
	HANDLE g_hChildStd_IN_Rd;
	HANDLE g_hChildStd_IN_Wr;
	HANDLE g_hChildStd_OUT_Rd;
	HANDLE g_hChildStd_OUT_Wr;

	STARTUPINFO siStartInfo;
	PROCESS_INFORMATION piProcInfo;

public:
	CPlayer();
	~CPlayer();

	bool initPlayer();
	bool runSubProcess(const char* cmd);

	size_t Send(const char* buff);
	size_t Recv(char* buff);

	DWORD getPlayerTime();
	DWORD getPlayerMemory();

	bool killSubProcess();
};




#endif //_OTHELLO_PLAYER_H_