#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>

#include "tolua_fix.h"
#include "../netbus/netbus.h"
#include "netbus_export_to_lua.h"
#include "lua_wrapper.h"
using namespace std;

static int tcp_listen_tolua(lua_State* tolua_S)
{
	int port = tolua_tonumber(tolua_S,1,0);
	const char* ip = tolua_tostring(tolua_S,2,0);
	if(port == 0 || ip == NULL)
	{
		goto lua_failed;
	}
lua_failed:
	netbus::instance()->tcp_listen(port,ip);
	return 0;
}

static int ws_listen_tolua(lua_State* tolua_S)
{
	int port = tolua_tonumber(tolua_S,1,0);
	const char* ip = tolua_tostring(tolua_S,2,0);
	if(port == 0 || ip == NULL)
	{
		goto lua_failed;
	}
lua_failed:
	netbus::instance()->ws_listen(port,ip);
	return 0;
}

static int udp_listen_tolua(lua_State* tolua_S)
{
	int port = tolua_tonumber(tolua_S,1,0);
	const char* ip = tolua_tostring(tolua_S,2,0);
	if(port == 0 || ip == NULL)
	{
		goto lua_failed;
	}
lua_failed:
	netbus::instance()->udp_listen(port,ip);
	return 0;
}

static void on_tcp_connected(int err,session*s,void* udata)
{
	if(err)
	{
		lua_pushinteger(lua_wrapper::lua_state(),err);
		lua_pushnil(lua_wrapper::lua_state());
	}
	else
	{
		lua_pushinteger(lua_wrapper::lua_state(),err);
		tolua_pushuserdata(lua_wrapper::lua_state(),s);
	}
	lua_wrapper::execute_script_handler((int)udata,2);
	lua_wrapper::remove_script_handler((int)udata);
}

static int tcp_connect_tolua(lua_State* tolua_S)
{
	const char* ip = luaL_checkstring(tolua_S,1);
	if(ip == NULL)
	{
		goto lua_failed;
	}
	int port = luaL_checkinteger(tolua_S,2);
	int handler = toluafix_ref_function(tolua_S,3,0);
	if(handler == 0)
	{
		goto lua_failed;
	}
	netbus::instance()->tcp_connect(ip,port,on_tcp_connected,(void*)handler);

lua_failed:
	return 0;
}

int register_netbus_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"Netbus",0);
		tolua_beginmodule(tolua_S,"Netbus");
		tolua_function(tolua_S,"tcp_listen",tcp_listen_tolua);
		tolua_function(tolua_S,"ws_listen",ws_listen_tolua);
		tolua_function(tolua_S,"udp_listen",udp_listen_tolua);
		tolua_function(tolua_S,"tcp_connect",tcp_connect_tolua);
		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,1);
	return 0;
}