#ifndef __PROTO_MAN_H__
#define __PROTO_MAN_H__

#include <string>
#include <map>
#include "google/protobuf/message.h"

enum 
{
	PROTO_JSON = 0,
	PROTO_BUF = 1,
};

struct cmd_msg
{
	int stype; //service tag
	int ctype; //commond tag
	unsigned int utag;
	void* body; // JSON str ªÚ’ﬂ «message;
};

class proto_man 
{
public:
	static void init(int proto_type);
	static void register_protobuf_cmd_map(std::map<int,std::string> &map);
	static const char* protobuf_cmd_name(int ctype);
	static int proto_type();

	static bool decode_cmd_msg(unsigned char* cmd, int cmd_len, struct cmd_msg** out_msg);
	static void cmd_msg_free(struct cmd_msg* msg);

	static unsigned char* encode_msg_to_raw(const struct cmd_msg* msg, int* out_len);
	static void msg_raw_free(unsigned char* raw);

	static google::protobuf::Message* create_message(const char* type_name);
	static void release_message(google::protobuf::Message* msg);
};


#endif
