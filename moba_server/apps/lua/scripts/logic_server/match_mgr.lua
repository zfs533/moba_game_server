--[[
    游戏房间管理
]]
local mysql_game = require("database/mysql_game")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
local player = require("logic_server/player")
local Zone = require("logic_server/Zone")
local State = require("logic_server/State")

local match_mgr = {}
local sg_matchid = 1--房间号
local PLAYER_NUM = 2

function match_mgr:new(instant)
    if not instant then
        instant = {}
    end
    setmetatable(instant,{__index = self})
    return instant
end

function match_mgr:init(zid)
    Logger.debug("-------------zid= "..zid)
    self.zid = zid
    self.matchid = sg_matchid
    sg_matchid = sg_matchid + 1
    self.state = State.InView
    self.frameid = 0

    self.inview_players = {}--旁观玩家列表，加入进来的玩家
    self.left_players = {} --左边的玩家
    self.right_players= {} --右边的玩家
    self.match_frames = {} --保存的是游戏开始依赖的所有的帧操作
    self.next_frame_opt = {frameid = self.frameid,opts = {}} --保存的是当前帧玩家的操作
end

--广播自己进入房间消息
function match_mgr:broadcast_cmd_inview_players(ctype,body,not_to_player)
    local k,v
    for k,v in pairs(self.inview_players) do
        local p = v
        if p ~= not_to_player then
            p:send_cmd(ctype,body)
        end
    end
end

--玩家加入房间
function match_mgr:enter_player(p)
    if self.state ~= State.InView or p.state ~= State.InView then
        return false
    end
    -- table.insert( self.inview_players,p)
    --将玩家加入到当前房间的，玩家集结列表里面
    p.matchid = self.matchid
    p.sync_frameid = 0
    for i = 1,PLAYER_NUM*2 do
        if not self.inview_players[i] then
            p.seatid = i
            p.side = 0
            if i>PLAYER_NUM then
                p.side = 1
            end
            self.inview_players[i] = p
            break
        end
    end

    local i
    --发送命令，告诉客户端，加入了一个比赛房间  zid matchid
    local body =
    {
        zid = self.zid,
        matchid = self.matchid,
        seatid = p.seatid,
        side = p.side,
    }
    p:send_cmd(Cmd.eEnterMatch,body)
    --end
    --将用户进入房间的消息发送给房间里面的其他玩家--广播
    body = p:get_user_arrived_info()
    self:broadcast_cmd_inview_players(Cmd.eUserArrived,body,p)
    --end
    --玩家还要收到其他在这个房间（等待列表）里的玩家
    for i = 1, #self.inview_players do
        local o_player = self.inview_players[i]
        if o_player ~= p then
            body = o_player:get_user_arrived_info()
            p:send_cmd(Cmd.eUserArrived,body)
        end
    end
    --end
    --判断房间人满
    if #self.inview_players >= PLAYER_NUM*2 then
        self.state = State.Ready
        -- for i = 1, #self.inview_players do
        --     self.inview_players[i].state = State.Ready
        -- end
        self:update_players_state(State.Ready)
        --开始游戏数据发送给客户端
        --进入到一个选英雄的界面，直到所有的玩家选好英雄这样一个状态
        --在游戏主页里面，自己设置你用的英雄，然后你自己再用大厅哪里设置英雄
        --这里服务器随机生成英雄id[1,5]
        for i = 1,PLAYER_NUM*2 do
            self.inview_players[i].heroid = math.floor(math.random()*5+1)--[1,5]
        end
        self:game_start()
    end
    return true
end

function match_mgr:update_players_state(state)
    for i = 1,PLAYER_NUM*2 do
        self.inview_players[i].state = state
    end
end

function match_mgr:game_start()
    local players_match_info = {}
    for i = 1,PLAYER_NUM*2 do
        local p = self.inview_players[i]
        local info = 
        {
            heroid = p.heroid, 
            seatid = p.seatid,
            side = p.side,
        }
        table.insert(players_match_info,info)
    end
    local body = 
    {
        playerMathInfo = players_match_info,
    }
    print("game_start.............")
    self:broadcast_cmd_inview_players(Cmd.eGameStart,body,nil)
    self.state = State.Playing
    self:update_players_state(State.Playing)
    --6秒以后 开始第一个帧事件，
    self.frameid = 1
    self.match_frames = {} --保存的是游戏开始依赖的所有的帧操作
    self.next_frame_opt = {frameid = self.frameid,opts = {}} --保存的是当前帧玩家的操作
    self.frame_timer = Scheduler.scheduler(function() 
        self:on_logic_frame()
    end,5000,50,-1)
end
--客户端发过来的当前帧操作
function match_mgr:handle_next_frame_event(next_frame_opt)
    local seatid = next_frame_opt.seatid
    local p = self.inview_players[seatid]
    if not p then
        return
    end
    --当前客户端已经同步了哪个？？
    print(p.sync_frameid,next_frame_opt.frameid - 1)
    if p.sync_frameid < next_frame_opt.frameid - 1 then
        p.sync_frameid = next_frame_opt.frameid - 1
    end
    --end
    if next_frame_opt.frameid ~= self.frameid then
        return
    end

    for i = 1,#next_frame_opt.opts do
        table.insert(self.next_frame_opt.opts,next_frame_opt.opts[i])
    end
end

function match_mgr:send_unsync_frames(p)
    local opt_frames = {}
    local len = #self.match_frames
    for i = (p.sync_frameid + 1), len do
        table.insert(opt_frames,self.match_frames[i])
    end
    local body = {frameid = self.frameid,unsyncFrames = opt_frames}
    p:udp_send_cmd(Stype.Logic,Cmd.eLogicFrame,body)
end

function match_mgr:on_logic_frame()
    --将当前玩家的所有操作帧保存起来
    table.insert( self.match_frames,self.next_frame_opt)
    for i = 1,PLAYER_NUM*2 do
        local p = self.inview_players[i]
        if p then
            self:send_unsync_frames(p) --将未同步的帧发送给客户端
        end
    end
    self.frameid = self.frameid + 1
    self.next_frame_opt = {frameid = self.frameid,opts = {}}
end



function match_mgr:exit_player(p)
    local body = {
        seatid = p.seatid,
    }
    self:broadcast_cmd_inview_players(Cmd.eUserExitMatch,body,p)
    self.inview_players[p.seatid] = nil
    p.zid = -1
    p.matchid = -1
    local body = {status = Respones.OK}
    p:send_cmd(Cmd.eExitMatchRes,body)
end

return match_mgr