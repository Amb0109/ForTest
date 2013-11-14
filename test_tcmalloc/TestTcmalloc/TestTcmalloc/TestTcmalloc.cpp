// TestTcmalloc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "test_func.h"

int _tmain(int argc, _TCHAR* argv[])
{
	{
		LogTime _AUTO_TIMER("list_malloc_free(1000000, 1024, 102400)");
		list_malloc_free(1000000, 1024, 102400);
	}

	{
		LogTime _AUTO_TIMER("random_malloc_free(1000000, 1024, 102400)");
		random_malloc_free(1000000, 1024, 102400);
	}

	{
		LogTime _AUTO_TIMER("random_malloc_free_mthd(3, 1000000, 1024, 102400)");
		random_malloc_free_mthd(3, 1000000, 1024, 102400);
	}
	system("pause");
	return 0;
}

