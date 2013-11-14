#ifndef _PYTHON_DEBUG_H_
#define _PYTHON_DEBUG_H_

#include "Python.h"

namespace py_engine
{
static PyObj* create_log_file(PyObj *self, PyObj *args);
static PyObj* print_log(PyObj *self, PyObj *args);


}

#endif //_PYTHON_DEBUG_H_
