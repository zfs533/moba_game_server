
local key = ""
function PrintTable(table , level)
  level = level or 1
  local indent = ""
  for i = 1, level do
    indent = indent.."  "
  end

  if key ~= "" then
    print(indent..key.." ".."=".." ".."{")
  else
    print(indent .. "{")
  end

  key = ""
  for k,v in pairs(table) do
     if type(v) == "table" then
        key = k
        PrintTable(v, level + 1)
     else
        local content = string.format("%s%s = %s", indent .. "  ",tostring(k), tostring(v))
      print(content)  
      end
  end
  print(indent .. "}")

end

------------------------------------------------------------------------------------
------------------------------------------------------------------------------------
------------------------------------------------------------------------------------
------------------------------------------------------------------------------------
-- package.path = "H:\\Apan_WorkAbout\\duokemeng\\mytest\\server\\moba_game_server\\moba_server\\lua_script\\?.lua;" ..package.path
-- local test = require "test"
-- 初始化日志模块
Logger.init("logger/talkroom/","talkroom",true);
-- 初始化协议模块(数据传输协议)
local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}
ProtoMan.init(proto_type.PROTO_BUF)

local cmd_name_map = require("cmd_name_map")
local trm_server = require("talkroom/trm_server")

--如果是protobuf协议，则注册一下映射表
if ProtoMan.proto_type() == proto_type.PROTO_BUF then
    if cmd_name_map then
        print("cmd nama map")
        PrintTable(cmd_name_map)
        ProtoMan.register_protobuf_cmd_map(cmd_name_map)
    end
end

Service.register(trm_server.stype,trm_server.service)
-- --开启网络服务

Netbus.tcp_listen(6080,"127.0.0.1")
Netbus.ws_listen(3000,"10.0.7.65")
Netbus.udp_listen(6081,"127.0.0.1")

print("start service success------------------")
