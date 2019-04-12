local mysql_game = require("database/mysql_game")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")

local player = {}
function player:new(instant)
    if not instant then
        instant = {}
    end
    setmetatable(instant, {__index = self})
    return instant
end

function player:init(uid,s,ret_handler)
    self.session = s
    self.uid = uid
    --get player basedata from database
    mysql_game.get_ugame_system_uinfo(uid,function(err,ugame_info) 
        if err then
            ret_handler(Respones.SystemErr)
            return
        end
        self.ugame_info = ugame_info
        ret_handler(Respones.OK)
    end)
    --end
end

function player:set_session(s)
    self.session = s
end

return player