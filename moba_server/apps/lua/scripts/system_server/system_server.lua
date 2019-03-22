
local Stype = require("Stype")
local Cmd = require("Ctype")
local utils = require("utils")

local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}

local system_service_handlers = {}
--{stype,ctype,utag,body}
function on_system_recv_cmd(s,msg)
    Logger.debug("current system req cmd===================> "..msg[2])
    if ProtoMan.proto_type() == proto_type.PROTO_BUF then
    else
    end
end

function on_system_session_disconnect(s,stype)
    local ip,port = Session.get_address(s)
    Logger.debug("system_server",port,ip)
end

local system_server = 
{
    on_session_recv_cmd = on_system_recv_cmd,
    on_session_disconnect = on_system_session_disconnect,
}

return system_server