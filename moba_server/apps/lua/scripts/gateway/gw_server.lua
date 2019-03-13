
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
--session come from server
function send_to_client(server_session,raw_cmd)
    local stype,ctype,utag = RawCmd.read_header(raw_cmd)
    local client_session = nil
    --很可能是uid来做key，可是同时要排除掉不是ukey来做的？
    --必须要区分这个命令是登录前还是登录后
    --只有命令的类型才知道我们是要到uid里查，还是到ukey表里查
    --暂时先预留出来，因为和登录有关系要衔接好
    if client_session_uid[utag] ~= nil then
        client_session = client_session_uid[utag]
    elseif client_sessions_ukey[utag] ~= nil then
        client_session = client_sessions_ukey[utag]
    end
    RawCmd.set_utag(raw_cmd,0)
    if client_session then
        Session.send_raw_cmd(client_session,raw_cmd)
    end
end

--session come from client
function send_to_server(client_session,raw_cmd)
    local stype,ctype,utag = RawCmd.read_header(raw_cmd)
    Logger.debug(stype,ctype,utag)
    local server_session = server_sesssion_man[stype]
    if server_session == nil then
        Logger.error("the server ["..game_config.servers[stype].desc.."]"..ip..":"..port..": is not open")
        return
    end
    
    local uid = Session.get_uid(client_session)
    local utag = Session.get_utag(client_session)
    if uid == 0 then--not login 登录前
        if utag == 0 then
            utag = g_ukey
            g_ukey = g_ukey + 1
            client_sessions_ukey[utag] = client_session
            Session.set_utag(client_session,utag)
        end
    else--logined 登录后
        utag = uid
        client_session_uid[utag] = client_session
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
    Logger.debug(port,ip)
    local asclient = Session.as_client(s)
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
    if client_sessions_ukey[utag] ~= nil then
        client_sessions_ukey[utag] = nil
        Session.set_utag(s,0)
        table.remove(client_sessions_ukey,utag)
    end

    --把客户端从uid映射表里面移除
    local uid = Session.get_uid(s)
    if client_session_uid[uid] ~= nil then
        client_session_uid[uid] = nil
        table.remove(client_session_uid,uid)
    end
    --客户端uid用户掉线了，把这个事件告诉和网关连接的stype类型的服务器
    if uid ~= 0 then

    end
end

gw_service_init()

local gw_server = 
{
    on_session_recv_raw_cmd = on_gw_recv_cmd_func,
    on_session_disconnect = on_gw_sesssion_disconnected,
}

return gw_server