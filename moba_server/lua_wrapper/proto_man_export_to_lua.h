#ifndef __PROTO_MAN_EXPORT_TO_LUA_H	
#define __PROTO_MAN_EXPORT_TO_LUA_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_proto_man_export(lua_State* tolua_S);
int register_raw_cmd_export(lua_State* tolua_S);

#endif