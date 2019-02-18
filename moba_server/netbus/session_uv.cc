#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>

#include "uv.h"
#include "session.h"
#include "session_uv.h"
#include "../utils/cacke_alloc.h"
#include "ws_protocal.h"
#include "tp_protocol.h"

using namespace std;

#define SESSION_CACHE_CAPACITY 6000
#define WQ_CACHE_CAPACITY 4096

#define WBUF_CACHE_CAPCITY 2048
#define CMD_CACHE_SIZE 2048

struct cache_allocer* session_allocer = NULL;
static cache_allocer* wr_allocer = NULL;
cache_allocer* wbuf_allocer = NULL;//tp_protocol
//初始化session_uv内存池
void init_session_allocer()
{
	if(session_allocer == NULL)
	{
		session_allocer = create_cache_allocer(SESSION_CACHE_CAPACITY,sizeof(uv_session));
	}
	if(wr_allocer == NULL)
	{
		wr_allocer = create_cache_allocer(WQ_CACHE_CAPACITY,sizeof(uv_write_t));
	}
	if(wbuf_allocer == NULL)
	{
		wbuf_allocer = create_cache_allocer(WBUF_CACHE_CAPCITY,CMD_CACHE_SIZE);
	}
}

extern "C"
{
	static void after_write(uv_write_t* req, int status)
	{
		if(status == 0)
		{
			printf("write success\n");
		}
		cache_free(wr_allocer,req);
	}
	static void on_close(uv_handle_t* handle)
	{
		uv_session* s = (uv_session*)handle->data;
		uv_session::destroy(s);
	}
	static void on_shutdown(uv_shutdown_t* req,int status)
	{
		uv_close((uv_handle_t*)req->handle,on_close);
	}
}

uv_session* uv_session::create()
{
	uv_session* uv_s = (uv_session*)cache_alloc(session_allocer,sizeof(uv_session));//从内存池中分配一块内存
	uv_s->uv_session::uv_session();//兼容c/c++，因为c++中的new会默认调用类构造函数，这里显示调用一下构造函数
	uv_s->init();
	return uv_s;
}

void uv_session::destroy(uv_session* s)
{
	printf("client close\n");
	s->exit();
	s->uv_session::~uv_session();
	cache_free(session_allocer,s);
}
void uv_session::init()
{
	memset(this->c_address,0,sizeof(this->c_address));
	this->c_port = 0;
	this->recved = 0;
	this->is_shutdown = false;
	this->is_ws_shaked = false;
}
void uv_session::exit()
{

}

void uv_session::close()
{
	if(this->is_shutdown)
	{
		return;
	}
	this->is_shutdown = true;
	uv_shutdown_t* reg = &this->shutdown;
	memset(reg,0,sizeof(uv_shutdown_t));
	uv_shutdown(reg,(uv_stream_t*)&this->tcp_handler,on_shutdown);
}

void uv_session::send_data(unsigned char* body,int len)
{
	uv_write_t* w_req = (uv_write_t*)cache_alloc(wr_allocer,sizeof(uv_write_t));
	uv_buf_t w_buf ;
	if(this->socket_type == WS_SOCKET )
	{
		//websocket
		if(this->is_ws_shaked)
		{
			int ws_pkg_len;
			unsigned char* ws_pkg = ws_protocol::package_ws_send_data(body,len,&ws_pkg_len);
			w_buf = uv_buf_init((char*)ws_pkg,ws_pkg_len);
			uv_write(w_req,(uv_stream_t*)&this->tcp_handler,&w_buf,1,after_write);
			ws_protocol::free_ws_send_pkg(ws_pkg);
		}
		else
		{
			w_buf = uv_buf_init((char*)body,len);
			uv_write(w_req,(uv_stream_t*)&this->tcp_handler,&w_buf,1,after_write);
		}
	}
	else
	{
		//tcp socket
		int tp_pkg_len;
		unsigned char* tp_pkg = tp_protocol::package(body,len,&tp_pkg_len);
		w_buf = uv_buf_init((char*)tp_pkg,tp_pkg_len);
		uv_write(w_req,(uv_stream_t*)&this->tcp_handler,&w_buf,1,after_write);
		tp_protocol::release_package(tp_pkg);
	}
}

const char* uv_session::get_address(int* port)
{
	*port = this->c_port;
	return this->c_address;
}


// c++ new 分配内存空间的同时会调用类的构造函数
// c malloc不会调用类实例的构造函数
// 要编写c/c++兼容的内存池管理 则用malloc和类::类构造函数，来编写，不用new delete
