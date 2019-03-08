#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include "../utils/logger.h"
#include "../lua_wrapper/lua_wrapper.h"
#include "logger_export_to_lua.h"
using namespace std;

static int logger_debug_tolua(lua_State* luastate)
{
	int nargs = lua_gettop(luastate);
	std::string t;
	for (int i = 1; i <= nargs; i++)
	{
		if (lua_istable(luastate, i))
			t += "table";
		else if (lua_isnone(luastate, i))
			t += "none";
		else if (lua_isnil(luastate, i))
			t += "nil";
		else if (lua_isboolean(luastate, i))
		{
			if (lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if (lua_isfunction(luastate, i))
			t += "function";
		else if (lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if (lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char * str = lua_tostring(luastate, i);
			if (str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if (i != nargs)
			t += "\t";
	}
	lua_wrapper::do_log_message(lua_wrapper::print_debug, t.c_str());
luafield:
	return 0;
}

static int logger_warning_tolua(lua_State* luastate)
{
int nargs = lua_gettop(luastate);
	std::string t;
	for (int i = 1; i <= nargs; i++)
	{
		if (lua_istable(luastate, i))
			t += "table";
		else if (lua_isnone(luastate, i))
			t += "none";
		else if (lua_isnil(luastate, i))
			t += "nil";
		else if (lua_isboolean(luastate, i))
		{
			if (lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if (lua_isfunction(luastate, i))
			t += "function";
		else if (lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if (lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char * str = lua_tostring(luastate, i);
			if (str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if (i != nargs)
			t += "\t";
	}
	lua_wrapper::do_log_message(lua_wrapper::print_warning, t.c_str());
luafield:
	return 0;
}

static int logger_error_tolua(lua_State* luastate)
{
int nargs = lua_gettop(luastate);
	std::string t;
	for (int i = 1; i <= nargs; i++)
	{
		if (lua_istable(luastate, i))
			t += "table";
		else if (lua_isnone(luastate, i))
			t += "none";
		else if (lua_isnil(luastate, i))
			t += "nil";
		else if (lua_isboolean(luastate, i))
		{
			if (lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if (lua_isfunction(luastate, i))
			t += "function";
		else if (lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if (lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char * str = lua_tostring(luastate, i);
			if (str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if (i != nargs)
			t += "\t";
	}
	lua_wrapper::do_log_message(lua_wrapper::print_error, t.c_str());
luafield:
	return 0;
}

static int logger_init_tolua(lua_State* tolua_S)
{
	const char* path = luaL_checkstring(tolua_S,1);
	if(!path)
	{
		goto luafield;
	}
	const char* module_name = luaL_checkstring(tolua_S,2);
	if(!path)
	{
		goto luafield;
	}
	bool print_cmd = tolua_toboolean(tolua_S,3,true);
	logger::init((char*)path,(char*)module_name,print_cmd);

luafield:
	return 0;
}

int register_logger_tolua(lua_State* tolua_S)
{
	//lua_wrapper::register_luaFunction("print",logger_debug_tolua);
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"Logger",0);
		tolua_beginmodule(tolua_S,"Logger");
		tolua_function(tolua_S,"debug",logger_debug_tolua);
		tolua_function(tolua_S,"warning",logger_warning_tolua);
		tolua_function(tolua_S,"error",logger_error_tolua);
		tolua_function(tolua_S,"init",logger_init_tolua);
		tolua_endmodule(tolua_S);
	}

	lua_pop(tolua_S,1);
	return 0;
}