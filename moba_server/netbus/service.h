#ifndef __SERVICE_H
#define __SERVICE_H

#include "session.h"
#include "proto_man.h"

class service
{
public:
	virtual bool on_session_recv_cmd(session* s,struct cmd_msg* msg);
	virtual void on_session_disconnect(session* s);
};

#endif