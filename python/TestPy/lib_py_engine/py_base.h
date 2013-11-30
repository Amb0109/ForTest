#ifndef _PYTHON_BASE_H_
#define _PYTHON_BASE_H_

#include "Python.h"

namespace py_engine
{
typedef PyObject PyObj;

int initialize();
int is_initialized();
void finalize();
void add_script_path(char* script_path);
int run_simple_string(char* script_str);


}

#endif //_PYTHON_BASE_H_
