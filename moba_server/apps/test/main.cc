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

static void timer_callback(void* data)
{
	log_debug((const char*)data);
}

static void on_query_cb(const char* err, std::vector<std::vector<std::string>>* result) 
{
	if (err) 
	{
		printf("err");
		return;
	}
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
	}
	printf("success");
}


static void connect_cb(const char* err,void* context)
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
	mysql_wrapper::query(context,"select * from student",on_query_cb);

}

void test_db()
{
	mysql_wrapper::connect("127.0.0.1",3306,"my_test","root","123456",connect_cb);
}
int main(int argc,char** argv)
{
	test_db();
	/*
	logger::init("logger/gameserver/","gama_server",true);
	log_debug("%d","lalallalala");
	log_debug("------------test");
	log_debug("%d",timestamp());
	log_debug("%d",timestamp_today());
	LibuvTimer::getInstance()->schedule(timer_callback,"timer",2000,1000,-1);
	LibuvTimer::getInstance()->scheduleOnce(timer_callback,"schedule_once",3000);
	*/


	proto_man::init(PROTO_BUF);
	init_pf_cmd_map();
	netbus::instance()->init();
	netbus::instance()->start_tcp_server(6080);
	netbus::instance()->start_ws_server(3000);
	
	netbus::instance()->run();
	
	system("pause");
	return 0;
}