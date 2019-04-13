#ifndef __SERVICE_H
#define __SERVICE_H

#include "session.h"
#include "proto_man.h"

class service
{
public :
	bool using_raw_cmd;
	service();
public:
	virtual bool on_session_recv_cmd(session* s,struct cmd_msg* msg);
	virtual void on_session_disconnect(session* s,int stype);
	virtual void on_session_connect(session* s, int stype);
	//register transpond service (gateway server)
	virtual bool on_session_recv_raw_cmd(session* s,struct raw_cmd* raw);

};

#endif