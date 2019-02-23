/*
#ifndef __REDIS_WRAPPER_H
#define __REDIS_WRAPPER_H

#include "hiredis.h"
class redis_wrapper
{
public:
	static void connect(char* ip,int port,void(*open_cb)(const char* err,void* context,void* udata),void* data = NULL);
	static void close_redis(void* context);
	static void query(void* context,char* cmd,void(*query_cb)(const char* err, redisReply* result,void* udata),void* udata = NULL);
};

#endif
*/