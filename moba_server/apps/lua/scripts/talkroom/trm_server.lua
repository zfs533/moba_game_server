local session_set = {}
--广播
function broadcast_except(msg,excpt_session)
    for i = 1, #session_set do
        if excpt_session ~= session_set[i] then
            Session.send_msg(session_set[i],msg)
        end
    end
end

function on_recv_login_cmd(s)
    --当前是否已经在这个集合中，若果是，则返回已经在这个聊天室的提示
    for i = 1,#session_set do
        if s == session_set[i] then
            local msg = {1,2,0,{status = -1}}
            Session.send_msg(s,msg)
            return
        end
    end
    --加入到集合
    table.insert( session_set,s)
    local msg = {1,2,0,{status = 1}}--1表示登录成功
    Session.send_msg(s,msg);
    --广播
    local s_ip,s_port = Session.get_address(s)
    msg = {1,7,0,{ip = s_ip,port = s_port}}
    broadcast_except(msg,s)
end

function on_recv_exit_cmd(s)
    for i = 1,#session_set do
        if s == session_set[i] then
            table.remove( session_set,i)
            local msg = {1,4,0,{status = 1}}
            Session.send_msg(s,msg)

            local s_ip,s_port = Session.get_address(s)
            msg = {1,8,0,{ip = s_ip,port = s_port}}
            broadcast_except(msg,s)
            return
        end
    end
end

function on_recv_send_msg_cmd(s,str)
    for i = 1,#session_set do
        if s == session_set[i] then
            local msg = {1,6,0,{status = "1"}}
            Session.send_msg(s,msg)

            local s_ip,s_port = Session.get_address(s)
            msg = {1,9,0,{ip = s_ip,port = s_port,content = str}}
            broadcast_except(msg,s)
            return
        end
    end
    local msg = {1,6,0,{status = "-1"}}
    Session.send_msg(s,msg)
end
function on_recv_on_send_msg_cmd(s,body)
    print(body.ip,body.port,body.content)
    -- for i = 1, #session_set do
    --     if s == session_set[i] then
            local msg = {1,9,0,{ip=body.ip, port= body.port, content= "看看这个会不会乱码"}}
            Session.send_msg(s,msg)
    --     end
    -- end 
end
--{stype,ctype,utag,body}
function on_trm_recv_cmd_func(s,msg)
    local ctype = msg[2]
    local body = msg[4]
    if ctype == 1 then
        on_recv_login_cmd(s)
    elseif ctype == 3 then
        on_recv_exit_cmd(s)
    elseif ctype == 5 then
        on_recv_send_msg_cmd(s,body.content)
    elseif ctype == 9 then
        on_recv_on_send_msg_cmd(s,body)
    end
end

function on_trm_sesssion_disconnected(s)
    local ip,port = Session.get_address(s)
    Logger.debug(port,ip)
end

local trm_service = 
{
    on_session_recv_cmd = on_trm_recv_cmd_func,
    on_session_disconnect = on_trm_sesssion_disconnected,
}

local trm_server = 
{
    stype = 1,
    service = trm_service,
}

return trm_server