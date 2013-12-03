// TestNoBreakPad.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CrashFunc.h"

#ifdef _USE_GOOGLE_BREAKPAD
#include "client/windows/handler/exception_handler.h"
//#include "client/windows/sender/crash_report_sender.h"
#else
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")
#endif

#ifndef _USE_GOOGLE_BREAKPAD
//dump文件函数
LONG WINAPI dumpfile_func(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	time_t now_time = time(NULL);
	tm* p_now_time	= localtime(&now_time);
	if(p_now_time) 
	{
		//以时间来名命dump文件
		char file_name[1024]= "";
		sprintf(file_name, "dump/%04d%02d%02d%02d%02d%02d.dmp", p_now_time->tm_year+1900, p_now_time->tm_mon+1, p_now_time->tm_mday,p_now_time->tm_hour,p_now_time->tm_min,p_now_time->tm_sec);

		HANDLE lhDumpFile = CreateFile((LPCTSTR)file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
		MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
		loExceptionInfo.ExceptionPointers = ExceptionInfo;
		loExceptionInfo.ThreadId = GetCurrentThreadId();
		loExceptionInfo.ClientPointers = TRUE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);

		CloseHandle(lhDumpFile);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	::CreateDirectory("dump/", NULL);
#ifdef _USE_GOOGLE_BREAKPAD
	std::wstring dump_path(L"dump/");
	google_breakpad::ExceptionHandler *pCrashHandler =
		new google_breakpad::ExceptionHandler(dump_path,
		NULL,
		NULL,
		NULL,
		google_breakpad::ExceptionHandler::HANDLER_ALL,
		MiniDumpNormal,
		(const wchar_t*)NULL,
		NULL);
#else
	__try {
#endif

	// proc logic
	crash_wild_point2();

#ifndef _USE_GOOGLE_BREAKPAD
	}
	__except (dumpfile_func(GetExceptionInformation()))
	{

	}
#endif

	return 0;
}

