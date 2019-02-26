#ifndef __SERVICE_EXPRORT_TO_LUA_H
#define __SERVICE_EXPRORT_TO_LUA_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_service_export(lua_State* tolua_S);

#endif