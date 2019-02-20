#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#ifdef WIN32
//#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libmysql.lib")
//#endif

#include "uv.h"
#include "mysql.h"
#include "mysql_wrapper.h"

#define my_malloc malloc
#define my_free free

struct connect_req
{
	char* ip;
	int port;
	char* db_name;
	char* uname;
	char* upwd;
	void(*open_cb)(const char* err,void* content);
	char* err;
	void* context;
};

struct mysql_context
{
	void* pConn;//mysql handle
	uv_mutex_t lock;
	int is_closed;
};

static void connect_work(uv_work_t* req)
{
	struct connect_req* r = (struct connect_req*)req->data;
	MYSQL* pConn = mysql_init(NULL);
	if(mysql_real_connect(pConn,r->ip,r->uname,r->upwd,r->db_name,r->port,NULL,0))
	{//connect success
		struct mysql_context* c = (struct mysql_context*)my_malloc(sizeof(struct mysql_context));
		memset(c,0,sizeof(struct mysql_context));
		c->pConn = pConn;
		uv_mutex_init(&c->lock);
		r->context = c;
		r->err = NULL;
	}
	else
	{
		r->context = NULL;
		r->err = strdup(mysql_error(pConn));
	}
}

static void on_connect_complete(uv_work_t* req, int status)
{
	struct connect_req* r = (struct connect_req*)req->data;
	r->open_cb(r->err,r->context);
	if(r->ip)
	{
		free(r->ip);
	}
	if(r->uname)
	{
		free(r->uname);
	}
	if(r->db_name)
	{
		free(r->db_name);
	}
	if(r->upwd)
	{
		free(r->upwd);
	}
	if(r->err)
	{
		free(r->err);
	}
	my_free(r);
	my_free(req);
}


void mysql_wrapper::connect(char* ip,int port, char* db_name,char* uname, char* pwd,void(*open_cb)(const char* err,void* context))
{
	uv_work_t* w = (uv_work_t*)my_malloc(sizeof(uv_work_t));
	memset(w,0,sizeof(uv_work_t));
	struct connect_req* r = (struct connect_req*)my_malloc(sizeof(struct connect_req));
	memset(r,0,sizeof(struct connect_req));
	r->ip = strdup(ip);
	r->port = port;
	r->db_name = strdup(db_name);
	r->uname = strdup(uname);
	r->upwd = strdup(pwd);
	r->open_cb = open_cb;
	w->data = (void*)r;
	//start one queue work, one thread
	uv_queue_work(uv_default_loop(),w,connect_work,on_connect_complete);
}



static void close_work(uv_work_t* req)
{
	struct mysql_context* r = (struct mysql_context*)req->data;
	uv_mutex_lock(&r->lock);
	MYSQL* pConn = (MYSQL*)r->pConn;
	mysql_close(pConn);
	uv_mutex_unlock(&r->lock);
}

static void on_close_complete(uv_work_t* req, int status)
{
	struct mysql_context* r = (struct mysql_context*)req->data;
	my_free(r);
	my_free(req);
}

void mysql_wrapper::close(void* context)
{
	struct mysql_context* c = (struct mysql_context*)context;
	if(c->is_closed)
	{
		return;
	}
	uv_work_t* w = (uv_work_t*)my_malloc(sizeof(uv_work_t));
	memset(w,0,sizeof(uv_work_t));
	w->data = context;
	c->is_closed = 1;
	uv_queue_work(uv_default_loop(),w,close_work,on_close_complete);
}

struct query_req
{
	void* context;
	char* sql;
	void(*query_cb)(const char* err,std::vector<std::vector<std::string>>* result);
	char* err;
	std::vector<std::vector<std::string>>* result;
};

static void query_work(uv_work_t* req) 
{
	query_req* r = (query_req*)req->data;
	struct mysql_context* my_conn = (struct mysql_context*)(r->context);
	uv_mutex_lock(&my_conn->lock);
	MYSQL* pConn = (MYSQL*)my_conn->pConn;
	int ret = mysql_query(pConn, r->sql);//execute the command
	if (ret != 0) 
	{
		r->err = strdup(mysql_error(pConn));
		r->result = NULL;
		uv_mutex_unlock(&my_conn->lock);
		return;
	}

	r->err = NULL;
	MYSQL_RES *result = mysql_store_result(pConn);//select result
	if (!result) 
	{
		r->result = NULL;
		return;
	}
	MYSQL_ROW row;

	r->result = new std::vector<std::vector<std::string>>;
	int num = mysql_num_fields(result);//fields num
	std::vector<std::string> empty;

	std::vector<std::vector<std::string>>::iterator end_elem;
	while (row = mysql_fetch_row(result)) 
	{
		r->result->push_back(empty);
		end_elem = r->result->end() - 1;
		for (int i = 0; i < num; i++) 
		{
			end_elem->push_back(row[i]);
		}
	}
	mysql_free_result(result);
	uv_mutex_unlock(&my_conn->lock);
}

static void on_query_complete(uv_work_t* req, int status) 
{
	query_req* r = (query_req*)req->data;
	r->query_cb(r->err, r->result);

	if (r->sql) 
	{
		free(r->sql);
	}
	if (r->result) 
	{
		delete r->result;
	}
	if (r->err) 
	{
		free(r->err);
	}
	my_free(r);
	my_free(req);
}

void mysql_wrapper::query(void* context,char* sql,void(*query_cb)(const char* err,std::vector<std::vector<std::string>>* result))
{
	struct mysql_context* c = (struct mysql_context*)context;
	if(c->is_closed)
	{
		return;
	}
	uv_work_t* w = (uv_work_t*)my_malloc(sizeof(uv_work_t));
	memset(w,0,sizeof(uv_work_t));
	query_req* r = (query_req*)my_malloc(sizeof(query_req));
	memset(r,0,sizeof(query_req));
	r->context = context;
	r->sql = strdup(sql);
	r->query_cb = query_cb;
	w->data = r;

	uv_queue_work(uv_default_loop(),w,query_work,on_query_complete);
}
