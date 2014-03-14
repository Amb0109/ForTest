

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

extern "C" {
#include "tests/tarray.h"
}

#include "tests/tclass.h"

void report_error(lua_State* L)
{
	int count = lua_gettop(L);

	while(count > 0)
	{
		const char* msg = lua_tostring(L, -1);

		fprintf(stderr, "%s\n", msg);
		lua_pop(L, 1);
		count--;
	}
}

int main (void)
{
	lua_State* L = lua_open();
	init_tarray(L);
	int ret = luaL_loadfile(L, "tarray.lua");
	if (ret == 0)
	{
		ret = lua_pcall(L, 0, LUA_MULTRET, 0);
	}
	if (ret != 0) report_error(L);

	run_tclass(L);

	lua_close(L);
	return 0;
}
