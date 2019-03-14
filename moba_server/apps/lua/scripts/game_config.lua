--[[

]]
local Stype = require("Stype")

local remote_servers = {}
--注册我们的服务所部属的IP地址和端口
remote_servers[Stype.Auth] = 
{
	stype = Stype.Auth,
	ip = "127.0.0.1",
	port = 8012,
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
	auth_mysql = {
		host = "127.0.0.1",--数据库所在的host
		port = 3306,--数据库所在的端口
		db_name = "auth_center",--数据库名称
		uname = "root",--登录数据库账号
		upwd = "123456",--登录数据库密码
	}
}

return game_config

