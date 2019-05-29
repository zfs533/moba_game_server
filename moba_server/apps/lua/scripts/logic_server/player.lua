local mysql_center = require("database/mysql_auth_center")
local mysql_game = require("database/mysql_game")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
local State = require("logic_server/State")

local player = {}
function player:new(instant)
    if not instant then
        instant = {}
    end
    setmetatable(instant, {__index = self})
    return instant
end

function player:init(stype,uid,s,ret_handler)
    self.session = s
    self.stype = stype
    self.uid = uid
    self.zid = -1 --玩家所在的空间，-1，不存在任何游戏场
    self.state = State.InView
    self.matchid = -1 --玩家所在房间id
    self.seatid  = -1 --玩家在比赛中的序列号
    self.side = -1 --玩家在比赛中所在的边（0,1）
    self.heroid = -1--英雄号[1,5]
    self.is_robot = false
    self.client_udp_ip = nil --玩家对应客户端的 udp的ip地址
    self.client_udp_port = 0 --玩家对应客户端的 udp的端口
    self.sync_frameid = 0 --玩家同步到那一帧了
    --get player basedata from database
    mysql_game.get_ugame_system_uinfo(uid,function(err,ugame_info) 
        if err then
            if ret_handler then
                ret_handler(Respones.SystemErr)
            end
            return
        end
        self.ugame_info = ugame_info
        mysql_center.get_uinfo_by_uid(uid,function(err,center_info) 
            if err then
                if ret_handler then
                    ret_handler(Respones.SystemErr)
                end
                return
            end
            self.center_info = center_info
            if ret_handler then
                ret_handler(Respones.OK)
            end
        end)
    end)
    --end
end

function player:get_user_arrived_info()
    local body = 
    {
        unick = self.center_info.unick,
        uface = self.center_info.uface,
        usex = self.center_info.usex,
        seatid= self.seatid,
        side = self.side;
    }
    return body
end

function player:send_cmd(ctype,body)
    if not self.session or self.is_robot then
        return
    end
    local msg = {self.stype,ctype,self.uid,body}
    utils.print_tb(msg)
    Session.send_msg(self.session,msg)
end

function player:set_session(s)
    self.session = s
end

function player:set_udp_addr(ip,port)
    self.client_udp_ip = ip
    self.client_udp_port = port
end

function player:udp_send_cmd(stype,ctype,body)
    if not self.session or self.is_robot then--玩家断线或是机器人
        return
    end
    if not self.client_udp_ip or self.client_udp_port == 0 then
        return 
    end
    local msg = {stype,ctype,0,body}
    Session.udp_send_msg(self.client_udp_ip,self.client_udp_port,msg)
end

return player