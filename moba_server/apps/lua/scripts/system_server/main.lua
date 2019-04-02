local game_config = require("game_config")
local servers = game_config.servers
local Stype = require("Stype")
--[[system server]]
-- 初始化日志模块
Logger.init("logger/system_server/","system_server",true);

--连接到我们的system_center database mysql 数据库
require("database/mysql_game")

-- 初始化协议模块(数据传输协议)
local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}
ProtoMan.init(proto_type.PROTO_BUF)

local cmd_name_map = require("cmd_name_map")
local system_server = require("system_server/system_server")

--如果是protobuf协议，则注册一下映射表
if ProtoMan.proto_type() == proto_type.PROTO_BUF then
    if cmd_name_map then
        ProtoMan.register_protobuf_cmd_map(cmd_name_map)
    end
end


-- --开启网关端口监听
Netbus.tcp_listen(servers[Stype.System].port,servers[Stype.System].ip)

--register service

  local ret = Service.register(Stype.System,system_server)
  if ret then
    Logger.debug("register System:[" .. Stype.System.. "] success!!!")
  else
    Logger.debug("register System:[" .. Stype.System.. "] failed!!!")
  end

print(TimeStamp.timestamp())
print(TimeStamp.timestamp_today())
print(TimeStamp.timestamp_yesterday())