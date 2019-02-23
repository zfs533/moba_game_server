/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lua_wrapper/lua_wrapper.h"
#include "redis_export_to_lua.h"
#include "../database/redis_wrapper.h"
#include "tolua_fix.h"
#include "../utils/logger.h"
#include "hiredis.h"


static void on_open_cb(const char* err,void* context,void* udata)
{
	if(err !=NULL)
	{
		lua_pushstring(lua_wrapper::lua_state(),err);
		lua_pushnil(lua_wrapper::lua_state());
	}
	else
	{
		lua_pushnil(lua_wrapper::lua_state());
		tolua_pushuserdata(lua_wrapper::lua_state(),context);
	}
	lua_wrapper::execute_script_handler((int)udata,2);
	lua_wrapper::remove_script_handler((int)udata);
}


static int lua_redis_connect(lua_State* tolua_S)
{
	char* ip = (char*)tolua_tostring(tolua_S,1,0);
	if(ip == NULL)
	{
		goto lua_failed;
	}
	int port = (int)tolua_tonumber(tolua_S,2,0);
	int handler = toluafix_ref_function(tolua_S,3,0);
	redis_wrapper::connect(ip,port,on_open_cb,(void*)handler);

lua_failed:
	return 0;
}


static void push_value_to_table(redisReply* result)
{
	switch (result->type) {
	case REDIS_REPLY_STRING:
	case REDIS_REPLY_STATUS:
		lua_pushstring(lua_wrapper::lua_state(), result->str);
		break;
	case REDIS_REPLY_INTEGER:
		lua_pushinteger(lua_wrapper::lua_state(), result->integer);
		break;
	case REDIS_REPLY_NIL:
		lua_pushnil(lua_wrapper::lua_state());
		break;
	case REDIS_REPLY_ARRAY:
		lua_newtable(lua_wrapper::lua_state());
		int index = 1;
		for (int i = 0; i < result->elements; i++) {
			push_value_to_table(result->element[i]);
			lua_rawseti(lua_wrapper::lua_state(), -2, index); 
			++index;
		}
		break;
	}
}


static void on_lua_query_cb(const char* err, redisReply* result,void* udata) 
{
	if (err) 
	{
		log_error(err);
		lua_pushstring(lua_wrapper::lua_state(),err);
		lua_pushnil(lua_wrapper::lua_state());
	}
	else
	{
		lua_pushnil(lua_wrapper::lua_state());
		if(result)
		{
			
			push_value_to_table(result);
		}
		else
		{
			lua_pushnil(lua_wrapper::lua_state());
		}
	}
	lua_wrapper::execute_script_handler((int)udata,2);
	lua_wrapper::remove_script_handler((int)udata);
	printf("success");
}

static int lua_redis_query(lua_State* tolua_S)
{
	char* context = (char*)tolua_touserdata(tolua_S,1,0);
	if(context == NULL)
	{
		goto lua_failed;
	}
	char* sql = (char*)tolua_tostring(tolua_S,2,0);
	if(sql == NULL)
	{
		goto lua_failed;
	}
	int handler = toluafix_ref_function(tolua_S,3,0);
	if(handler == 0)
	{
		goto lua_failed;
	}
	redis_wrapper::query(context,sql,on_lua_query_cb,(void*)handler);

lua_failed:
	return 0;
}


static int lua_redis_close(lua_State* tolua_S)
{
	void* context = tolua_touserdata(tolua_S,1,0);
	if(context)
	{
		redis_wrapper::close_redis(context);
	}
	return 0;
}

int register_redis_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"redis_wrapper",0);
		tolua_beginmodule(tolua_S,"redis_wrapper");

		tolua_function(tolua_S,"connect",lua_redis_connect);
		tolua_function(tolua_S,"close",lua_redis_close);
		tolua_function(tolua_S,"query",lua_redis_query);

		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,1);
	return 0;
}
*/