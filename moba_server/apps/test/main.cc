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

int main(int argc,char** argv)
{
	proto_man::init(PROTO_BUF);
	init_pf_cmd_map();
	netbus::instance()->init();
	netbus::instance()->start_tcp_server(6080);
	netbus::instance()->start_ws_server(3000);

	netbus::instance()->run();
	system("pause");
	return 0;
}