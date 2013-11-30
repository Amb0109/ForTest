// TestPy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "py_base.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (!py_engine::initialize())
		return 0;


	py_engine::add_script_path("script");
	py_engine::run_simple_string("import test");
	//py_engine::run_string("test.output()");

	py_engine::finalize();
	return 0;
}

