#ifndef __MYSQL_WRAPPER_H
#define __MYSQL_WRAPPER_H

#include "mysql.h"

class mysql_wrapper
{
public:
	static void connect(char* ip,int port, char* db_name,char* uname, char* pwd,void(*open_cb)(const char* err,void* context,void* data),void* udata=NULL);
	static void close(void* context);
	static void query(void* context,char* sql,void(*query_cb)(const char* err,MYSQL_RES* result ,void* udata),void* udata);
};

#endif