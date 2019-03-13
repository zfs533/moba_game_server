function ToStringEx(value)
    if type(value)=='table' then
       return TableToStr(value)
    elseif type(value)=='string' then
        return "\""..value.."\""
    else
       return tostring(value)
    end
end

function TableToStr(t)
    if t == nil then return "" end
    local retstr= "{"

    local i = 1
    for key,value in pairs(t) do
        local signal = ","
        if i==1 then
          signal = ""
        end

        if key == i then
            retstr = retstr..signal..ToStringEx(value)
        else
            if type(key)=='number' or type(key) == 'string' then
                retstr = retstr..signal..''..ToStringEx(key).."="..ToStringEx(value)
            else
                if type(key)=='userdata' then
                    retstr = retstr..signal.."*s"..TableToStr(getmetatable(key)).."*e".."="..ToStringEx(value)
                else
                    retstr = retstr..signal..key.."="..ToStringEx(value)
                end
            end
        end

        i = i+1
    end

     retstr = retstr.."}"
     return retstr
end
-------------------------------------------------------------------------


local Stype = require("Stype")
local Cmd = require("Ctype")
local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}
--{stype,ctype,utag,body}
function on_auth_recv_cmd(s,msg)
    Logger.debug("auth_recv_cmd..")
    if ProtoMan.proto_type() == proto_type.PROTO_BUF then
        Logger.debug(msg[1], msg[2], msg[3])
        local res_msg = {Stype.Auth,Cmd.eLoginRes,msg[3],{status = 300}}
        Session.send_msg(s,res_msg)
    else
        print(msg[1],msg[2],msg[3],msg[4])
        local msg = {Stype.Auth,Cmd.eOnSendMsg,msg[3],msg[4]}
        -- local msg = {Stype.Auth,Cmd.eOnSendMsg,msg[3],TableToStr({ip=111,port=111,content="qweqwe"})}
        Session.send_msg(s,msg)
    end
end

function on_auth_session_disconnect(s,stype)
    local ip,port = Session.get_address(s)
    Logger.debug(port,ip)
end

local auth_server = 
{
    on_session_recv_cmd = on_auth_recv_cmd,
    on_session_disconnect = on_auth_session_disconnect,
}

return auth_server