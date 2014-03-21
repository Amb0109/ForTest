/*
** Lua binding: tcallback
** Generated automatically by tolua++-1.0.92 on 03/21/14 10:18:19.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_tcallback_open (lua_State* tolua_S);

#include "tcallback.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_TestCB (lua_State* tolua_S)
{
 TestCB* self = (TestCB*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"LUA_FUNCTION");
 tolua_usertype(tolua_S,"TestCB");
}

/* method: new of class  TestCB */
#ifndef TOLUA_DISABLE_tolua_tcallback_TestCB_new00
static int tolua_tcallback_TestCB_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TestCB",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TestCB* tolua_ret = (TestCB*)  Mtolua_new((TestCB)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TestCB");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TestCB */
#ifndef TOLUA_DISABLE_tolua_tcallback_TestCB_new00_local
static int tolua_tcallback_TestCB_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TestCB",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TestCB* tolua_ret = (TestCB*)  Mtolua_new((TestCB)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TestCB");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  TestCB */
#ifndef TOLUA_DISABLE_tolua_tcallback_TestCB_delete00
static int tolua_tcallback_TestCB_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TestCB",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TestCB* self = (TestCB*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: print of class  TestCB */
#ifndef TOLUA_DISABLE_tolua_tcallback_TestCB_print00
static int tolua_tcallback_TestCB_print00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TestCB",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TestCB* self = (TestCB*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'print'", NULL);
#endif
  {
   self->print();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'print'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_lua_func of class  TestCB */
#ifndef TOLUA_DISABLE_tolua_tcallback_TestCB_set_lua_func00
static int tolua_tcallback_TestCB_set_lua_func00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TestCB",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TestCB* self = (TestCB*)  tolua_tousertype(tolua_S,1,0);
  LUA_FUNCTION lua_func = ((LUA_FUNCTION)  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_lua_func'", NULL);
#endif
  {
   self->set_lua_func(lua_func);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_lua_func'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: do_func of class  TestCB */
#ifndef TOLUA_DISABLE_tolua_tcallback_TestCB_do_func00
static int tolua_tcallback_TestCB_do_func00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TestCB",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TestCB* self = (TestCB*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'do_func'", NULL);
#endif
  {
   self->do_func();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_func'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_tcallback_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"TestCB","TestCB","",tolua_collect_TestCB);
  #else
  tolua_cclass(tolua_S,"TestCB","TestCB","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"TestCB");
   tolua_function(tolua_S,"new",tolua_tcallback_TestCB_new00);
   tolua_function(tolua_S,"new_local",tolua_tcallback_TestCB_new00_local);
   tolua_function(tolua_S,".call",tolua_tcallback_TestCB_new00_local);
   tolua_function(tolua_S,"delete",tolua_tcallback_TestCB_delete00);
   tolua_function(tolua_S,"print",tolua_tcallback_TestCB_print00);
   tolua_function(tolua_S,"set_lua_func",tolua_tcallback_TestCB_set_lua_func00);
   tolua_function(tolua_S,"do_func",tolua_tcallback_TestCB_do_func00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_tcallback (lua_State* tolua_S) {
 return tolua_tcallback_open(tolua_S);
};
#endif

