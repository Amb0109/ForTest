// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
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