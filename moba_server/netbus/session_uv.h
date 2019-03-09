#ifndef __SESSION_UV_H
#define __SESSION_UV_H


#define RECV_LEN 4096
//enum server type
enum
{
	TCP_SOCKET,
	WS_SOCKET,
};
class uv_session:public session
{
public:
	uv_tcp_t tcp_handler;//客户端键入接入进来的句柄
	char c_address[32];
	int c_port;

	uv_shutdown_t shutdown;
	bool is_shutdown;//要标记一下session,避免重复uv_shutdown
public:
	char recv_buf[RECV_LEN];
	int recved;
	int socket_type;
	char* long_pkg;
	int long_pkg_size;
public:
	bool is_ws_shaked;
private:
	void init();
	void exit();
public:
	static uv_session* create();
	static void destroy(uv_session* s);
public:
	virtual void close();
	virtual void send_data(unsigned char* body,int len);
	virtual const char* get_address(int* client_port);
	virtual void send_msg(struct cmd_msg* msg);
	virtual void send_raw_cmd(struct raw_cmd* msg);
};
void init_session_allocer();
#endif