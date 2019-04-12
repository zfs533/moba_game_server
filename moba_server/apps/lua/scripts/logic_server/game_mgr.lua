local mysql_game = require("database/mysql_game")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
local player = require("logic_server/player")

local function send_status(s,stype,ctype,uid,status)
    Logger.debug("login_logic_server4")
    print(stype,ctype,uid,status)
    local msg = {stype,ctype,uid,{status = status}}
    Session.send_msg(s,msg)
end

--uid<=>player
local logic_server_players = {}
local online_player_num = 0
--{stype,ctype,utag,body}
local function login_logic_server(s,req)
    local uid = req[3]
    local p = logic_server_players[uid]
    if p then
        p:set_session(s)
        send_status(s,Stype.Logic,Cmd.eLoginLogicRes,uid,Respones.OK)
        return 
    end
    p = player:new()
    p:init(uid,s,function(status) 
        if status == Respones.OK then
            logic_server_players[uid] = p
            online_player_num = online_player_num + 1
        end
        send_status(s,Stype.Logic,Cmd.eLoginLogicRes,uid,status)
    end)
end
--{stype,ctype,utag,body}
local function user_lost_connect(e,req)
    local uid = req[3]
    if logic_server_players[uid] then
        logic_server_players[uid] = nil
        online_player_num = online_player_num -1
    end
end

local game_mgr = {
    login_logic_server = login_logic_server,
    user_lost_connect = user_lost_connect,
}

return game_mgr