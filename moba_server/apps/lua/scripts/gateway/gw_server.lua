local Stype = require("Stype")
local Cmd = require("Ctype")
local Respones = require("respones")
local game_config = require("game_config")
--stype --> session的一个映射
local server_sesssion_man = {}
--current connecting and not finish
local do_connecting = {}
--临时的ukey 来找client session
local g_ukey = 1
local client_sessions_ukey = {}
--uid来找client session
local client_session_uid = {}

function connect_to_server(stype,ip,port)
    Netbus.tcp_connect(ip,port,function(err,session) 
        do_connecting[stype] = false;
        if err ~= 0 then
            Logger.error("connect error to server ["..game_config.servers[stype].desc.."]"..ip..":"..port)
            return
        end
        server_sesssion_man[stype] = session
        Logger.debug("connect success to server ["..game_config.servers[stype].desc.."]"..ip..":"..port)
    end)
end

function check_server_connect()
    for k,v in pairs(game_config.servers) do
        if server_sesssion_man[v.stype] == nil and do_connecting[v.stype] == false then
            do_connecting[v.stype] = true;
            Logger.debug("connecting to server ["..v.desc.."] "..v.ip..":"..v.port)
            connect_to_server(v.stype,v.ip,v.port)
        end
    end
end

--init gateway server
function gw_service_init()
    for k,v in pairs(game_config.servers) do
        server_sesssion_man[v.stype] = nil
        do_connecting[v.stype] = false;
    end
    --start a timer to check connect or break
    Scheduler.scheduler(check_server_connect,1000,3000,-1)
end

function is_login_return_cmd(ctype)
    if ctype == Cmd.eGuestLoginRes or ctype == Cmd.eUnameLoginRes then
        return true
    end
    return false
end

--session come from server
function send_to_client(server_session,raw_cmd)
    local stype,ctype,utag = RawCmd.read_header(raw_cmd)
    local client_session = nil
    
    if is_login_return_cmd(ctype) then 
        client_session = client_sessions_ukey[utag]
        client_sessions_ukey[utag] = nil
        if client_session == nil then
            Logger.debug("client_session is nil")
            return 
        end
        local body = RawCmd.read_body(raw_cmd)--return the protobuf data (lua table),to get uid and manager it
        if body.status ~= Respones.OK then--login failed
            Logger.debug("login failed......")
            RawCmd.set_utag(raw_cmd,0)
            Session.send_raw_cmd(client_session,raw_cmd)
            return
        end
        local uid = body.info.uid
        Logger.debug("user uid===========================>uid=> "..uid)
        --relogin
        --jugement the session is logined??是否有已经登录的session,这种情况应该不会出现吧，多个session用同一个uid
        -- if client_session_uid[uid] and client_session_uid[uid] ~= client_session then
        if client_session_uid[uid] and client_session_uid[uid] == client_session then
            local relogin_cmd = {Stype.Auth,Cmd.eRelogin,0,{status = 1}}
            Session.send_msg(client_session_uid[uid],relogin_cmd)
            -- Session.close()
            return
        end
        client_session_uid[uid] = client_session
        Session.set_uid(client_session,uid)
        --clear
        body.info.uid = 0
        local login_res = {stype,ctype,0,body}
        Session.send_msg(client_session,login_res)
        return
    end
    client_session = client_session_uid[utag]
    if client_session then
        RawCmd.set_utag(raw_cmd,0)
        Session.send_raw_cmd(client_session,raw_cmd)
        if ctype == Cmd.eLoginOutRes then  -- 注销得消息，转发给其他得服务器
            Logger.debug(utag,utag)
            Session.set_uid(client_session,0)
            client_session_uid[utag] = nil
        end
    end
end

function is_login_request_cmd(ctype)
    if ctype == Cmd.eGuestLoginReq or ctype == Cmd.eUnameLoginReq then
        return true
    end
    return false
end

--session come from client
function send_to_server(client_session,raw_cmd)
    local stype,ctype,utag = RawCmd.read_header(raw_cmd)
    Logger.debug(stype,ctype,utag)
    local server_session = server_sesssion_man[stype]--find current server by stype
    if server_session == nil then
        Logger.error("the server ["..game_config.servers[stype].desc.."]"..ip..":"..port..": is not open")
        return
    end
    --manager uid
    if is_login_request_cmd(ctype) then
        utag = Session.get_utag(client_session)
        if utag == 0 then
            utag = g_ukey
            g_ukey = g_ukey + 1
            Session.set_utag(client_session,utag)
        end
        client_sessions_ukey[utag] = client_session
    elseif ctype == Cmd.eLoginLogicReq then
        local uid = Session.get_uid(client_session)
        utag = uid
        if utag == 0 then -- 改操作要先登录
            return
        end
        local tcp_ip,tcp_port = Session.get_address(client_session)
        local body = RawCmd.read_body(raw_cmd)
        body.udp_ip = tcp_ip

        local login_logic_cmd = {stype,ctype,utag,body}
        Session.send_msg(server_session,login_logic_cmd)
        return
    else
        local uid = Session.get_uid(client_session)
        utag = uid
        if utag == 0 then --if you want other options,please login first
            Logger.debug("if you want other options,please login first")
            return
        end
    end
    --打上utag然后转发给我们的服务器
    --将utag打入数据包,然后转发给服务器
    RawCmd.set_utag(raw_cmd,utag)
    Session.send_raw_cmd(server_session,raw_cmd)
end

--{stype,ctype,utag,body}
function on_gw_recv_cmd_func(s,raw_cmd)
    if Session.as_client(s) == 0 then--client session and transition to server
        send_to_server(s,raw_cmd)
    else
        send_to_client(s,raw_cmd)
    end
end

function on_gw_sesssion_disconnected(s,stype)
    local ip,port = Session.get_address(s)
    local asclient = Session.as_client(s)
    print(asclient)
    Logger.debug(port,ip,asclient)
    --the net break --- the session that connected server
    --连接到网关的服务器(server client)的session断线了
    if asclient == 1 then
        for k,v in pairs(server_sesssion_man) do
            if v == s then
                Logger.debug("gateway disconnect ["..game_config.servers[k].desc.."]")
                server_sesssion_man[k] = nil
            end
        end
        return
    end
    --连接到网关的客户端掉线
    --the client net break
    --把客户端从临时映射表里面删除
    local utag = Session.get_utag(s)
    if client_sessions_ukey[utag] ~= nil and client_sessions_ukey == s then
        client_sessions_ukey[utag] = nil
        Session.set_utag(s,0)
        print("close1")
    end

    --把客户端从uid映射表里面移除
    local uid = Session.get_uid(s)
    if client_session_uid[uid] ~= nil and client_session_uid[uid] == s then
        client_session_uid[uid] = nil
        print("close")
    end

    local server_session = server_sesssion_man[stype]
    if server_session == nil then
        return 
    end

    --客户端uid用户掉线了，把这个事件告诉和网关连接的stype类型的服务器
    Logger.debug("lose player uid=> "..uid)
    if uid ~= 0 then
        local user_lost = {stype,Cmd.eUserLostConn,uid,nil}
        Session.send_msg(server_session,user_lost)
    end
end

gw_service_init()

local gw_server = 
{
    on_session_recv_raw_cmd = on_gw_recv_cmd_func,
    on_session_disconnect = on_gw_sesssion_disconnected,
}

return gw_server