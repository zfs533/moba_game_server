local TableToStr = require("utils").table_to_str
local utils = require("utils")

local Stype = require("Stype")
local Cmd = require("Ctype")
local guest = require("auth_server/guest")
local edit_profile = require("auth_server/edit_profile")
local account_upgrade = require("auth_server/account_upgrade")
local uname_login = require("auth_server/uname_login")
local login_out = require("auth_server/login_out")

local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}

local auth_service_handlers = {}
auth_service_handlers[Cmd.eGuestLoginReq] = guest.login
auth_service_handlers[Cmd.eUnameLoginReq] = uname_login.login
auth_service_handlers[Cmd.eEditProfileReq]= edit_profile.do_edit_profile
auth_service_handlers[Cmd.eAccountUpgradeReq]= account_upgrade.do_upgrade
auth_service_handlers[Cmd.eLoginOutReq] = login_out.login_out

--{stype,ctype,utag,body}
function on_auth_recv_cmd(s,msg)
    Logger.debug("---------------------------recv_auth_cmd")
    utils.print_tb(msg)
    if ProtoMan.proto_type() == proto_type.PROTO_BUF then
        if auth_service_handlers[msg[2]] then
            auth_service_handlers[msg[2]](s,msg)
        end
    else
        -- print(msg[1],msg[2],msg[3],msg[4])
        -- local msg = {Stype.Auth,Cmd.eOnSendMsg,msg[3],msg[4]}
        -- -- local msg = {Stype.Auth,Cmd.eOnSendMsg,msg[3],TableToStr({ip=111,port=111,content="qweqwe"})}
        -- Session.send_msg(s,msg)
    end
end

function on_auth_session_disconnect(s,stype)
    local ip,port = Session.get_address(s)
    Logger.debug("auth_server",port,ip)
end

local auth_server = 
{
    on_session_recv_cmd = on_auth_recv_cmd,
    on_session_disconnect = on_auth_session_disconnect,
}

return auth_server