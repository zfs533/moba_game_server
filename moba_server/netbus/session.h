/*
管理客户端接入进来的tcp
接口（抽象类）
1: 发送数据;
2  获取session信息;
3: 提供 close函数主动关闭socket;
session对象是客户端每一个TCP连接都会对应一个sesion
其实就是客户端接入时，对应的给每个客户端分配一个session对象，这个sission对象用来管理当前接入客户端的一切
*/
#ifndef __SESSION_H
#define __SESSION_H
class session
{
public:
	virtual void close() = 0;//=0表示纯虚函数
	virtual void send_data(unsigned char* body,int len) = 0;
	virtual const char* get_address(int* client_port) = 0;
	virtual void send_msg(struct cmd_msg* msg) = 0;
};
#endif