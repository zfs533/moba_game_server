#ifndef __LOGGER_EXPORT_TO_LUA_H__
#define __LOGGER_EXPORT_TO_LUA_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_logger_tolua(lua_State* tolua_S);

#endif