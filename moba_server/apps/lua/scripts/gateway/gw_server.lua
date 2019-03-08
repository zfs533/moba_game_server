
--{stype,ctype,utag,body}
function on_gw_recv_cmd_func(s,msg)
end

function on_gw_sesssion_disconnected(s)
    local ip,port = Session.get_address(s)
    Logger.debug(port,ip)
end

local gw_service = 
{
    on_session_recv_cmd = on_gw_recv_cmd_func,
    on_session_disconnect = on_gw_sesssion_disconnected,
}

return gw_server