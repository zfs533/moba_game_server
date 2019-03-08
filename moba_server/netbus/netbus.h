/*
1:  netbus: 提供一个全局的对象, instance();
2:  start_tcp_server: 提供启动tcp_server接口;
3:  start_ws_server: 提供启动ws server接口;websocket服务器
4: run接口来开启事件循环;
*/
#ifndef __NETBUS_H
#define __NETBUS_H
#include "session.h"
class netbus
{
public:
	static netbus* instance();
public:
	void init();
	void tcp_listen(int port,const char* ip);
	void ws_listen(int port,const char* ip);
	void udp_listen(int port,const char* ip);
	void run();
	void tcp_connect(const char* server_ip,int port,void(*connected)(int err,session*s,void* udata),void* udata);
};
#endif