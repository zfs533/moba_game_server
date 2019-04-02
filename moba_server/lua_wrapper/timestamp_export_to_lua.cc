#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>

#include "tolua_fix.h"
#include "../lua_wrapper/lua_wrapper.h"
#include "timestamp_export_to_lua.h"
#include "../utils/timestamp.h"

static int timestamp_tolua(lua_State* tolua_S)
{
	int curTime = timestamp();
	lua_pushinteger(tolua_S,curTime);
	return 1;
}

static int timestamp_today_tolua(lua_State* tolua_S)
{
	int curTime = timestamp_today();
	lua_pushinteger(tolua_S,curTime);
	return 1;
}

static int timestamp_yesterday_tolua(lua_State* tolua_S)
{
	int curTime = timestamp_yesterday();
	lua_pushinteger(tolua_S,curTime);
	return 1;
}

int register_timestamp_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"TimeStamp",0);
		tolua_beginmodule(tolua_S,"TimeStamp");
		tolua_function(tolua_S,"timestamp",timestamp_tolua);
		tolua_function(tolua_S,"timestamp_today",timestamp_today_tolua);
		tolua_function(tolua_S,"timestamp_yesterday",timestamp_yesterday_tolua);
		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,-1);
	return 0;
}