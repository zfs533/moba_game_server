
local mysql_game = require("database/mysql_game")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
local moba_ugame_config = require("moba_game_config")

local function send_bonues_to_user(uid,bonues_info,ret_handler)
    if bonues_info.bonues_time<TimeStamp.timestamp_today() then--没领取
        if bonues_info.bonues_time >= TimeStamp.timestamp_yesterday() then --连续领取
            bonues_info.days = bonues_info.days + 1
        else
            bonues_info.days = 1--不连续，重新开始计算
        end
        if bonues_info.days > #moba_ugame_config.login_bonues then
            bonues_info.days = 1--重置
        end
        bonues_info.status = 0
        bonues_info.bonues_time = TimeStamp.timestamp()
        bonues_info.bonues = moba_ugame_config.login_bonues[bonues_info.days]
        --update
        mysql_game.update_login_bonues(uid,bonues_info,function(err,ret) 
            if err then
                ret_handler(err,nil)
                return
            end
            ret_handler(nil,bonues_info)
        end)
        return
    end
    ret_handler(nil,bonues_info)
end

local function check_login_bonues(uid,ret_handler)
    mysql_game.get_bonues_info(uid,function(err,bonues_info) 
        if err then
            ret_handler(err,nil)
            return
        end
        --第一次登录，没有数据
        if bonues_info == nil then
            mysql_game.insert_bonues_info(uid,function(err,ret) 
                if err then 
                    ret_handler(err,nil)
                    return
                end
                check_login_bonues(uid,ret_handler)
            end)
            return
        end
        send_bonues_to_user(uid,bonues_info,ret_handler)
    end)
end
--{stype,ctype,utag,body}
local function recv_login_bonues( s, req)
    local uid = req[3]
    mysql_game.get_bonues_info(uid,function(err,bonues_info)
        if err then--告诉客户端系统错误信息;
            local msg = {Stype.System,Cmd.eRecvLoginBonuesRes,uid,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return
        end
        if bonues_info == nil or bonues_info.status ~= 0 then
            local msg = {Stype.System,Cmd.eRecvLoginBonuesRes,uid,{status = Respones.InvalidOpt}} 
            Session.send_msg(s,msg)
            return
        end
        -- 有奖励可以领取
        mysql_game.update_login_bonues_status(uid,function(err,ret)
            if err then--告诉客户端系统错误信息;
                local msg = {Stype.System,Cmd.eRecvLoginBonuesRes,uid,{status = Respones.SystemErr}}
                Session.send_msg(s,msg)
                return
            end
            --更新数据库的uchip
            mysql_game.add_chip(uid,bonues_info.bonues,nil)
            local msg = {Stype.System,Cmd.eRecvLoginBonuesRes,uid,{status = Respones.OK}}
            Session.send_msg(s,msg)
        end)
    end)
end

local login_bonues = {
    check_login_bonues = check_login_bonues,
    recv_login_bonues = recv_login_bonues,
}

return login_bonues