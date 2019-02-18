#ifndef __TP_PROTOCOL_H
#define __TP_PROTOCOL_H
//compare with websocket protocol
class tp_protocol
{
public:
	static bool read_header(unsigned char* data,int data_len,int* pkg_size,int* out_header_size);
	static unsigned char* package(const unsigned char* raw_data,int len, int* pkg_len);
	static void release_package(unsigned char* tp_pkg);
};

#endif