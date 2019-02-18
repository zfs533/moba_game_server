#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
using namespace std;


#include "tp_protocol.h"

//parse head information
bool tp_protocol::read_header(unsigned char* data,int data_len,int* pkg_size,int* out_header_size)
{
	if(data_len<2)
	{
		return false;
	}
	*pkg_size = (data[0] | (data[1] << 8));
	*out_header_size = 2;
	return true;
}
//package data
unsigned char* tp_protocol::package(const unsigned char* raw_data,int len, int* pkg_len)
{
	int head_size = 2;
	//cache malloc
	*pkg_len = (head_size + len);
	unsigned char* data_buf = (unsigned char*)malloc(head_size+len);
	data_buf[0] = (unsigned char)((*pkg_len) & 0x000000ff);
	data_buf[1] = (unsigned char)(((*pkg_len) & 0x0000ff00) >> 8);
	memcpy(data_buf + head_size,raw_data,len);

	return data_buf;
}

//free data
void tp_protocol::release_package(unsigned char* tp_pkg)
{
	free(tp_pkg);
}