#ifndef __NETBUS_EXPORT_TO_LUA_H	
#define __NETBUS_EXPORT_TO_LUA_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_netbus_export(lua_State* tolua_S);

#endif