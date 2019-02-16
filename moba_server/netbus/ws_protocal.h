#ifndef __WS_PROTOCAL_H
#define __WS_PROTOCAL_H

class session;
class ws_protocol
{
public: 
	static bool ws_shake_hand(session* s,char* body, int len);
	static bool read_ws_header(unsigned char* pkg_data,int pkg_len,int* pkg_size,int* out_header_size);
	static void parser_ws_recv_data(unsigned char* raw_data, unsigned char* mask, int raw_len);
	static unsigned char* package_ws_send_data(const unsigned char* raw_data,int len,int* ws_data_len);
	static void free_ws_send_pkg(unsigned char* ws_pkg);
public:
	static void parseHttpRequest(const char* protolData);
};

#endif