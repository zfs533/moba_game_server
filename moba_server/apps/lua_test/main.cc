#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>

//libuv lib
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Userenv.lib")

//protobuf lib
#pragma comment(lib,"libprotobufd.lib")
#pragma comment(lib,"libprotocd.lib")

using namespace std;

#include "../../netbus/netbus.h"
#include "../../../lua_wrapper/lua_wrapper.h"

//lua script develop main func
int main(int argc,char** argv)
{
	netbus::instance()->init();
	lua_wrapper::init();
	if(argc != 3)
	{
		string search_path = "../../../apps/lua_test/scripts/";
		lua_wrapper::add_search_path(search_path);
		string lua_start_file = search_path +"main.lua";
		lua_wrapper::do_file(lua_start_file);
	}
	else
	{
		string search_path = argv[1];
		if(*(search_path.end()-1)!='/')
		{
			search_path += '/';
		}
		lua_wrapper::add_search_path(search_path);
		string lua_file = search_path + argv[2];
		lua_wrapper::do_file(lua_file);
	}
	netbus::instance()->run();
	lua_wrapper::exit();	
	system("pause");
	return 0;
}