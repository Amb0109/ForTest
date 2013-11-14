
#include "py_debug.h"



static PyObject* create_log_file(PyObject *self, PyObject *args)
{

}

static PyObject* print_log(PyObject *self, PyObject *args)
{
	char* str_output;
	if (!PyArg_ParseTuple(args, "s", &str_output))
		return NULL;

	printf(str_output);
	return Py_BuildValue("i", true);
}


static PyMethodDef C_debug_Methods[] = {
	{"output",  output, METH_VARARGS},
	{NULL,      NULL}        /* Sentinel */
};


void CGsPyScript::register_c_debug_module()
{
	PyObject* p;
	p = Py_InitModule("c_debug", C_debug_Methods);
	assert(p);
}