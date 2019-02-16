#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Userenv.lib")

#include "uv.h"
#include "../../netbus/netbus.h"
using namespace std;

int main(int argc,char** argv)
{
	netbus::instance()->init();
	netbus::instance()->start_tcp_server(6080);
	netbus::instance()->start_ws_server(3000);
	netbus::instance()->run();
	system("pause");
	return 0;
}