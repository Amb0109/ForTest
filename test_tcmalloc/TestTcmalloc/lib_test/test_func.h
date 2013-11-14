#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

#include "stdafx.h"

typedef struct _RANDOM_MALLOC_FREE_ARGS
{
	int test_cnt;
	int test_cap;
	int max_mem;
} RANDOM_MALLOC_FREE_ARGS;

void random_malloc_free(int test_cnt, int test_cap, int max_mem);
void random_malloc_free_mthd(int thd_cnt, int test_cnt, int test_cap, int max_mem);



#endif // _TEST_COMMON_H_