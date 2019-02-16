#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

#include "session.h"
#include "ws_protocal.h"
#include "../3rd/http_parser/http_parser.h"
#include "../3rd/crypto/md5.h"
#include "../3rd/crypto/sha1.h"
#include "../3rd/crypto/base64_encode_zfs.h"


using namespace std;

static http_parser parser;
static char value_sec_key[512];//保存报文中的Sec-WebSocket-Key对应的值,这个很总要
static int is_sec_key = 0;//是否接续到Sec-WebSocket-Key的标志
static int has_sec_key = 0;
static int is_shaker_ended = 0;

//用来握手加密的字符串
static char* wb_migic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
// base64(sha1(key + wb_migic))，加密后发给客户端的固定字符串
static char *wb_accept = "HTTP/1.1 101 Switching Protocols\r\n"
"Upgrade:websocket\r\n"
"Connection: Upgrade\r\n"
"Sec-WebSocket-Accept: %s\r\n"
"WebSocket-Protocol:chat\r\n\r\n";

//--------------------http_parser-------------------
//开始解析回调函数
static int on_message_begin(http_parser* p)
{
	printf("on_message_begin\n");
	return 0;
}
//解析到url的回调
static int on_url(http_parser*, const char *at, size_t length)
{
	static char url_buf[1024];
	strncpy_s(url_buf, at, length);
	url_buf[length] = 0;
	printf("url-> %s\n", url_buf);
	return 0;
}
//解析到状态的回调
static int on_status(http_parser*, const char *at, size_t length)
{
	static char url_buf[1024];
	strncpy_s(url_buf, at, length);
	url_buf[length] = 0;
	printf("status-> %s\n", url_buf);
	return 0;
}
//解析到headers列表中键值对的key的回调
static int on_header_field(http_parser*, const char *at, size_t length)
{
	if(strncmp(at,"Sec-WebSocket-Key",length) == 0)
	{
		is_sec_key = 1;//找到了,用这个字段来判断是否是websocket，握手的关键
	}
	else
	{
		is_sec_key = 0;
	}

	static char url_buf[1024];
	strncpy_s(url_buf, at, length);
	url_buf[length] = 0;
	printf("on_header_field-> %s\n", url_buf);
	return 0;
}
//解析到headers列表中键值对的value的回调
static int on_header_value(http_parser*, const char *at, size_t length)
{
	if(!is_sec_key)
	{
		return 0;
	}
	strncpy(value_sec_key,at,length);
	value_sec_key[length] = 0;
	has_sec_key = 1;
	return 0;
}
//解析header头列表中的键值对完成的回调
static int on_headers_complete(http_parser* p)
{
	printf("on_headers_complete\n");
	return 0;
}
//解析到body信息的回调
static int on_body(http_parser*, const char *at, size_t length)
{
	static char url_buf[1024];
	strncpy_s(url_buf, at, length);
	url_buf[length] = 0;
	printf("on_body-> %s\n", url_buf);
	return 0;
}
//解析完成回调
static int on_message_complete(http_parser* p)
{
	printf("on_message_complete\n");
	is_shaker_ended = 1;
	return 0;
}

static http_parser_settings setting = 
{
	on_message_begin,
	on_url,
	on_status,
	on_header_field,
	on_header_value,
	on_headers_complete,
	on_body,
	on_message_complete,
	/*
	on_chunk_header,
	on_chunk_complete,
	*/
};

//解析客户端请求报文
void ws_protocol::parseHttpRequest(const char* protolData)
{
	//初始化解析类型
	http_parser_init(&parser,HTTP_REQUEST);
	//开始解析
	http_parser_execute(&parser,&setting,protolData,strlen(protolData));
}

//----------------------------------------------------------------------------------------------------
//base64加密算法
#ifdef __cplusplus
extern "C" {
#endif

char* wc_base64_encode(const uint8_t* text, int sz, int* out_esz) {
	static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int encode_sz = (sz + 2) / 3 * 4;
	int SMALL_CHUNK = 256;
	char *buffer = NULL;
	if (encode_sz >= SMALL_CHUNK) {
		buffer = (char*)malloc(encode_sz + 1);
	}
	else {
		buffer = (char*)malloc(SMALL_CHUNK);
	}
	int i =0;
	int	j =0;
	for (i = 0; i<(int)sz - 2; i += 3) {
		uint32_t v = text[i] << 16 | text[i + 1] << 8 | text[i + 2];
		buffer[j] = encoding[v >> 18];
		buffer[j + 1] = encoding[(v >> 12) & 0x3f];
		buffer[j + 2] = encoding[(v >> 6) & 0x3f];
		buffer[j + 3] = encoding[(v)& 0x3f];
		j += 4;
	}
	int padding = sz - i;
	uint32_t v;
	switch (padding) {
	case 1:
		v = text[i];
		buffer[j] = encoding[v >> 2];
		buffer[j + 1] = encoding[(v & 3) << 4];
		buffer[j + 2] = '=';
		buffer[j + 3] = '=';
		break;
	case 2:
		v = text[i] << 8 | text[i + 1];
		buffer[j] = encoding[v >> 10];
		buffer[j + 1] = encoding[(v >> 4) & 0x3f];
		buffer[j + 2] = encoding[(v & 0xf) << 2];
		buffer[j + 3] = '=';
		break;
	}
	buffer[encode_sz] = 0;
	*out_esz = encode_sz;
	return buffer;
}
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------

bool ws_protocol::ws_shake_hand(session* s,char* body, int len)
{
	parseHttpRequest((const char*)body);
	if(has_sec_key && is_shaker_ended)//解析到了websocket里面的Sec-WebSocket-Key
	{
		//key+magic
		unsigned char key_migic[512];
		unsigned char sha1_key_magic[SHA1_DIGEST_SIZE];
		static char send_client[512];//要发给客户端的字符串
		int sha1_size;
		//key+migic，将连个字符串连接起来组成一个新的字符串
		sprintf((char*)&key_migic,"%s%s",value_sec_key,wb_migic);
		//sha1加密,并将加密后的字符串保存在sha1_key_magic中
		crypt_sha1(key_migic,strlen((char*)&key_migic),(unsigned char*)&sha1_key_magic,&sha1_size);
		int base64_len;
		char* base_buf = wc_base64_encode(sha1_key_magic, sha1_size, &base64_len);
		sprintf(send_client,wb_accept,base_buf);
		base64_encode_free(base_buf);
		//客户端发送握手信息
		s->send_data((unsigned char*)send_client,strlen(send_client));
		return true;
	}
	return false;
}
//解读包头信息
bool ws_protocol::read_ws_header(unsigned char* recv_data,int recv_len,int* pkg_size,int* out_header_size)
{
	unsigned char* data = recv_data;
	//头两位为固定字节（1000 0001或1000 0010)
	if(data[0] !=0x81 && data[0] != 0x82)//81字符串,82二进制数据
	{
		return false;
	}
	if(recv_len<2)
	{
		return false;
	}
	//包长度字节, 去掉最高位, 剩下7为得到一个整数(0, 127);125以内的长度直接表示就可以了；
    //126表示后面两个字节表示大小,127表示后面的8个字节是数据的长度;(高位存在低地址)
	unsigned int data_len = data[1] &0x0000007f;
	int head_size = 2;
	if(data_len == 126)
	{
		//后面两个字节表示的是数据的长度;data[2],data[3]
		data_len = data[3] | (data[2] <<8);
		head_size += 2;
		if(recv_len <head_size)
		{
			return false;
		}
	}
	else if(data_len == 127)
	{
		//后面8个字节表示数据的长度；2,3,4,5|6,7,8,9
		unsigned int low = data[5]|(data[4]<<8)|(data[3]<<16)|(data[2]<<24);
		unsigned int hight = data[9]|(data[8]<<8)|(data[7]<<16)|(data[6]<<24);
		data_len = low;
		head_size+=8;
		if(recv_len <head_size)
		{
			return false;
		}
	}
	head_size += 4;//4个mask掩码
	*pkg_size = data_len + head_size;
	*out_header_size = head_size;
	return true;
}
//解读recv data
void ws_protocol::parser_ws_recv_data(unsigned char* raw_data, unsigned char* mask, int raw_len)
{
	for(int i = 0; i<raw_len; i++)
	{
		raw_data[i] = raw_data[i]^mask[i%4];
	}

}
//发送数据包处理
unsigned char* ws_protocol::package_ws_send_data(const unsigned char* raw_data,int len,int* ws_data_len)
{
	int head_size = 2;
	if (len > 125 && len < 65536) {
		head_size += 2;
	}
	else if (len >= 65536) {
		head_size += 8;
		return NULL;
	}
	// cache malloc
	unsigned char* data_buf = (unsigned char*)malloc(head_size + len);
	data_buf[0] = 0x81;
	if (len <= 125) {
		data_buf[1] = len;
	}
	else if (len > 125 && len < 65536) {
		data_buf[1] = 126;
		data_buf[2] = (len & 0x0000ff00) >> 8;
		data_buf[3] = (len & 0x000000ff);
	}
	
	memcpy(data_buf + head_size, raw_data, len);
	*ws_data_len = (head_size + len);

	return data_buf;
}

void ws_protocol::free_ws_send_pkg(unsigned char* ws_pkg)
{
	//cacke free
	free(ws_pkg);
}