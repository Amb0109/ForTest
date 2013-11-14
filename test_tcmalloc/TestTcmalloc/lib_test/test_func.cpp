#include "test_func.h"

void random_malloc_free(int test_cnt, int test_cap, int max_mem)
{
	typedef std::map<int, char*> POINT_MAP;
	typedef POINT_MAP::iterator POINT_MAP_ITOR;
	POINT_MAP test_map;
	for(int i=0; i<test_cnt; ++i)
	{
		int rand_id = rand() % test_cap;
		POINT_MAP_ITOR find_part = test_map.find(rand_id);
		if (find_part != test_map.end())
		{
			//printf("free %d\n", rand_id);
			delete[] (find_part->second);
			test_map.erase(find_part);
		}
		else
		{
			int len = rand() * rand() % max_mem;
			//printf("malloc %d:%d\n", rand_id, len);
			char* block = new char[len];
			test_map[rand_id] = block;
		}
	}

	for(POINT_MAP_ITOR it = test_map.begin(); it != test_map.end(); ++it)
	{
		delete[] it->second;
	}
}

DWORD WINAPI RAMDON_MALLOC_FREE_THD(LPVOID p_param)
{
	RANDOM_MALLOC_FREE_ARGS* test_args = (RANDOM_MALLOC_FREE_ARGS*)p_param;
	if (test_args == NULL) return 1;
	random_malloc_free(test_args->test_cnt, test_args->test_cap, test_args->max_mem);
	return 0;
}

void random_malloc_free_mthd(int thd_cnt, int test_cnt, int test_cap, int max_mem)
{
	RANDOM_MALLOC_FREE_ARGS test_args;
	test_args.test_cnt = test_cnt;
	test_args.test_cap = test_cap;
	test_args.max_mem = max_mem;

	HANDLE* thd_lst = new HANDLE[thd_cnt];
	for (int i=0; i<thd_cnt; ++i)
	{
		thd_lst[i] = ::CreateThread(NULL, 0, RAMDON_MALLOC_FREE_THD, 
							(LPVOID)(&test_args), CREATE_SUSPENDED, NULL);
	}

	for (int i=0; i<thd_cnt; ++i)
		::ResumeThread(thd_lst[i]);

	::WaitForMultipleObjects(thd_cnt, thd_lst, TRUE, INFINITE);
	for (int i=0; i<thd_cnt; ++i)
		::CloseHandle(thd_lst[i]);

	delete[] thd_lst;
}
