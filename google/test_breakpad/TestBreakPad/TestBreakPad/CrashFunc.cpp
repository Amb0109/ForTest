#include "stdafx.h"
#include "CrashFunc.h"

int crash_div_zero()
{
	int y = 3;
	int x = 10;

	for (int i=0; i<5; ++i)
	{
		y--;
		printf("%d %d %d\n", x, y, x/y);
	}
	return x;
}

int crash_segment_fault_stack()
{ // no crash, ̫Σ����
	int a[10] = {0};
	int b[10] = {0};

	for (int i=0; i<64; ++i)
	{
		printf("%d b = %d\n", i, b[i]);
	}

	for (int i=0; i<64; ++i)
	{
		a[i] = i;
		printf("%d a = %d\n", i, a[i]);
	}

	return 0;
}

int crash_segment_fault_stack2()
{ // ̫��ͱ���
	int a[10] = {0};
	int b[10] = {0};

	for (int i=0; i<640000; ++i)
	{
		printf("%d b = %d\n", i, b[i]);
	}

	for (int i=0; i<640000; ++i)
	{
		a[i] = i;
		printf("%d a = %d\n", i, a[i]);
	}

	return 0;
}

int crash_segment_fault_stack3()
{ // crash
	char str[10];
	std::string str_s(26, 'a');
	memcpy(str, str_s.c_str(), str_s.length() + 1);
	puts(str);
	return 0;
}

int crash_segment_fault_heap()
{ //��crash_segment_fault_stack��ͬ
	int* a = new int(10);
	int* b = new int(10);

	for (int i=0; i<64; ++i)
	{
		printf("%d b = %d\n", i, b[i]);
	}

	for (int i=0; i<64; ++i)
	{
		a[i] = i;
		printf("%d a = %d\n", i, a[i]);
	}

	return 0;
}

int crash_segment_fault_heap2()
{//��crash_segment_fault_stack2��ͬ������
	int* a = new int(10);
	int* b = new int(10);

	for (int i=0; i<640000; ++i)
	{
		printf("%d b = %d\n", i, b[i]);
	}

	for (int i=0; i<640000; ++i)
	{
		a[i] = i;
		printf("%d a = %d\n", i, a[i]);
	}

	return 0;
}

int crash_segment_fault_heap3()
{ // no crash
	char* str = new char(10);
	std::string str_s(26, 'a');
	memcpy(str, str_s.c_str(), str_s.length() + 1);
	puts(str);
	return 0;
}

int crash_segment_fault_heap4()
{ // crash
	char* str = new char(10);
	std::string str_s(10000, 'a');
	memcpy(str, str_s.c_str(), str_s.length() + 1);
	puts(str);
	return 0;
}

int crash_stl_segment_fault()
{ // crash
	std::vector<int> a;
	a.assign(10, 0);

	for (int i=0; i<15; ++i)
	{
		a[i] = i;
		printf("%d\n", a[i]);
	}
	return 0;
}

int crash_stack_overflow(int i)
{ // ջ����
	int a[10000] = {0};
	int b[10000] = {0};
	for (int j=0; j<10; ++j)
	{
		a[j] = i;
		printf("%d?", a[j]);
	}
	printf("%d\n", i);
	crash_stack_overflow(i+1);
	for (int j=0; j<10; ++j)
	{
		b[9999-j] = i;
		printf("%d?", b[9999-j]);
	}
	printf("%d\n", i);
	return 0;
}

int crash_null_point()
{ // crash
	for (int i=0; i<100; ++i)
	{
		int* p_a = (int*)malloc(sizeof(int));
		(*p_a) = 10;
		printf("%d\n", *p_a);
		free(p_a);
		p_a = NULL;
		(*p_a) = 20;
		printf("%d\n", *p_a);
	}
	return 0;
}

int crash_wild_point()
{ // no crash Ұָ��Ҫ�ȿ�ָ��Σ�յĶ�
	for (int i=0; i<100; ++i)
	{
		int* p_a = (int*)malloc(sizeof(int));
		(*p_a) = 10;
		printf("%d\n", *p_a);
		free(p_a);
		(*p_a) = 20;
		printf("%d\n", *p_a);
	}
	return 0;
}

int crash_wild_point2()
{ // crash �����Ǳ���
	for (int i=0; i<100; ++i)
	{
		c_test* p_a = new c_test();
		p_a->func(1, 1);
		printf("%d %d\n", p_a->a, p_a->b);
		free(p_a);
		p_a->func(1, 1);
		printf("%d %d\n", p_a->a, p_a->b);
	}
	return 0;
}

int crash_pure_call()
{
	c_test* p_test = new c_test();
	p_test->func_call();
	delete p_test;
	return 0;
}
