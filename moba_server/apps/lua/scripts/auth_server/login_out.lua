local mysql_center = require("database/mysql_auth_center")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")

--{stype,ctype,utag,body}
local function login_out(s,req)
    utils.print_tb(req)
    local uid = req[3]
    Logger.debug("loginout-------------------------------")
    local msg = {Stype.Auth,Cmd.eLoginOutRes,uid,{status = Respones.OK}}
    Session.send_msg(s, msg)
end


local user_login_out = 
{
    login_out = login_out,
}

return user_login_out
