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
#include "proto/pf_cmd_map.h"
#include "../../netbus/proto_man.h"
#include "../../utils/LibuvTimer.h"
#include "../../utils/logger.h"
#include "../../utils/timestamp.h"
#include "../../database/mysql_wrapper.h"
#include "../../../lua_wrapper/lua_wrapper.h"

static void timer_callback(void* data)
{
	log_debug((const char*)data);
}

static void on_query_cb(const char* err, MYSQL_RES* result,void* udata) 
{
	if (err) 
	{
		printf("err");
		return;
	}/*
	int size = result->size();
	for(int i = 0; i<size; i++)
	{
		int num = result[i].size();
		for(int j = 0; j<num; j++)
		{
			int fields = result[i][j].size();
			for(int n = 0; n <fields; n++)
			{
				string item = result[i][j][n];
				printf("%s ",item.c_str());
			}
			printf("\n");
		}
		break;
	}*/
	printf("success");
}


static void connect_cb(const char* err,void* context,void* data)
{
	if(err !=NULL)
	{
		printf("%s",err);
		return;
	}
	printf("success connect...\n");
	//mysql_wrapper::query(context,"update student set name = \"lebulang\" where id = 2",on_query_cb);
	//mysql_wrapper::query(context,"delete from student where id = 5",on_query_cb);
	//mysql_wrapper::query(context,"insert into student(name,age) values(\"xiaowang\",11230)",on_query_cb);
	mysql_wrapper::query(context,"select * from student",on_query_cb,"");

}

void test_db()
{
	mysql_wrapper::connect("127.0.0.1",3306,"my_test","root","123456",connect_cb);
}

void test_lua_wrapper()
{
	lua_wrapper::init();
	std::string luafile = "main.lua";
	lua_wrapper::do_file(luafile);
	/*
	string lua_head = "../../../lua_script/";
	string lua_file = "main.lua";
	const char* fileaa = (lua_head + lua_file).c_str();
	printf("%s\n",lua_head.c_str());
	printf("%s\n",(lua_head + lua_file).c_str());

	printf("%s\n",fileaa);
	lua_wrapper::exe_lua_file((lua_head + lua_file).c_str());
	*/
}

int main(int argc,char** argv)
{
	logger::init("logger/gameserver/","gama_server",true);
	log_debug("-------11111111111");
	//test_db();
	//test_lua_wrapper();

	proto_man::init(PROTO_BUF);
	init_pf_cmd_map();
	netbus::instance()->init();
	netbus::instance()->tcp_listen(6080,"127.0.0.1");
	netbus::instance()->ws_listen(3000,"10.0.7.65");
	netbus::instance()->udp_listen(6081,"127.0.0.1");
	
	netbus::instance()->run();
	
	system("pause");
	return 0;
}