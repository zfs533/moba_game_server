#ifndef __LUA_WRAPPER_H
#define __LUA_WRAPPER_H

#include "lua.hpp"
#include <string>
class lua_wrapper
{
public:
	static void init();
	static void exit();
	static bool do_file(std::string& lua_file);
	static void register_luaFunction(const char* func_name,int(*lua_func)(lua_State* L));
	static void add_search_path(std::string& path);
public:
	static lua_State* lua_state();
	static int execute_script_handler(int nHandler,int numArgs);
	static void remove_script_handler(int nHandler);
public:
	static void do_log_message(void(*log)(const char* file_name, int line_num, const char* msg), const char* msg);
	static void print_debug(const char* file_name,int line_num,const char* msg);
	static void print_warning(const char* file_name,int line_num,const char* msg);
	static void print_error(const char* file_name,int line_num,const char* msg);
};

#endif