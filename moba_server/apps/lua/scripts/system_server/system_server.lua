
local Stype = require("Stype")
local Cmd = require("Ctype")
local utils = require("utils")

local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}
local ugame = require("system_server/ugame")
local login_bonues = require("system_server/login_bonues")
local game_rank = require("system_server/game_rank")
local sys_msg = require("system_server/sys_msg")

local system_service_handlers = {}
system_service_handlers[Cmd.eGetUgameInfoReq] = ugame.get_ugame_info
system_service_handlers[Cmd.eRecvLoginBonuesReq] = login_bonues.recv_login_bonues
system_service_handlers[Cmd.eGetWorldRankUchipReq] = game_rank.get_world_uchip_rank_info
system_service_handlers[Cmd.eGetSystemMessageReq] = sys_msg.get_sys_message
--{stype,ctype,utag,body}
local function on_system_recv_cmd(s,msg)
    Logger.debug("---------------------------recv_system_cmd")
    utils.print_tb(msg)
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