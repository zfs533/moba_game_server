#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "../utils/LibuvTimer.h"
#include "tolua_fix.h"
#include "../lua_wrapper/lua_wrapper.h"
#include "scheduler_export_to_lua.h"
using namespace std;

struct lua_timer_Str
{
	int handler;
	int repeat_count;
};

static void on_lua_repeat_timer(void* udata)
{
	struct lua_timer_Str* lts = (struct lua_timer_Str*)udata;
	int handler = lts->handler;
	int repeat_count = lts->repeat_count;
	lua_wrapper::execute_script_handler(handler,0);
	if(lts->repeat_count == -1)
	{//execute forever
		return;
	}
	lts->repeat_count--;
	if(lts->repeat_count<=0)
	{
		lua_wrapper::remove_script_handler(lts->handler);
		free(lts);
	}
}

static int scheduler_tolua(lua_State* tolua_S)
{
	int handler = toluafix_ref_function(tolua_S,1,0);
	if(handler == 0)
	{
		goto luafailed;
	}
	int after_time = tolua_tonumber(tolua_S,2,0);
	if(after_time < 0)
	{
		goto luafailed;
	}
	int time_offset = tolua_tonumber(tolua_S,3,0);
	int repeat_count = tolua_tonumber(tolua_S,4,0);

	struct lua_timer_Str* lts = (struct lua_timer_Str*)malloc(sizeof(struct lua_timer_Str));
	lts->handler = handler;
	lts->repeat_count = repeat_count;
	STimer* tm = LibuvTimer::getInstance()->schedule_repeat(on_lua_repeat_timer,lts,after_time,time_offset,repeat_count);
	tolua_pushuserdata(tolua_S,tm);
	return 1;
luafailed:
	if(handler != 0)
	{
		lua_wrapper::remove_script_handler(handler);
	}
	lua_pushnil(tolua_S);
	return 1;
}

static int scheduler_once_tolua(lua_State* tolua_S)
{
	int handler = toluafix_ref_function(tolua_S,1,0);
	if(handler == 0)
	{
		goto luafailed;
	}
	int after_time = tolua_tonumber(tolua_S,2,0);
	if(after_time < 0)
	{
		goto luafailed;
	}

	struct lua_timer_Str* lts = (struct lua_timer_Str*)malloc(sizeof(struct lua_timer_Str));
	lts->handler = handler;
	lts->repeat_count = 1;
	STimer* tm = LibuvTimer::getInstance()->scheduleOnce(on_lua_repeat_timer,lts,after_time);
	tolua_pushuserdata(tolua_S,tm);
	return 1;
luafailed:
	if(handler != 0)
	{
		lua_wrapper::remove_script_handler(handler);
	}
	lua_pushnil(tolua_S);
	return 1;
}

static int scheduler_cancel_tolua(lua_State* tolua_S)
{
	STimer* st = (STimer*)tolua_touserdata(tolua_S,1,0);
	if(st == 0)
	{
		goto lua_failed;	
	}
	struct lua_timer_Str* lts = (struct lua_timer_Str*)st->data;
	lua_wrapper::remove_script_handler(lts->handler);
	free(lts);
	LibuvTimer::getInstance()->cancel(st);
lua_failed:
	return 0;
}


int register_scheduler_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"scheduler",0);
		tolua_beginmodule(tolua_S,"scheduler");
		tolua_function(tolua_S,"scheduler",scheduler_tolua);
		tolua_function(tolua_S,"once",scheduler_once_tolua);
		tolua_function(tolua_S,"cancel",scheduler_cancel_tolua);
		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,-1);
	return 0;
}