local mysql_game = require("database/mysql_game")
local mysql_center = require("database/mysql_auth_center")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
local moba_ugame_config = require("moba_game_config")

local function send_rank_info_to_client(s,uid,ret,center_info,ugame_data)
    local rank_info = {}
    local i
    for i = 1,#ret do
        local info = {}
        info.unick = center_info[i].unick
        info.uface = center_info[i].uface
        info.usex = center_info[i].usex
        info.uvip = ugame_data[i].uvip
        info.uchip = ugame_data[i].uchip
        rank_info[i] = info
    end
    local msg = {Stype.System,Cmd.eGetWorldRankUchipRes,uid,{status = Respones.OK,rankinfo = rank_info}}
    Session.send_msg(s,msg)
end

local function get_ugame_system_info(uid,success,failed)
    mysql_game.get_ugame_system_uinfo(uid,function(err,info)
        if err then
            failed()
            return
        end
        success(info)
    end)
end

local function get_ugame_info_rank(s,uid,ret,center_info)
    local ugame_data = {}
    local index = 1
    local function success(ugame_info)
        ugame_data[index] = ugame_info
        if #ugame_data == #ret then
            send_rank_info_to_client(s,uid,ret,center_info,ugame_data)
        else
            index = index + 1
            get_ugame_system_info(ret[index].uid,success,failed)
        end
    end
    local function failed()
        local msg = {Stype.System,Cmd.eGetWorldRankUchipRes,uid,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
        return;
    end
    get_ugame_system_info(ret[index].uid,success,failed)
end

local function get_auth_center_info(uid,success,failed)
    mysql_center.get_uinfo_by_uid(uid,function(err,uinfo) 
        if err or uinfo == nil then
            failed()
            return
        end
        success(uinfo)
    end)
end

--{stype,ctype,utag,body}
local function get_world_uchip_rank_info(s,req)
    local uid = req[3]
    local rankNum = req[4].ranknum
    utils.print_tb(req)
    mysql_game.get_world_rank_with_uchip(rankNum,function(err,ret) 
        if err then
            local msg = {Stype.System,Cmd.eGetWorldRankUchipRes,uid,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return;
        end
        
        local function failed()
            local msg = {Stype.System,Cmd.eGetWorldRankUchipRes,uid,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return;
        end

        local center_info = {}
        local index = 1
        local function success(uinfo)
            center_info[index] = uinfo
            if #center_info == #ret then
                get_ugame_info_rank(s,uid,ret,center_info)--获取游戏数据
            else
                index = index + 1
                get_auth_center_info(ret[index].uid,success,failed)
            end
        end
        get_auth_center_info(ret[1].uid,success,failed)--获取用户中心数据,unick,usex,uface
    end)
end

local game_rank = 
{
    get_world_uchip_rank_info = get_world_uchip_rank_info,
}

return game_rank