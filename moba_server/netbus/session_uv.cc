#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>

#include "uv.h"
#include "session.h"
#include "session_uv.h"

using namespace std;

extern "C"
{
	static void after_write(uv_write_t* req, int status)
	{
		if(status == 0)
		{
			printf("write success\n");
		}
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
	uv_session* uv_s = new uv_session();
	uv_s->init();
	return uv_s;
}

void uv_session::destroy(uv_session* s)
{
	printf("client close\n");
	s->exit();
	delete s;
}
void uv_session::init()
{
	memset(this->c_address,0,sizeof(this->c_address));
	this->c_port = 0;
	this->recved = 0;
}
void uv_session::exit()
{

}

void uv_session::close()
{
	uv_shutdown_t* reg = &this->shutdown;
	memset(reg,0,sizeof(uv_shutdown_t));
	uv_shutdown(reg,(uv_stream_t*)&this->tcp_handler,on_shutdown);
}

void uv_session::send_data(unsigned char* body,int len)
{
	uv_write_t* w_req = &this->w_req;
	uv_buf_t* w_buf = &this->w_buf;
	*w_buf = uv_buf_init((char*)body,len);
	uv_write(w_req,(uv_stream_t*)&this->tcp_handler,w_buf,1,after_write);
}

const char* uv_session::get_address(int* port)
{
	*port = this->c_port;
	return this->c_address;
}

