// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <shlwapi.h>
#include <psapi.h>

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <direct.h>

#include <list>
#include <queue>
#include <map>

typedef __int64				LLIntT;
typedef unsigned __int64	ULLIntT;

typedef HANDLE				ProcessHandleT;
typedef HANDLE				ThreadHandleT;
typedef HANDLE				DeviceHandleT;
typedef HANDLE				FileHandleT;
typedef HANDLE				PipeHandleT;
typedef HANDLE				EventHandleT;
typedef HANDLE				JobHandleT;
typedef CRITICAL_SECTION	CSMutexT;
typedef HANDLE				IocpHandleT;
typedef SERVICE_STATUS_HANDLE	ServiceHandleT;

#define RETRYDELAY	(100)
#define INF_TIME	(-1)
#define ERR_SIZE	(-1)

#define SHOW_POS printf("*** %s:%d %s\n", __FILE__, __LINE__, __FUNCTION__)
#define for_std_unit(type, obj, it) for(type::iterator it=obj.begin(); it!=obj.end(); ++it)