#include "lua_engine.h"


LuaEngine::LuaEngine()
	: lua_state_(NULL)
{

}

LuaEngine::~LuaEngine()
{

}

LuaEngine* LuaEngine::get_instance()
{
	static LuaEngine _global_ge_lua_engine;
	return &_global_ge_lua_engine;
}

lua_State* LuaEngine::get_state()
{
	return lua_state_;
}

bool LuaEngine::open()
{
	lua_state_ = lua_open();
	if (lua_state_ == NULL) return false;

	luaL_openlibs(lua_state_);
	return true;
}

void LuaEngine::close()
{
	if (lua_state_) lua_close(lua_state_);
	lua_state_ = NULL;
}

bool LuaEngine::run_script( const char* path )
{
	if (lua_state_ == NULL) return false;
	bool err = luaL_dofile(lua_state_, path);
	if (err) report_error();
	return !err;
}

int LuaEngine::run_function( LUA_FUNCTION func_id, int args_cnt )
{
	if (lua_state_ == NULL) return false;

	toluafix_get_function_by_refid(lua_state_, func_id);
	if (!lua_isfunction(lua_state_, -1))
	{
		fprintf(stderr, "function refid '%d' does not reference a Lua function\n", func_id);
		lua_pop(lua_state_, 1);
		lua_settop(lua_state_, 0);
		return false;
	}

	if (args_cnt > 0)
	{
		lua_insert(lua_state_, -(args_cnt + 1));
	}
	int ret = _execute_function(args_cnt);

	lua_settop(lua_state_, 0);
	return ret;
}

void LuaEngine::report_error()
{
	if (lua_state_ == NULL) return;
	int count = lua_gettop(lua_state_);

	while(count > 0)
	{
		const char* msg = lua_tostring(lua_state_, -1);

		fprintf(stderr, "%s\n", msg);
		lua_pop(lua_state_, 1);
		count--;
	}
}

int LuaEngine::_execute_function( int args_cnt )
{
	int functionIndex = -(args_cnt + 1);
	if (!lua_isfunction(lua_state_, functionIndex))
	{
		fprintf(stderr, "value at stack [%d] is not function", functionIndex);
		lua_pop(lua_state_, args_cnt + 1);
		return 0;
	}

	int err = lua_pcall(lua_state_, args_cnt, 1, 0);
	if (err) report_error();

	int ret = 0;
	if (lua_isnumber(lua_state_, -1))
	{
		ret = lua_tointeger(lua_state_, -1);
	}
	else if (lua_isboolean(lua_state_, -1))
	{
		ret = lua_toboolean(lua_state_, -1);
	}
	lua_pop(lua_state_, 1);

	return ret;
}

void LuaEngine::push_int( int val )
{
	lua_pushinteger(lua_state_, val);
}


