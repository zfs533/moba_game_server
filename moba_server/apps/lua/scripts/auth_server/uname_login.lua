local mysql_center = require("database/mysql_auth_center")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")

--{stype,ctype,utag,body}
function login(s,req)
    utils.print_tb(req[4])
    local utag = req[3]
    local uname = req[4].uname
    local upwd = req[4].upwd
    if string.len(upwd) ~= 32 or string.len(uname) <= 0 then
        local msg = {Stype.Auth,Cmd.eUnameLoginRes,utag,{status = Respones.InvalidParams}}
        Session.send_msg(s,msg)
        return
    end
    mysql_center.get_uinfo_by_uname_upwd(uname,upwd,function(err,uinfo)
        if err then--告诉客户端系统错误信息;
            local msg = {Stype.Auth,Cmd.eUnameLoginRes,utag,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return
        end
        if uinfo == nil then--没有查到对应的 g_key的信息
            local msg = {Stype.Auth,Cmd.eUnameLoginRes,utag,{status = Respones.UnameOrUpwdError}}
            Session.send_msg(s,msg)
            return
        end

        --找到了g_key所对应的游客数据
        if uinfo.status ~= 0 then --账号被查封
            local msg = {Stype.Auth,Cmd.eUnameLoginRes,utag,{status = Respones.UserIsFreeze}}
            Session.send_msg(s,msg)
            return 
        end
        Logger.debug(uinfo.uid,uinfo.unick)
        --登录成功还回给客户端
        local msg = {Stype.Auth,Cmd.eUnameLoginRes,utag,{
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

local uname_login =
{
    login = login,
}

return uname_login