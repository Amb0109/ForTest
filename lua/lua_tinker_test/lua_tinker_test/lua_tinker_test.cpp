// lua_tinker_test.cpp : 定义控制台应用程序的入口点。
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
	base() { puts("new base"); }
	virtual ~base() { puts("release base"); }

	const char* is_base(){ return "this is base"; }
	virtual void is_what() { puts("this is base!"); }
};

struct xyz
{
	float x;
	float y;
	float z;
};

class test : public base
{
public:
	test() { puts("new test"); }
	virtual ~test() { puts("release test"); }

	const char* is_test(){ return "this is test"; }
	virtual void is_what() { puts("this is test!"); }
	
	xyz* get_xyz() {return &pos;}
	xyz& get_xyz2() {return pos;}
	void set_xyz(float x, float y, float z)	{pos.x = x; pos.y = y; pos.z = z;}

private:
	xyz pos;
};

int _tmain(int argc, _TCHAR* argv[])
{
	lua_State* L = lua_open();
	luaopen_base(L);

	lua_tinker::class_add<xyz>(L, "xyz");
	lua_tinker::class_mem<xyz>(L, "x", &xyz::x);
	lua_tinker::class_mem<xyz>(L, "y", &xyz::y);
	lua_tinker::class_mem<xyz>(L, "z", &xyz::z);

	lua_tinker::class_add<base>(L, "base");
	lua_tinker::class_con<base>(L, &lua_tinker::constructor<base>);
	lua_tinker::class_def<base>(L, "is_base", &base::is_base);
	lua_tinker::class_def<base>(L, "is_what", &base::is_what);

	lua_tinker::class_add<test>(L, "test");
	lua_tinker::class_inh<test, base>(L);
	lua_tinker::class_con<test>(L, &lua_tinker::constructor<test>);
	lua_tinker::class_def<test>(L, "is_test", &test::is_test);
	lua_tinker::class_def<test>(L, "is_what", &test::is_what);
	lua_tinker::class_def<test>(L, "get_xyz", &test::get_xyz);
	lua_tinker::class_def<test>(L, "get_xyz2", &test::get_xyz2);
	lua_tinker::class_def<test>(L, "set_xyz", &test::set_xyz);

	lua_tinker::dofile(L, "test.lua");

	lua_close(L);

	return 0;
}

