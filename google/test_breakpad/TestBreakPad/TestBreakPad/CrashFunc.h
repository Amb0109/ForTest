#ifndef _CRASH_FUNC_H_
#define _CRASH_FUNC_H_

class c_test_base
{
public:
	c_test_base(){}
	virtual void func_call() {func(0, 0);}
	virtual void func(int a, int b) = 0;
};

class c_test : public c_test_base
{
public:
	c_test(){func(0, 0);}
	virtual void func_call() {c_test::func_call();}
	virtual void func(int a, int b) {
		this->a = a;
		this->b = b;
		printf("%d,%d %d,%d\n", a, this->a, b, this->b);
	}

	int a, b;
};

int crash_div_zero();
int crash_segment_fault_stack();
int crash_segment_fault_stack2();
int crash_segment_fault_stack3();
int crash_segment_fault_heap();
int crash_segment_fault_heap2();
int crash_segment_fault_heap3();
int crash_segment_fault_heap4();
int crash_stl_segment_fault();
int crash_stack_overflow(int i);
int crash_null_point();
int crash_wild_point();
int crash_wild_point2();
int crash_pure_call();

#endif //_CRASH_FUNC_H_