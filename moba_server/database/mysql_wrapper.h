#ifndef __MYSQL_WRAPPER_H
#define __MYSQL_WRAPPER_H

#include <vector>
#include <string>

class mysql_wrapper
{
public:
	static void connect(char* ip,int port, char* db_name,char* uname, char* pwd,void(*open_cb)(const char* err,void* context));
	static void close(void* context);
	static void query(void* context,char* sql,void(*query_cb)(const char* err,std::vector<std::vector<std::string>>* result));
};

#endif