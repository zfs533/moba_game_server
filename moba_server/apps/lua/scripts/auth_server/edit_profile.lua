local mysql_center = require("database/mysql_auth_center")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")

--{stype,ctype,utag,body}
function do_edit_profile(s,req)

    local uid = req[3]
    local edit_profile_req = req[4]
    local unick = edit_profile_req.unick
    local uface = edit_profile_req.uface
    local usex = edit_profile_req.usex
    Logger.debug(uid,edit_profile_req.unick,edit_profile_req.uface,edit_profile_req.usex)
    if string.len(unick) <= 0 or uface <= 0 or uface > 9 or (usex ~= 0 and usex ~=1) then 
        local msg = {Stype.Auth,Cmd.eEditProfileRes,uid,{status = Respones.InvalidParams}}
        Session.send_msg(s,msg)
        return
    end
    --更新用户中心数据库
    local ret_status = Respones.OK
    local ret_handler = function(err,ret)
        if err then
            ret_status = Respones.SystemErr
        end
        local msg = {Stype.Auth,Cmd.eEditProfileRes,uid,{status = ret_status}}
        Session.send_msg(s,msg)
    end
    mysql_center.edit_profile(uid,unick,uface,usex,ret_handler)
end

local edit_profile = 
{
    do_edit_profile = do_edit_profile
}

return edit_profile