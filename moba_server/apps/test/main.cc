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

static void timer_callback(void* data)
{
	log_debug((const char*)data);
}

int main(int argc,char** argv)
{
	logger::init("logger/gameserver/","gama_server",true);
	log_debug("%d","lalallalala");
	log_debug("------------test");
	log_debug("%d",timestamp());
	log_debug("%d",timestamp_today());
	//LibuvTimer::getInstance()->schedule(timer_callback,"timer",2000,1000,-1);
	LibuvTimer::getInstance()->scheduleOnce(timer_callback,"schedule_once",3000);
	


	proto_man::init(PROTO_BUF);
	init_pf_cmd_map();
	netbus::instance()->init();
	netbus::instance()->start_tcp_server(6080);
	netbus::instance()->start_ws_server(3000);
	
	netbus::instance()->run();
	
	system("pause");
	return 0;
}