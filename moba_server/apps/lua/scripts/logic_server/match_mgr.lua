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
local PLAYER_NUM = 3

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

    self.inview_players = {}--旁观玩家列表，加入进来的玩家
    self.left_players = {} --左边的玩家
    self.right_players= {} --右边的玩家
end

--广播自己进入房间消息
function match_mgr:broadcast_cmd_inview_players(ctype,body,not_to_player)
    local i
    for i = 1,#self.inview_players do
        local p = self.inview_players[i]
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
    table.insert( self.inview_players,p)--将玩家加入到当前房间的，玩家集结列表里面
    p.matchid = self.matchid

    local i
    --发送命令，告诉客户端，加入了一个比赛房间  zid matchid
    local body =
    {
        zid = self.zid,
        matchid = self.matchid
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
        for i = 1, #self.inview_players do
            self.inview_players[i].state = State.Ready
        end
    end

    return true
end

return match_mgr