local TableToStr = require("utils").table_to_str
local utils = require("utils")
local Stype = require("Stype")
local Cmd = require("Ctype")

local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}

local game_mgr = require("logic_server/game_mgr")

local logic_service_handlers = {}
logic_service_handlers[Cmd.eLoginLogicReq] = game_mgr.login_logic_server
logic_service_handlers[Cmd.eUserLostConn] = game_mgr.client_disconnect--玩家掉线
logic_service_handlers[Cmd.eEnterZoneReq] = game_mgr.enter_zone
logic_service_handlers[Cmd.eExitMatchReq] = game_mgr.exit_match_list
logic_service_handlers[Cmd.eUdpTest] = game_mgr.do_udp_test
logic_service_handlers[Cmd.eNextFrameOpts] = game_mgr.next_frame_opt

--{stype,ctype,utag,body}
local function on_logic_recv_cmd(s,msg)
    Logger.debug("---------------------------recv_logic_cmd")
    utils.print_tb(msg)
    if ProtoMan.proto_type() == proto_type.PROTO_BUF then
        if logic_service_handlers[msg[2]] then
            logic_service_handlers[msg[2]](s,msg)
        end
    else
        -- print(msg[1],msg[2],msg[3],msg[4])
        -- local msg = {Stype.logic,Cmd.eOnSendMsg,msg[3],msg[4]}
        -- -- local msg = {Stype.logic,Cmd.eOnSendMsg,msg[3],TableToStr({ip=111,port=111,content="qweqwe"})}
        -- Session.send_msg(s,msg)
    end
end

local function on_logic_session_disconnect(s,stype)
    game_mgr.gatway_disconnect(s,stype)
end

local function on_logic_session_connect(s,stype)
    game_mgr.gatway_connect(s,stype)
end

local logic_server = 
{
    on_session_recv_cmd = on_logic_recv_cmd,
    on_session_disconnect = on_logic_session_disconnect,
    on_session_connect = on_logic_session_connect,
}

return logic_server