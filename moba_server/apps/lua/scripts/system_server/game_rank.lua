local mysql_game = require("database/mysql_game")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
local moba_ugame_config = require("moba_game_config")

--{stype,ctype,utag,body}
local function get_world_uchip_rank_info(s,req)
    local uid = req[3]
    local rankNum = req[4].rankNum
    mysql_game.get_world_rank_with_uchip(rankNum,function(err,ret) 
        if err then
            local msg = {Stype.System,Cmd.eGetWorldRankUchipRes,uid,{status = Respones.SystemErr,rankinfo = {}}}
            Session.send_msg(s,msg)
            return;
        end
        local msg = {Stype.System,Cmd.eGetWorldRankUchipRes,uid,{status = Respones.OK,rankinfo = ret}}
        Session.send_msg(s,msg)
    end)
end

local game_rank = 
{
    get_world_uchip_rank_info = get_world_uchip_rank_info,
}

return game_rank