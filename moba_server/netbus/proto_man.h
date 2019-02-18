#ifndef __PROTO_MAN_H__
#define __PROTO_MAN_H__

enum 
{
	PROTO_JSON = 0,
	PROTO_BUF = 1,
};

struct cmd_msg
{
	int stype; 
	int ctype; 
	unsigned int utag;
	void* body; // JSON str ªÚ’ﬂ «message;
};

class proto_man 
{
public:
	static void init(int proto_type);
	static void register_pf_cmd_map(char** pf_map, int len);
	static int proto_type();

	static bool decode_cmd_msg(unsigned char* cmd, int cmd_len, struct cmd_msg** out_msg);
	static void cmd_msg_free(struct cmd_msg* msg);

	static unsigned char* encode_msg_to_raw(const struct cmd_msg* msg, int* out_len);
	static void msg_raw_free(unsigned char* raw);
};


#endif
