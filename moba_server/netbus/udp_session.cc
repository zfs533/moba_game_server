#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>

#include "uv.h"
#include "session.h"
#include "udp_session.h"
#include "proto_man.h"
#include "../utils/logger.h"

#include "../utils/small_alloc.h"
#define my_malloc small_alloc
#define my_free small_free

using namespace std;

static void on_uv_udp_send_end(uv_udp_send_t* req,int status)
{
	if(status == 0)
	{
		log_debug("udp_send_success");
	}
	my_free(req);
}

void udp_session::send_data(unsigned char* body,int len)
{
	uv_buf_t w_buf;
	w_buf = uv_buf_init((char*)body,len);
	uv_udp_send_t* req = (uv_udp_send_t*)my_malloc(sizeof(uv_udp_send_t));
	uv_udp_send(req,this->udp_handler,&w_buf,1,this->addr,on_uv_udp_send_end);
}

const char* udp_session::get_address(int* client_port)
{
	*client_port = this->c_port;
	return this->c_address;
}

void udp_session::send_msg(struct cmd_msg* msg)
{
	unsigned char* encode_pkg = NULL;
	int encode_len = 0; 
	encode_pkg = proto_man::encode_msg_to_raw(msg,&encode_len);
	if(encode_pkg)
	{
		this->send_data(encode_pkg,encode_len);
		proto_man::msg_raw_free(encode_pkg);
	}
}

void udp_session::close()
{}