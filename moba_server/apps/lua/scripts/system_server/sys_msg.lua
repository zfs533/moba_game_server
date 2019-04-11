local mysql_game = require("database/mysql_game")
local Respones = require("respones")
local Stype = require("Stype")
local Cmd   = require("Ctype")
local utils = require("utils")

local sys_msg_tb = {}
local sys_msg_version = 0
local function get_sys_msg_tb() 
    mysql_game.get_sys_msg_info(function(err,ret) 
        if err then
            Logger.debug(err)
            Scheduler.once(get_sys_msg_tb,5000)
            return
        end
        sys_msg_version = TimeStamp.timestamp()
        sys_msg_tb = ret;
        if sys_msg_tb == nil or #sys_msg_tb <= 0 then
            sys_msg_tb = {}
            return 
        end
        --过了晚上1点更新一次
        local tomorrow = TimeStamp.timestamp_today() + 25*60*60
        Scheduler.once(get_sys_msg_tb,(tomorrow - sys_msg_version)*1000)
    end)
end

Scheduler.once(get_sys_msg_tb,2000)

--{stype,ctype,utag,body}
local function get_sys_message(s,req)
    local uid = req[3]
    local vernum = req[4].vernum
    if vernum == sys_msg_version then
        local msg = {Stype.System,Cmd.eGetSystemMessageRes,uid,{
            status = Respones.OK,
            vernum = sys_msg_version,
        }}
        Session.send_msg(s,msg)
        return
    end
    local msg = {Stype.System,Cmd.eGetSystemMessageRes,uid,{
        status = Respones.OK,
        vernum = sys_msg_version,
        info = sys_msg_tb,
    }}
    Session.send_msg(s,msg)
end

local sys_msg = {
    get_sys_message = get_sys_message,
}

return sys_msg