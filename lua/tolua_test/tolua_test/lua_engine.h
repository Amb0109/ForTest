#ifndef _LUA_ENGINE_H_
#define _LUA_ENGINE_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "tolua++.h"
#include "tolua_fix.h"
}

typedef int LUA_FUNCTION;

class LuaEngine
{
	LuaEngine();
	virtual ~LuaEngine();

public:
	static LuaEngine* get_instance();
	lua_State* get_state();

	bool open();
	void close();

	bool run_script(const char* path);
	int run_function(LUA_FUNCTION func_id, int args_cnt);

	void push_int(int val);

	void report_error();

protected:
	int _execute_function(int args_cnt);

private:
	lua_State*	lua_state_;

};

#endif // _LUA_ENGINE_H_