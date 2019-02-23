#ifndef __LUA_WRAPPER_H
#define __LUA_WRAPPER_H

#include "lua.hpp"

class lua_wrapper
{
public:
	static void init();
	static void exit();
	static bool exe_lua_file(const char* lua_file);
	static void register_luaFunction(const char* func_name,int(*lua_func)(lua_State* L));
public:
	static lua_State* lua_state();
	static int execute_script_handler(int nHandler,int numArgs);
	static void remove_script_handler(int nHandler);
};

#endif