#ifndef __TIMESTAMP_EXPORT_TO_LUA_H__
#define __TIMESTAMP_EXPORT_TO_LUA_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif
int register_timestamp_export(lua_State* tolua_S);
#endif