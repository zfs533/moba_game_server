local mysql_game = require("database/mysql_game")
local mysql_center = require("database/mysql_auth_center")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
local player = require("logic_server/player")
local Zone = require("logic_server/Zone")
local State = require("logic_server/State")
local match_mgr = require("logic_server/match_mgr")
local robot_player = require("logic_server/robot_player")

--uid<=>player
local logic_server_players = {}
local online_player_num = 0
local zone_wait_list = {}--匹配等待列表
zone_wait_list[Zone.ASSY] = {}
zone_wait_list[Zone.SGYD] = {}
local zone_match_list = {} --当前比赛的列表
zone_match_list[Zone.ASSY] = {}
zone_match_list[Zone.SGYD] = {}
local zone_robot_list = {}  --存放当前所在区间的机器人
zone_robot_list[Zone.ASSY] = {}
zone_robot_list[Zone.SGYD] = {}

-----------------load robot------------------------------------------------------------
--创建机器人
local function do_new_robot_players(robots)
    if #robots<=0 then
        return 
    end
    local half_len = #robots
    local i = 1
    half_len = math.floor(half_len/2)
    --前半部分放一个区
    for i = 1,half_len do
        local v = robots[i]
        local rb_player = robot_player:new()
        rb_player:init(Stype.Logic,v.uid,nil,nil)
        rb_player.zid = Zone.SGYD
        zone_robot_list[Zone.SGYD][v.uid] = rb_player
    end
    --后半部分放一个区
    for i = half_len+1,#robots do
        local v = robots[i]
        local rb_player = robot_player:new()
        rb_player:init(Stype.Logic,v.uid,nil,nil)
        rb_player.zid = Zone.ASSY
        zone_robot_list[Zone.ASSY][v.uid] = rb_player
    end
end

local function do_load_robot_uinfo(now_index,robots)
    --这里可以递归检查mysql_center中是否有对应的机器人
    do_new_robot_players(robots)
end

local function do_load_robot_ugame_info()
    mysql_game.get_robots_ugame_info(function(err,ret) 
        if err then 
            return 
        end
        if not ret or #ret<= 0 then
            return
        end
        do_load_robot_uinfo(1,ret)
    end)
end

local function load_robots()
    if not mysql_center.is_connected() or not mysql_game.is_connected() then
        Scheduler.once(load_robots,5000)
        return
    end
    do_load_robot_ugame_info()
end

Scheduler.once(load_robots,5000)
-----------------load robot end---------------------------------------------------------
local function send_status(s,stype,ctype,uid,status)
    Logger.debug("login_logic_server4")
    print(stype,ctype,uid,status)
    local msg = {stype,ctype,uid,{status = status}}
    Session.send_msg(s,msg)
end

--{stype,ctype,utag,body}
local function login_logic_server(s,req)
    local uid = req[3]
    local stype = req[1]
    local body = req[4]
    local p = logic_server_players[uid]
    if p then
        p:set_session(s)
        p:set_udp_addr(body.udp_ip,body.udp_port)
        send_status(s,stype,Cmd.eLoginLogicRes,uid,Respones.OK)
        return 
    end
    p = player:new()
    p:init(stype,uid,s,function(status) 
        if status == Respones.OK then
            logic_server_players[uid] = p
            online_player_num = online_player_num + 1
        end
        send_status(s,stype,Cmd.eLoginLogicRes,uid,status)
    end)
    p:set_udp_addr(body.udp_ip,body.udp_port)
end
--{stype,ctype,utag,body}
local function client_disconnect(e,req)
    local uid = req[3]
    local p = logic_server_players[uid]
    if not p then
        return
    end
    --todo 游戏中的玩家，正在游戏的玩家后面在处理
    if p.zid ~= -1 then
        --玩家在等待列表里面
        if zone_wait_list[p.zid][p.uid] then
            zone_wait_list[p.zid][p.uid] = nil
            p.zid = -1
            print("remove from wait list")
        end
    end
    --玩家断开离开房间
    if p then
        print("player uid "..uid.." disconnect!")
        logic_server_players[uid] = nil
        online_player_num = online_player_num -1
    end
end

local function gatway_disconnect(s,stype)
    local k,v 
    for k,v in pairs(logic_server_players) do
        v:set_session(nil)
    end
end

local function gatway_connect(s,stype)
    local k,v 
    for k,v in pairs(logic_server_players) do
        v:set_session(s)
    end
end

local function enter_zone(s,req)
    local stype = req[1]
    local uid = req[3]
    local p = logic_server_players[uid]
    local zid = req[4].zid
    if not p or p.zid ~= -1 then
        send_status(s,stype,Cmd.eEnterZoneRes,uid,Respones.InvalidParams)
        return
    end
    if not zone_wait_list[zid] then
        zone_wait_list[zid] = {}
    end
    zone_wait_list[zid][uid] = p
    p.zid = zid
    send_status(s,stype,Cmd.eEnterZoneRes,uid,Respones.OK)
end
--搜索zid战区可以加入的房间
local function search_inview_match_mgr(zid)
    local match_list = zone_match_list[zid]
    for k,v in pairs(match_list) do
        if v.state == State.InView then
            return v--处于等待状态的房间
        end
    end
    local match = match_mgr:new()
    -- table.insert(match_list,match)
    match:init(zid)
    match_list[match.matchid] = match
    return match
end

--每隔几秒钟检查一次匹配列表中的玩家
local function do_match_players()
    local zid
    local wait_list
    for zid, wait_list in pairs(zone_wait_list) do
        local k,v
        for k,v in pairs(wait_list) do
            local match = search_inview_match_mgr(zid)
            if match then
                if not match:enter_player(v) then
                    Logger.error("match system error : player state: ", v.state)
                else
                    wait_list[k] = nil
                end
            end
        end
    end
end

Scheduler.scheduler(do_match_players,1000,3000,-1)
--搜索空闲的机器人
local function start_search_idle_robot(zid)
    local robots = zone_robot_list[zid]
    local k,v
    for k,v in pairs(robots) do
        if v.matchid == -1 then
            return v
        end
    end
    return nil
end
--将机器人添加到等待列表
local function do_match_robots()
    local zid, match_list
    local k,match
    for zid,match_list in pairs(zone_match_list) do
        for k,match in pairs(match_list) do
            if match.state == State.InView then --找到一个处于等待状态的房间
                local robot = start_search_idle_robot(zid)
                if robot then
                    match:enter_player(robot)
                end
            end
        end
    end
end
Scheduler.scheduler(do_match_robots,1000,3000,-1)

local function send_status(s,stype,ctype,uid,status)
    local msg = {stype,ctype,uid,{status = status}}
    Session.send_msg(s,msg)
end

local function exit_match_list(s,req)
    local stype = req[1]
    local uid = req[3]
    local p = logic_server_players[uid]
    if not p then
        send_status(s,stype,Cmd.eExitMatchRes,uid,Respones.InvalidOpt)
        return 
    end
    if p.state ~= State.InView or p.zid == -1 or p.matchid == -1 then
        send_status(s,stype,Cmd.eExitMatchRes,uid,Respones.InvalidOpt)
        return
    end
    local match = zone_match_list[p.zid][p.matchid]
    if not match or match.state ~= State.InView then
        send_status(s,stype,Cmd.eExitMatchRes,uid,Respones.InvalidOpt)
        return
    end
    match:exit_player(p)
end

--{stype,ctype,utag,body}
local function do_udp_test(s,req)
    local stype = req[1]
    local ctype = req[2]
    local body  = req[4]
    local msg = {stype,ctype,0,{content = body.content}}
    Session.send_msg(s,msg)
end

--{stype,ctype,utag,body}
local function next_frame_opt(s,req)
    local stype = req[1]
    local ctype = req[2]
    local body = req[4]
    local match = zone_match_list[body.zid][body.matchid]
    if not match or match.state ~= State.Playing then
        return
    end
    match:handle_next_frame_event(body)
end

local game_mgr = {
    login_logic_server = login_logic_server,
    client_disconnect = client_disconnect,
    gatway_disconnect = gatway_disconnect,
    gatway_connect = gatway_connect,
    enter_zone = enter_zone,
    exit_match_list = exit_match_list,
    do_udp_test = do_udp_test,
    next_frame_opt = next_frame_opt,
}

return game_mgr