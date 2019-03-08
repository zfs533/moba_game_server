--[[user center server]]
-- 初始化日志模块
Logger.init("logger/auth_server/","auth_server",true);
-- 初始化协议模块(数据传输协议)
local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}
ProtoMan.init(proto_type.PROTO_BUF)

local cmd_name_map = require("cmd_name_map")
local auth_server = require("auth_server/auth_server")

--如果是protobuf协议，则注册一下映射表
if ProtoMan.proto_type() == proto_type.PROTO_BUF then
    if cmd_name_map then
        ProtoMan.register_protobuf_cmd_map(cmd_name_map)
    end
end

local game_config = require("game_config")
local servers = game_config.servers
local Stype = require("Stype")
-- --开启网关端口监听
Netbus.tcp_listen(servers[Stype.Auth].port,servers[Stype.Auth].ip)

--register service

  local ret = Service.register(Stype.Auth,auth_server)
  if ret then
    Logger.debug("register auth_server:[" .. Stype.Auth.. "] success!!!")
  else
    Logger.debug("register auth_server:[" .. Stype.Auth.. "] failed!!!")
  end

