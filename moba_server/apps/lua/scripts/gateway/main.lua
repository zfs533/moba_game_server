
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
-- 初始化日志模块
Logger.init("logger/gateway/","gateway",true);
-- 初始化协议模块(数据传输协议)
local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}
ProtoMan.init(proto_type.PROTO_BUF)

local cmd_name_map = require("cmd_name_map")
local gw_service = require("gateway/gw_server")

--如果是protobuf协议，则注册一下映射表
if ProtoMan.proto_type() == proto_type.PROTO_BUF then
    if cmd_name_map then
        PrintTable(cmd_name_map)
        ProtoMan.register_protobuf_cmd_map(cmd_name_map)
    end
end

local game_config = require("game_config")
-- --开启网关端口监听
Netbus.tcp_listen(game_config.gateway_tcp_port,gateway_tcp_ip)
Netbus.ws_listen(game_config.gateway_ws_port,game_config.gateway_ws_ip)
-- Netbus.udp_listen(6081,"127.0.0.1")

--register service
local service = game_config.servers
for k , v in pairs(service) do
  local ret = Service.register_with_raw(v.stype,gw_service)
  if ret then
    Logger.debug("register gw_servce:[" .. v.stype.. "] success!!!")
  else
    Logger.debug("register gw_servce:[" .. v.stype.. "] failed!!!")
  end
end
