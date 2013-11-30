#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

#include "stdafx.h"

typedef struct _RANDOM_MALLOC_FREE_ARGS
{
	int test_cnt;
	int test_cap;
	int max_mem;
} RANDOM_MALLOC_FREE_ARGS;

void list_malloc_free(int test_cnt, int test_cap, int max_mem);
void random_malloc_free(int test_cnt, int test_cap, int max_mem);
void random_malloc_free_mthd(int thd_cnt, int test_cnt, int test_cap, int max_mem);


class LogTime
{
public:
	LogTime(char* log_msg):start_ti_(clock()), log_msg_(log_msg) {}
	~LogTime() {printf("%s %d\n", log_msg_.c_str(), clock() - start_ti_);}

private:
	clock_t start_ti_;
	std::string log_msg_;
};


#endif // _TEST_COMMON_H_