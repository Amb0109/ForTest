
#include "lua_engine.h"

#include "tests/tarray.h"
#include "tests/tclass.h"
#include "tests/tcallback.h"

int main (void)
{
	LuaEngine* LE = LuaEngine::get_instance();
	LE->open();

	lua_State* L = LE->get_state();

	run_tarray(LE);
	run_tclass(LE);

	run_tcallback(LE);
	

	LE->close();
	return 0;
}
