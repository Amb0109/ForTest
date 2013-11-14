// TestTcmalloc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "test_func.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int ti = clock();
	//random_malloc_free(1000000, 1024, 102400);
	random_malloc_free_mthd(3, 1000000, 1024, 102400);
	printf("%d\n", clock() - ti);
	return 0;
}

