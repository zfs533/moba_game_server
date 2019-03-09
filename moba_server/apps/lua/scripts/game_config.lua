--[[

]]
local Stype = require("Stype")

local remote_servers = {}
--注册我们的服务所部属的IP地址和端口
remote_servers[Stype.Auth] = 
{
	stype = Stype.Auth,
	ip = "127.0.0.1",
	port = 8000,
	desc = "Auth server",
}
--[[
remote_servers[Stype.System] = 
{
	stype = Stype.System,
	ip = "127.0.0.1",
	port = 8001,
	desc = "System server"
}
]]

local game_config = 
{
	gateway_tcp_ip = "127.0.0.1",
	gateway_tcp_port = 6080,

	gateway_ws_ip = "10.0.7.65",
	gateway_ws_port = 3000,

	servers = remote_servers,
}

return game_config

