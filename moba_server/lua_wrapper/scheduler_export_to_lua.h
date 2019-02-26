#ifndef __SCHEDULER_EXPORT_TO_LUA_H__
#define __SCHEDULER_EXPORT_TO_LUA_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif
int register_scheduler_export(lua_State* tolua_S);
#endif