#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
using namespace std;

#include "uv.h"
#include "session.h"
#include "session_uv.h"
#include "netbus.h"
extern "C"
{
static void on_close(uv_handle_t* handle)
{
	uv_session* s = (uv_session*)handle->data;
	uv_session::destroy(s);
}

static void on_shutdown(uv_shutdown_t* req, int status)
{
	uv_close((uv_handle_t*)req->handle,on_close);
}
static void after_read(uv_stream_t* stream,
                           ssize_t nread,
                           const uv_buf_t* buf)
{
	uv_session* s = (uv_session*)stream->data;
	//连接端口，客户端掉线
	if(nread<0)
	{
		uv_shutdown_t* reg = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
		memset(reg,0,sizeof(uv_shutdown_t));
		uv_shutdown(reg,stream,on_shutdown);//on_shutdown回调函数
		return;
	}
	buf->base[nread] = 0;//加一个结束符
	printf("recv %d\n",nread);
	printf("%s\n",buf->base);
	//测试发送给我们的客户端
	s->send_data((unsigned char*)buf->base,nread);
	s->recved = 0;
}

static void uv_alloc_buf(uv_handle_t* handle,
                            size_t suggested_size,
                            uv_buf_t* buf)
{
	uv_session* s = (uv_session*)handle->data;
	*buf = uv_buf_init(s->recv_buf+s->recved,RECV_LEN-s->recved);//申请一块接收数据的内存
}
static void uv_connection(uv_stream_t* server, int status)
{
	uv_session* s = uv_session::create();
	uv_tcp_t* client = &s->tcp_handler;
	//接入客户端
	memset(client,0,sizeof(uv_tcp_t));
	uv_tcp_init(uv_default_loop(),client);
	client->data = (void*)s;
	uv_accept(server,(uv_stream_t*)client);
	//--
	struct sockaddr_in addr;
	int len = sizeof(addr);
	uv_tcp_getpeername(client,(sockaddr*)&addr,&len);
	uv_ip4_name(&addr,(char*)s->c_address,64);
	s->c_port = ntohs(addr.sin_port);
	s->socket_type = (int)server->data;//current connect server type
	printf("new client comming %s:%d\n",s->c_address,s->c_port);
	//end
	//告诉event loop，让他帮你管理哪个事件
	//将client交给libuv event loop管理
	uv_read_start((uv_stream_t*)client,uv_alloc_buf,after_read);
}

}

static netbus g_netbus;
netbus* netbus::instance()
{
	return &g_netbus;
}

void netbus::start_tcp_server(int port)
{
	uv_tcp_t* listen = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	memset(listen,0,sizeof(uv_tcp_t));
	uv_tcp_init(uv_default_loop(),listen);
	struct sockaddr_in addr;
	uv_ip4_addr("127.0.0.1",port,&addr);
	//bind ip and port
	int ret = uv_tcp_bind(listen,(const struct sockaddr*)&addr,0);
	if(ret !=0)
	{
		printf("bind error\n");
		free(listen);
		return;
	}
	//start listen client connect in
	uv_listen((uv_stream_t*)listen,SOMAXCONN,uv_connection);
	listen->data = (void*)TCP_SOCKET;
}

void netbus::start_ws_server(int port)
{}

void netbus::run()
{
	uv_run(uv_default_loop(),UV_RUN_DEFAULT);
}