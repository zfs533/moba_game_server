#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <map>
#include "tolua_fix.h"
#include "../netbus/proto_man.h"
#include "proto_man_export_to_lua.h"
#include "google/protobuf/message.h"
#include "lua_wrapper.h"

using namespace google::protobuf;
using namespace std;

static int proto_man_init_tolua(lua_State* tolua_S)
{
	int type = tolua_tonumber(tolua_S,1,-1);
	if(type == -1)
	{
		goto lua_failed;
	}
	proto_man::init(type);
lua_failed:
	return 0;
}

static int proto_type_tolua(lua_State* tolua_S)
{
	int type = proto_man::proto_type();
	if(type != PROTO_BUF && type != PROTO_JSON)
	{
		goto lua_failed;
	}
	lua_pushinteger(tolua_S,type);
	
	return 1;
lua_failed:
	lua_pushnil(tolua_S);
	return 1;
}

static int register_protobuf_cmd_map_tolua(lua_State* tolua_S)
{
	map<int,string> mp;
	int len = luaL_len(tolua_S,1);
	for(int i = 1; i<=len; ++i)
	{
		lua_pushnumber(tolua_S,i);
		lua_gettable(tolua_S,1);
		const char* name = luaL_checkstring(tolua_S,-1);
		if(name)
		{
			mp[i] = name;
		}
		lua_pop(tolua_S,1);
	}
	proto_man::register_protobuf_cmd_map(mp);
lua_failed:
	return 0;
}

int register_proto_man_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S,"_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"ProtoMan",0);
		tolua_beginmodule(tolua_S,"ProtoMan");
		tolua_function(tolua_S,"init",proto_man_init_tolua);
		tolua_function(tolua_S,"proto_type",proto_type_tolua);
		tolua_function(tolua_S,"register_protobuf_cmd_map",register_protobuf_cmd_map_tolua);
		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,1);
	return 0;
}

static int raw_read_header_tolua(lua_State* tolua_S)
{
	int argc = lua_gettop(tolua_S);
	if(argc != 1)
	{
		goto lua_failed;
	}
	struct raw_cmd* raw = (struct raw_cmd*)tolua_touserdata(tolua_S,1,NULL);
	if(raw == NULL)
	{
		goto lua_failed;
	}
	lua_pushinteger(tolua_S,raw->stype);
	lua_pushinteger(tolua_S,raw->ctype);
	lua_pushinteger(tolua_S,raw->utag);
	return 3;
lua_failed:
	return 0;
}

static int raw_set_utag_tolua(lua_State* tolua_S)
{
	int argc = lua_gettop(tolua_S);
	if(argc!=2)
	{
		goto lua_failed;
	}
	struct raw_cmd* raw = (struct raw_cmd*)tolua_touserdata(tolua_S,1,NULL);
	if(raw == NULL)
	{
		goto lua_failed;
	}
	unsigned int utag = (unsigned int)luaL_checkinteger(tolua_S,2);
	raw->utag = utag;
	
	unsigned char* utag_ptr = raw->raw_cmd_cmd + 4;
	utag_ptr[0] = (utag & 0x000000FF);
	utag_ptr[1] = ((utag & 0x0000FF00) >> 8);
	utag_ptr[2] = ((utag & 0x00FF0000) >> 16);
	utag_ptr[3] = ((utag & 0xFF000000) >> 24);
	
	return 0;
lua_failed:
	return 0;
}


static void push_proto_message_tolua(const Message* message) 
{
	lua_State* state = lua_wrapper::lua_state();
	if (!message) {
		// printf("PushProtobuf2LuaTable failed, message is NULL");
		return;
	}
	const Reflection* reflection = message->GetReflection();

	// new a table
	lua_newtable(state);

	const Descriptor* descriptor = message->GetDescriptor();
	for (int32_t index = 0; index < descriptor->field_count(); ++index) {
		const FieldDescriptor* fd = descriptor->field(index);
		const std::string& name = fd->lowercase_name();

		// key
		lua_pushstring(state, name.c_str());

		bool bReapeted = fd->is_repeated();

		if (bReapeted) {
			// repeatedÕâ²ãµÄtable
			lua_newtable(state);
			int size = reflection->FieldSize(*message, fd);
			for (int i = 0; i < size; ++i) {
				char str[32] = { 0 };
				switch (fd->cpp_type()) {
				case FieldDescriptor::CPPTYPE_DOUBLE:
					lua_pushnumber(state, reflection->GetRepeatedDouble(*message, fd, i));
					break;
				case FieldDescriptor::CPPTYPE_FLOAT:
					lua_pushnumber(state, (double)reflection->GetRepeatedFloat(*message, fd, i));
					break;
				case FieldDescriptor::CPPTYPE_INT64:
					sprintf(str, "%lld", (long long)reflection->GetRepeatedInt64(*message, fd, i));
					lua_pushstring(state, str);
					break;
				case FieldDescriptor::CPPTYPE_UINT64:

					sprintf(str, "%llu", (unsigned long long)reflection->GetRepeatedUInt64(*message, fd, i));
					lua_pushstring(state, str);
					break;
				case FieldDescriptor::CPPTYPE_ENUM: 
					lua_pushinteger(state, reflection->GetRepeatedEnum(*message, fd, i)->number());
					break;
				case FieldDescriptor::CPPTYPE_INT32:
					lua_pushinteger(state, reflection->GetRepeatedInt32(*message, fd, i));
					break;
				case FieldDescriptor::CPPTYPE_UINT32:
					lua_pushinteger(state, reflection->GetRepeatedUInt32(*message, fd, i));
					break;
				case FieldDescriptor::CPPTYPE_STRING:
				{
					std::string value = reflection->GetRepeatedString(*message, fd, i);
					lua_pushlstring(state, value.c_str(), value.size());
				}
					break;
				case FieldDescriptor::CPPTYPE_BOOL:
					lua_pushboolean(state, reflection->GetRepeatedBool(*message, fd, i));
					break;
				case FieldDescriptor::CPPTYPE_MESSAGE:
					push_proto_message_tolua(&(reflection->GetRepeatedMessage(*message, fd, i)));
					break;
				default:
					break;
				}

				lua_rawseti(state, -2, i + 1); // lua's index start at 1
			}

		}
		else {
			char str[32] = { 0 };
			switch (fd->cpp_type()) {

			case FieldDescriptor::CPPTYPE_DOUBLE:
				lua_pushnumber(state, reflection->GetDouble(*message, fd));
				break;
			case FieldDescriptor::CPPTYPE_FLOAT:
				lua_pushnumber(state, (double)reflection->GetFloat(*message, fd));
				break;
			case FieldDescriptor::CPPTYPE_INT64:

				sprintf(str, "%lld", (long long)reflection->GetInt64(*message, fd));
				lua_pushstring(state, str);
				break;
			case FieldDescriptor::CPPTYPE_UINT64:

				sprintf(str, "%llu", (unsigned long long)reflection->GetUInt64(*message, fd));
				lua_pushstring(state, str);
				break;
			case FieldDescriptor::CPPTYPE_ENUM: 
				lua_pushinteger(state, (int)reflection->GetEnum(*message, fd)->number());
				break;
			case FieldDescriptor::CPPTYPE_INT32:
				lua_pushinteger(state, reflection->GetInt32(*message, fd));
				break;
			case FieldDescriptor::CPPTYPE_UINT32:
				lua_pushinteger(state, reflection->GetUInt32(*message, fd));
				break;
			case FieldDescriptor::CPPTYPE_STRING:
			{
				std::string value = reflection->GetString(*message, fd);
				lua_pushlstring(state, value.c_str(), value.size());
			}
				break;
			case FieldDescriptor::CPPTYPE_BOOL:
				lua_pushboolean(state, reflection->GetBool(*message, fd));
				break;
			case FieldDescriptor::CPPTYPE_MESSAGE:
				push_proto_message_tolua(&(reflection->GetMessage(*message, fd)));
				break;
			default:
				break;
			}
		}

		lua_rawset(state, -3);
	}
}

static int read_body_tolua(lua_State* tolua_S)
{
	int argc = lua_gettop(tolua_S);
	if(argc != 1)
	{
		goto lua_failed;
	}
	struct raw_cmd* raw = (struct raw_cmd*)tolua_touserdata(tolua_S,1,NULL);
	if(raw == NULL)
	{
		goto lua_failed;
	}
	struct cmd_msg* msg;
	if(proto_man::decode_cmd_msg(raw->raw_cmd_cmd,raw->raw_len,&msg))
	{
		if(msg->body == NULL)
		{
			lua_pushnil(tolua_S);
		}
		else if(proto_man::proto_type() == PROTO_JSON)
		{
			lua_pushstring(tolua_S,(const char*)msg->body);
		}
		else
		{
			push_proto_message_tolua((Message*)msg->body);
		}
		proto_man::cmd_msg_free(msg);
	}
	return 1;

lua_failed:
	return 0;
}

int register_raw_cmd_export(lua_State* tolua_S)
{
	lua_getglobal(tolua_S, "_G");
	if(lua_istable(tolua_S,-1))
	{
		tolua_open(tolua_S);
		tolua_module(tolua_S,"RawCmd",0);
		tolua_beginmodule(tolua_S,"RawCmd");

		tolua_function(tolua_S,"read_header",raw_read_header_tolua);
		tolua_function(tolua_S,"set_utag",raw_set_utag_tolua);
		tolua_function(tolua_S,"read_body",read_body_tolua);

		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S,1);
	return 0;
}