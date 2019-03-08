#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <map>
#include "tolua_fix.h"
#include "../netbus/proto_man.h"
#include "proto_man_export_to_lua.h"
using namespace std;

static int proto_man_init_tolua(lua_State* tolua_S)
{
	int type = tolua_tonumber(tolua_S,1,-1);
	if(type == -1)
	{
		goto lua_failed;
	}
	proto_man::init(type);
lua_failed:
	return 0;
}

static int proto_type_tolua(lua_State* tolua_S)
{
	int type = proto_man::proto_type();
	if(type != PROTO_BUF && type != PROTO_JSON)
	{
		goto lua_failed;
	}
	lua_pushinteger(tolua_S,type);
	
	return 1;
lua_failed:
	lua_pushnil(tolua_S);
	return 1;
}

static int register_protobuf_cmd_map_tolua(lua_State* tolua_S)
{
	map<int,string> mp;
	int len = luaL_len(tolua_S,1);
	for(int i = 1; i<=len; ++i)
	{
		lua_pushnumber(tolua_S,i);
		lua_gettable(tolua_S,1);
		const char* name = luaL_checkstring(tolua_S,-1);
		if(name)
		{
			mp[i] = name;
		}
		lua_pop(tolua_S,1);
	}
	proto_man::register_protobuf_cmd_map(mp);
lua_failed:
	return 0;
}

int register_proto_man_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"ProtoMan",0);
		tolua_beginmodule(tolua_S,"ProtoMan");
		tolua_function(tolua_S,"init",proto_man_init_tolua);
		tolua_function(tolua_S,"proto_type",proto_type_tolua);
		tolua_function(tolua_S,"register_protobuf_cmd_map",register_protobuf_cmd_map_tolua);
		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,1);
	return 0;
}