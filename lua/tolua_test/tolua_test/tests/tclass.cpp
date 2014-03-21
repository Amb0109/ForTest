#include "tclass.h"

//Test::Tst_A* Test::Tst_A::last;
//Test::Tst_B* Test::Tst_B::last;
//Test::Tst_C* Test::Tst_C::last;


int  tolua_tclass_open (lua_State*);

int run_tclass (LuaEngine* LE)
{
	Test::Tst_B* b = new Test::Tst_B;         // instance used in Lua code

	tolua_tclass_open(LE->get_state());

	LE->run_script("tclass.lua");

	delete b;
	return 0;
}

