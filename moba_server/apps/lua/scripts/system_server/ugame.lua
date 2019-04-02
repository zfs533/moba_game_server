
local mysql_game = require("database/mysql_game")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
local moba_ugame_config = require("moba_game_config")
local login_bonues = require("system_server/login_bonues")
--{stype,ctype,utag,body}
local function get_ugame_info(s,req)
    utils.print_tb(req)
    local uid = req[3]
    mysql_game.get_ugame_system_uinfo(uid,function(err,uinfo)
        if err then--告诉客户端系统错误信息;
            local msg = {Stype.System,Cmd.eGetUgameInfoRes,uid,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return
        end
        if uinfo == nil then--没有查到对应的 uid信息
            local uchip = moba_ugame_config.ugame.uchip
            local uvip  = moba_ugame_config.ugame.uvip
            local uexp  = moba_ugame_config.ugame.uexp
            mysql_game.insert_ugame_system_user(uid,uchip,uvip,uexp,function(err,ret)--插入到数据库
                if err then--告诉客户端系统错误信息
                    local msg = {Stype.System,Cmd.eGetUgameInfoRes,uid,{status = Respones.SystemErr}}
                    Session.send_msg(s,msg)
                    return
                end
                --插入之后在调用一次
                get_ugame_info(s,req)
            end)
            return
        end

        -- 找到了g_key所对应的游客数据
        if uinfo.ustatus ~= 0 then --账号被查封
            local msg = {Stype.System,Cmd.eGetUgameInfoRes,uid,{status = Respones.UserIsFreeze}}
            Session.send_msg(s,msg)
            return 
        end

        --检查登录奖励
        login_bonues.check_login_bonues(uid,function(err,bonues_info) 
            if err then--告诉客户端系统错误信息;
                local msg = {Stype.System,Cmd.eGetUgameInfoRes,uid,{status = Respones.SystemErr}}
                Session.send_msg(s,msg)
                return
            end
            utils.print_tb(bonues_info)
            --登录成功还回给客户端
            local msg = {Stype.System,Cmd.eGetUgameInfoRes,uid,{
                status = Respones.OK,
                info = 
                {
                    uchip = uinfo.uchip,
                    uchip1 = uinfo.uchip1,
                    uchip2 = uinfo.uchip2,
                    udata1 = uinfo.udata1,
                    udata2 = uinfo.udata2,
                    udata3 = uinfo.udata3,
                    uexp = uinfo.uexp,
                    uvip = uinfo.uvip,
                    bonues_status = bonues_info.status,
                    bonues = bonues_info.bonues,
                    days = bonues_info.days,
                }
                }
            }
            Session.send_msg(s,msg)
        end)
    end)
end

local get_ugame_info_tb = 
{
    get_ugame_info = get_ugame_info,
}
return get_ugame_info_tb