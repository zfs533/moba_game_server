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
    self.is_robot = false
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
    }
    return body
end

function player:send_cmd(ctype,body)
    if not self.session or self.is_robot then
        return
    end
    local msg = {self.stype,ctype,self.uid,body}
    Session.send_msg(self.session,msg)
end

function player:set_session(s)
    self.session = s
end

return player