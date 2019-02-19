#ifndef __SERVICE_MAN_H
#define __SERVICE_MAN_H

class session;
class service;
struct cmd_msg;

class service_man
{
public:
	static void init();
	static bool register_service(int stype,service* s);
	static bool on_recv_cmd_msg(session* s, struct cmd_msg* msg);
	static void on_session_disconnect(session* s);
};

#endif