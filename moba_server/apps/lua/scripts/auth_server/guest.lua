
local mysql_center = require("database/mysql_auth_center")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")
--{stype,ctype,utag,body}
local function login(s,req)
    local g_key = req[4].guestkey
    Logger.debug(req[1], req[2], req[3],req[4].guestkey)
    local utag = req[3]
    --判断g_key合法性，是否为字符串，且长度为32
    if type(g_key) ~= "string" or string.len(g_key) ~= 32 then
        local msg = {Stype.Auth,Cmd.eGuestLoginRes,utag,{status = Respones.InvalidParams}}
        Session.send_msg(s,msg)
        return
    end
    mysql_center.get_guest_uinfo(g_key,function(err,uinfo)
        if err then--告诉客户端系统错误信息;
            local msg = {Stype.Auth,Cmd.eGuestLoginRes,utag,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return
        end
        if uinfo == nil then--没有查到对应的 g_key的信息
            mysql_center.insert_guest_user(g_key,function(err,ret)--插入到数据库
                if err then--告诉客户端系统错误信息
                    local msg = {Stype.Auth,Cmd.eGuestLoginRes,utag,{status = Respones.SystemErr}}
                    Session.send_msg(s,msg)
                    return
                end
                --插入之后在调用一次
                login(s,req)
            end)
            return
        end

        -- 找到了g_key所对应的游客数据
        if uinfo.status ~= 0 then --账号被查封
            local msg = {Stype.Auth,Cmd.eGuestLoginRes,utag,{status = Respones.UserIsFreeze}}
            Session.send_msg(s,msg)
            return 
        end
        if uinfo.is_guest ~= 1 then --账号已经不是游客账号了
            local msg = {Stype.Auth,Cmd.eGuestLoginRes,utag,{status = Respones.UserInNotGuest}}
            Session.send_msg(s,msg)
            return 
        end

        Logger.debug(uinfo.uid,uinfo.unick)
        --登录成功还回给客户端
        local msg = {Stype.Auth,Cmd.eGuestLoginRes,utag,{
            status = Respones.OK,
            info = 
            {
                unick = uinfo.unick,
                sex = uinfo.usex,
                face = uinfo.uface,
                uvip = uinfo.uvip,
                uid = uinfo.uid,
            }
            }
        }
        Session.send_msg(s,msg)
    end)
end

local guest = 
{
    login = login,
}

return guest