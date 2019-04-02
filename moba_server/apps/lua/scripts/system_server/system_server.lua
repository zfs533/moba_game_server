
local Stype = require("Stype")
local Cmd = require("Ctype")
local utils = require("utils")

local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}
local ugame = require("system_server/ugame")
local login_bonues = require("system_server/login_bonues")

local system_service_handlers = {}
system_service_handlers[Cmd.eGetUgameInfoReq] = ugame.get_ugame_info
system_service_handlers[Cmd.eRecvLoginBonuesReq] = login_bonues.recv_login_bonues
--{stype,ctype,utag,body}
local function on_system_recv_cmd(s,msg)
    Logger.debug("current system req cmd===================> "..msg[2])
    if ProtoMan.proto_type() == proto_type.PROTO_BUF then
        if system_service_handlers[msg[2]] then
            system_service_handlers[msg[2]](s,msg)
        end
    else
    end
end

local function on_system_session_disconnect(s,stype)
    local ip,port = Session.get_address(s)
    Logger.debug("system_server",port,ip)
end

local system_server = 
{
    on_session_recv_cmd = on_system_recv_cmd,
    on_session_disconnect = on_system_session_disconnect,
}

return system_server