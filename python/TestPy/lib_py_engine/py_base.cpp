#include "py_base.h"



namespace py_engine
{

int initialize() { Py_Initialize(); return Py_IsInitialized(); }
int is_initialized() { return Py_IsInitialized(); }
void finalize() { if(!Py_IsInitialized()) return; Py_Finalize(); }
void add_script_path( char* script_path ) { if(!Py_IsInitialized()) return; PySys_SetPath(script_path); }
int run_simple_string( char* script_str ){ if(!Py_IsInitialized()) return NULL; return PyRun_SimpleString(script_str); }


}
