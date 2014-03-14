// lua_tinker_test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include "lua_tinker.h"

struct base
{
	base() {}

	const char* is_base(){ return "this is base"; }
};

class test : public base
{
public:
	test() { puts("test up"); }
	~test() {}

	const char* is_test(){ return "this is test"; }
};

int _tmain(int argc, _TCHAR* argv[])
{
	lua_State* L = lua_open();
	luaopen_base(L);

	// ע��base���͵�LUA
	lua_tinker::class_add<base>(L, "base");
	lua_tinker::class_def<base>(L, "is_base", &base::is_base);

	// ע��test���͵�LUA,ע��test�ĳ�Ա�����ͳ�Ա����
	lua_tinker::class_add<test>(L, "test");
	lua_tinker::class_inh<test, base>(L);
	lua_tinker::class_con<test>(L, &lua_tinker::constructor<test>);
	lua_tinker::class_def<test>(L, "is_test", &test::is_test);

	lua_tinker::dofile(L, "test.lua");

	lua_close(L);

	return 0;
}

