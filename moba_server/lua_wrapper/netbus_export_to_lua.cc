#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>

#include "../netbus/netbus.h"
#include "netbus_export_to_lua.h"
using namespace std;

static int tcp_listen_tolua(lua_State* tolua_S)
{
	int port = tolua_tonumber(tolua_S,1,0);
	if(port == 0)
	{
		goto lua_failed;
	}
lua_failed:
	netbus::instance()->tcp_listen(port);
	return 0;
}

static int ws_listen_tolua(lua_State* tolua_S)
{
	int port = tolua_tonumber(tolua_S,1,0);
	if(port == 0)
	{
		goto lua_failed;
	}
lua_failed:
	netbus::instance()->ws_listen(port);
	return 0;
}

static int udp_listen_tolua(lua_State* tolua_S)
{
	int port = tolua_tonumber(tolua_S,1,0);
	if(port == 0)
	{
		goto lua_failed;
	}
lua_failed:
	netbus::instance()->udp_listen(port);
	return 0;
}

int register_netbus_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"netbus",0);
		tolua_beginmodule(tolua_S,"netbus");
		tolua_function(tolua_S,"tcp_listen",tcp_listen_tolua);
		tolua_function(tolua_S,"ws_listen",ws_listen_tolua);
		tolua_function(tolua_S,"udp_listen",udp_listen_tolua);
		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,1);
	return 0;
}