
#include "tcallback.h"


void TestCB::do_func()
{
	LuaEngine* LE = LuaEngine::get_instance();
	LE->push_int(10086);
	LE->push_int(54321);
	LE->run_function(lua_func_, 2);
}


void run_tcallback(LuaEngine* LE)
{
	int  tolua_tcallback_open (lua_State*);

	tolua_tcallback_open(LE->get_state());
	toluafix_open(LE->get_state());
	LE->run_script("tcallback.lua");
}

