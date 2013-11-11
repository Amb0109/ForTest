#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>



#include <queue>
#include <string>
#include <map>



typedef long		nSIZE;
typedef long		nTIME;

typedef HANDLE				hPROCESS;
typedef HANDLE				hTHREAD;
typedef HANDLE				hPIPE;
typedef HANDLE				hEVENT;
typedef HANDLE				hDEVICE;
typedef CRITICAL_SECTION	hMUTEX;
typedef OVERLAPPED			hIOTASK;

#define INFTIME (-1)
#define ERRSIZE (-1)