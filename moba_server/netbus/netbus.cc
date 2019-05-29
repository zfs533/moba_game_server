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
#include "ws_protocal.h"
#include "tp_protocol.h"
#include "proto_man.h"
#include "service_man.h"
#include "udp_session.h"
#include "../utils/small_alloc.h"

extern "C"
{
static void on_uv_udp_send_end(uv_udp_send_t* req,int status)
{
	if (status == 0 )
	{
	}
	small_free(req);
}

struct udp_recv_buf {
	char* recv_buf;
	size_t max_recv_len;
};

static void on_recv_client_cmd(session* s,unsigned char* body,int len)
{
	printf("数据解析完成,给客户端发送数据操作..\n");
	static char test_buf[4096];
	memcpy(test_buf,body,len);
	test_buf[len] = 0;
	printf("recv:%s\n",test_buf);
	//test
	//s->send_data(body,len);
	struct raw_cmd raw;
	if(proto_man::decode_raw_cmd(body,len,&raw))
	{
		if(!service_man::on_recv_cmd_msg((session*)s, &raw))
		{
			s->close();
		}
	}

	/*
	struct cmd_msg* msg = NULL;
	if(proto_man::decode_cmd_msg(body,len,&msg))
	{
		if(!service_man::on_recv_cmd_msg((session*)s,msg))
		{
			s->close();
		}
		//s->send_msg(msg);
		proto_man::cmd_msg_free(msg);
	}
	*/
}

static void on_recv_tcp_data(uv_session* s)
{
	unsigned char* pkg_data = (unsigned char*)(s->long_pkg != NULL ? s->long_pkg : s->recv_buf);
	while(s->recved>0)
	{
		int pkg_size = 0;
		int head_size = 0;

		if(!tp_protocol::read_header(pkg_data,s->recved,&pkg_size,&head_size))
		{
			break;
		}
		//包的数据大小必须要大于>数据头的大小
		if(pkg_size<=head_size)
		{
			s->close();
			break;
		}
		if(s->recved <pkg_size)
		{
			break;
		}
		unsigned char* raw_data = pkg_data + head_size;
		//recv client command
		on_recv_client_cmd((session*)s,raw_data,pkg_size-head_size);
		//end
		if(s->recved > pkg_size)
		{
			memmove(pkg_data,pkg_data+pkg_size,s->recved - pkg_size);
		}
		s->recved -= pkg_size;
		if(s->recved == 0 && s->long_pkg != NULL)
		{
			free(s->long_pkg);
			s->long_pkg = NULL;
			s->long_pkg_size = 0;
		}
	}
}

static void on_recv_ws_data(uv_session* s)
{
	unsigned char* pkg_data = (unsigned char*)(s->long_pkg != NULL ? s->long_pkg : s->recv_buf);
	while(s->recved>0)
	{
		int pkg_size = 0;
		int head_size = 0;
		if(pkg_data[0] == 0x88)//客户端关闭
		{
			s->close();
			break;
		}
		//pkg_size - head_size = body_size
		if(!ws_protocol::read_ws_header(pkg_data,s->recved,&pkg_size,&head_size))
		{
			break;
		}
		if(s->recved <pkg_size)
		{
			break;
		}
		unsigned char* raw_data = pkg_data + head_size;
		unsigned char* mask = raw_data - 4;
		ws_protocol::parser_ws_recv_data(raw_data,mask,pkg_size-head_size);
		//recv client command
		on_recv_client_cmd((session*)s,raw_data,pkg_size-head_size);
		//end
		if(s->recved > pkg_size)
		{
			memmove(pkg_data,pkg_data+pkg_size,s->recved - pkg_size);
		}
		s->recved -= pkg_size;
		if(s->recved == 0 && s->long_pkg != NULL)
		{
			free(s->long_pkg);
			s->long_pkg = NULL;
			s->long_pkg_size = 0;
		}
	}
}

static void on_close(uv_handle_t* handle)
{
	uv_session* s = (uv_session*)handle->data;
	uv_session::destroy(s);
}

static void on_shutdown(uv_shutdown_t* req, int status)
{
	uv_close((uv_handle_t*)req->handle,on_close);
}

static void after_uv_udp_recv(uv_udp_t* handle,ssize_t nread,const uv_buf_t* buf,const struct sockaddr* addr, unsigned flags) 
{
	udp_session udp_s;
	udp_s.udp_handler = handle;
	udp_s.addr = addr;
	uv_ip4_name((struct sockaddr_in*)addr,udp_s.c_address,32);
	udp_s.c_port = ntohs(((struct sockaddr_in*)addr)->sin_port);
	on_recv_client_cmd((session*)&udp_s,(unsigned char*)buf->base,nread);
}

static void after_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	uv_session* s = (uv_session*)stream->data;
	//连接端口，客户端掉线
	if(nread<0)
	{
		/*
		uv_shutdown_t* reg = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
		memset(reg,0,sizeof(uv_shutdown_t));
		uv_shutdown(reg,stream,on_shutdown);//on_shutdown回调函数
		*/
		s->close();
		return;
	}
	s->recved += nread;
	if(s->socket_type == WS_SOCKET)
	{
		//websocket
		if(!s->is_ws_shaked)
		{
			if(ws_protocol::ws_shake_hand((session*)s,s->recv_buf,s->recved))
			{
				printf("shake_handle_success..\n");
				s->is_ws_shaked = true;
				s->recved = 0;
			}
		}
		else
		{
			on_recv_ws_data(s);
		}
	}
	else if(s->socket_type == TCP_SOCKET)
	{
		//tcp socket
		on_recv_tcp_data(s);
	}
	/*
	buf->base[nread] = 0;//加一个结束符
	printf("recv %d\n",nread);
	printf("%s\n",buf->base);
	//测试发送给我们的客户端
	s->send_data((unsigned char*)buf->base,nread);
	s->recved = 0;
	*/
}

static void udp_uv_alloc_buf(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf)
{
	suggested_size = suggested_size < 8096 ? 8096 : suggested_size;
	struct udp_recv_buf* udp_buf = (struct udp_recv_buf*)handle->data;
	if(udp_buf->max_recv_len < suggested_size)
	{
		if(udp_buf->recv_buf)
		{
			free(udp_buf->recv_buf);
			udp_buf->recv_buf = NULL;
		}
		udp_buf->recv_buf = (char*)malloc(suggested_size);
		udp_buf->max_recv_len = suggested_size;
	}
	buf->base = udp_buf->recv_buf;
	buf->len = suggested_size;
}

static void uv_alloc_buf(uv_handle_t* handle,
                            size_t suggested_size,
                            uv_buf_t* buf)
{
	uv_session* s = (uv_session*)handle->data;
	if(s->recved <RECV_LEN)
	{
		*buf = uv_buf_init(s->recv_buf+s->recved,RECV_LEN-s->recved);//申请一块接收数据的内存
	}
	else//recv数据包长度大于预先定义的接收长度
	{
		if(s->long_pkg == NULL)
		{
			if(s->socket_type == WS_SOCKET && s->is_ws_shaked)
			{
				//websocket接收数据包
				int pkg_size;
				int head_size;
				ws_protocol::read_ws_header((unsigned char*)s->recv_buf,s->recved,&pkg_size,&head_size);
				s->long_pkg_size = pkg_size;
				s->long_pkg = (char*)malloc(pkg_size);
				memcpy(s->long_pkg,s->recv_buf,s->recved);
			}
			else
			{
				//tcp socket recv data pack
				int pkg_size;
				int head_size;
				tp_protocol::read_header((unsigned char*)s->recv_buf,s->recved,&pkg_size,&head_size);
				s->long_pkg_size = pkg_size;
				s->long_pkg = (char*)malloc(pkg_size);
				memcpy(s->long_pkg,s->recv_buf,s->recved);
			}
			*buf = uv_buf_init(s->long_pkg + s->recved,s->long_pkg_size - s->recved);
		}
	}

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
	service_man::on_session_connect((session*)s);
}

}

static netbus g_netbus;
netbus* netbus::instance()
{
	return &g_netbus;
}

void netbus::udp_listen(int port,const char* ip)
{
	if(this->udp_handler)
	{
		return;
	}
	uv_udp_t* server = (uv_udp_t*)malloc(sizeof(uv_udp_t));
	memset(server,0,sizeof(uv_udp_t));
	uv_udp_init(uv_default_loop(),server);
	struct udp_recv_buf* udp_buf = (struct udp_recv_buf*)malloc(sizeof(struct udp_recv_buf));
	memset(udp_buf,0,sizeof(struct udp_recv_buf));
	server->data = (struct udp_recv_buf*) udp_buf;
	
	struct sockaddr_in addr;
	//uv_ip4_addr("127.0.0.1",port,&addr);
	uv_ip4_addr(ip,port,&addr);
	uv_udp_bind(server,(const struct sockaddr*)&addr,0);
	this->udp_handler = (void*)server;
	uv_udp_recv_start(server,udp_uv_alloc_buf,after_uv_udp_recv);
}

void netbus::tcp_listen(int port,const char* ip)
{
	uv_tcp_t* listen = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	memset(listen,0,sizeof(uv_tcp_t));
	uv_tcp_init(uv_default_loop(),listen);
	struct sockaddr_in addr;
	//uv_ip4_addr("127.0.0.1",port,&addr);
	uv_ip4_addr(ip,port,&addr);
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

void netbus::ws_listen(int port,const char* ip)
{
	uv_tcp_t* listen = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	memset(listen,0,sizeof(uv_tcp_t));
	uv_tcp_init(uv_default_loop(),listen);
	struct sockaddr_in addr;
	//uv_ip4_addr("127.0.0.1",port,&addr);
	//uv_ip4_addr("10.0.7.65",port,&addr);
	uv_ip4_addr(ip,port,&addr);
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
	listen->data = (void*)WS_SOCKET;
}

void netbus::run()
{
	uv_run(uv_default_loop(),UV_RUN_DEFAULT);
}

void netbus::init()
{
	service_man::init();
	init_session_allocer();
}
//-------------
struct connect_cb 
{
	void(*on_connected)(int err, session* s, void* udata);
	void* udata;
};

static void after_connect(uv_connect_t* handle, int status)
{
	uv_session* s= (uv_session*)handle->handle->data;
	struct connect_cb* cb = (struct connect_cb*)handle->data;
	if(status)
	{
		if(cb->on_connected)
		{
			cb->on_connected(1,NULL,cb->udata);

		}
		s->close();
		free(cb);
		free(handle);
		return;
	}
	if(cb->on_connected)
	{
		cb->on_connected(0,(session*)s,cb->udata);
	}
	uv_read_start((uv_stream_t*)handle->handle,uv_alloc_buf,after_read);
	free(cb);
	free(handle);
}



//server as client to connect to other server(eg:gateway server)
void netbus::tcp_connect(const char* server_ip,int port,void(*connected)(int err,session*s,void* udata),void* udata)
{
	struct sockaddr_in bind_addr;
	int iret = uv_ip4_addr(server_ip,port,&bind_addr);
	if(iret)
	{
		return ;
	}
	uv_session* s = uv_session::create();
	uv_tcp_t* client = &s->tcp_handler;
	memset(client,0,sizeof(uv_tcp_t));
	uv_tcp_init(uv_default_loop(),client);
	client->data = (void*)s;
	s->as_client = 1;
	s->socket_type = TCP_SOCKET;
	strcpy(s->c_address,server_ip);
	s->c_port = port;

	uv_connect_t* connect_req = (uv_connect_t*)malloc(sizeof(uv_connect_t));
	struct connect_cb* cb = (struct connect_cb*)malloc(sizeof(struct connect_cb));
	cb->on_connected = connected;
	cb->udata = udata;
	connect_req->data = (void*)cb;

	iret = uv_tcp_connect(connect_req,client,(struct sockaddr*)&bind_addr,after_connect);
	if(iret)
	{
		return;
	}

}

netbus::netbus()
{
	this->udp_handler = NULL;
}

void netbus::udp_send_to(char* ip,int port,unsigned char* body,int len)
{
	uv_buf_t w_buf;
	w_buf = uv_buf_init((char*)body,len);
	uv_udp_send_t* req = (uv_udp_send_t*)small_alloc(sizeof(uv_udp_send_t));
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	uv_udp_send(req,(uv_udp_t*)this->udp_handler,&w_buf,1,(const sockaddr*)&addr,on_uv_udp_send_end);
}