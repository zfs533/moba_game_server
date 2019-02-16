/*
1:  netbus: 提供一个全局的对象, instance();
2:  start_tcp_server: 提供启动tcp_server接口;
3:  start_ws_server: 提供启动ws server接口;websocket服务器
4: run接口来开启事件循环;
*/
#ifndef __NETBUS_H
#define __NETBUS_H
class netbus
{
public:
	static netbus* instance();
public:
	void init();
	void start_tcp_server(int port);
	void start_ws_server(int port);
	void run();
};
#endif