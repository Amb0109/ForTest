#ifndef TCALLBACK_H
#define TCALLBACK_H

#include "../lua_engine.h"

typedef void (*callback_func) (int arg0, int arg1);

class TestCB
{
public:
	TestCB() {}
	~TestCB() {}


public:
	void print() {puts("hehehe");}
	
	void set_callback(callback_func func) { func_ = func;}

	void set_lua_func(int lua_func) { lua_func_ = lua_func;}

	void do_func();

private:

	callback_func func_;

	int  lua_func_;
};



void run_tcallback(LuaEngine* LE);

#endif