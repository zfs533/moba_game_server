local mysql_center = require("database/mysql_auth_center")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")

function _do_account_upgrade(s,req,uid,uname,upwdmd5)
    mysql_center.do_guest_account_upgrade(uid,uname,upwdmd5,function(err,ret)
        if err then
            local msg = {Stype.Auth,Cmd.eAccountUpgradeRes,uid,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return
        end
        
        local msg = {Stype.Auth,Cmd.eAccountUpgradeRes,uid,{status = Respones.OK}}
        Session.send_msg(s,msg)
    end)
end

function _check_is_guest(s,req,uid,uname,upwdmd5)
    mysql_center.get_uinfo_by_uid(uid,function(err,uinfo)
        if err then
            local msg = {Stype.Auth,Cmd.eAccountUpgradeRes,uid,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return
        end
        if uinfo.is_guest ~= 1 then --不是游客账号
            local msg = {Stype.Auth,Cmd.eAccountUpgradeRes,uid,{status = Respones.UserInNotGuest}}
            Session.send_msg(s,msg)
            return
        end
        _do_account_upgrade(s,req,uid,uname,upwdmd5)
    end)
end

--{stype,ctype,utag,body}
function do_upgrade(s,req)
    local uid = req[3]
    local account_upgrade_req = req[4]
    local uname = account_upgrade_req.uname
    local upwdmd5 = account_upgrade_req.upwdmd5
    if string.len(uname) <=0 or string.len(upwdmd5) ~= 32 then
        local msg = {Stype.Auth,Cmd.eAccountUpgradeRes,uid,{status = Respones.InvalidParams}}
        Session.send_msg(s,msg)
        return
    end

    Logger.debug(uname,upwdmd5,uid)
    mysql_center.check_uname_exit(uname,function(err,ret) 
        if err then
            local msg ={Stype.Auth,Cmd.eAccountUpgradeRes,uid,{status = Respones.SystemErr}}
            Session.send_msg(s,msg)
            return
        end
        if ret then--账号已存在/已被占用
            local msg = {Stype.Auth,Cmd.eAccountUpgradeRes,uid,{status = Respones.UnameIsExist}}
            Session.send_msg(s,msg)
            return
        end
        --检查uid是否为游客
        --end
        _check_is_guest(s,req,uid,uname,upwdmd5)
    end)
end

local account_upgrade = 
{
    do_upgrade = do_upgrade;
}

return account_upgrade