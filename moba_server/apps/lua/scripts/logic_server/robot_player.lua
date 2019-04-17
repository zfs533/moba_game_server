--extends player
local player = require("logic_server/player")
local robot_player = player:new()

function robot_player:new()
    local constant = {}
    setmetatable(constant,{__index = self})
    return constant
end

function robot_player:init(stype,uid,s,ret_handler)
    player.init(self,stype,uid,s,ret_handler)
    self.is_robot = true
end

return robot_player
