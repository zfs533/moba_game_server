local game_config = require("game_config")
local mysql_conn = nil

function mysql_connect_to_auth_center()
    local auth_conf = game_config.auth_mysql
    Mysql.connect(auth_conf.host,auth_conf.port,auth_conf.db_name,auth_conf.uname,auth_conf.upwd,
    function(err,conn)
        if err then
            Logger.error(err) 
            Scheduler.once(mysql_connect_to_auth_center,5000)
            return
        end
        Logger.debug("connect to auth_center database success!!!")
        mysql_conn = conn
    end)
end

mysql_connect_to_auth_center()

--获取游客登录信息
function get_guest_uinfo(g_key, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!!!",nil)
        end
        return
    end
    local sql = "select uid, unick, usex, uface, uvip, status, is_guest from uinfo where guestkey = \"%s\" limit 1"
	local cmd_sql = string.format(sql, g_key)	
    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then
            if ret_handler ~= nil then
                ret_handler(err,nil)
            end
            return
        end
        --没有这条数据
        if ret == nil or #ret <= 0 then
            if ret_handler ~= nil then
                ret_handler(nil,nil)
            end
            return
        end
        --end
        local result = ret[1]
        local uinfo = {}
        uinfo.uid = tonumber(result[1])
        uinfo.unick = result[2]
        uinfo.usex = tonumber(result[3])
        uinfo.uface = tonumber(result[4])
        uinfo.uvip = tonumber(result[5])
        uinfo.status = tonumber(result[6])
        uinfo.is_guest = tonumber(result[7])
        ret_handler(nil,uinfo)
    end)

end

--插入游客信息
function insert_guest_user(g_key,ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!!!",nil)
        end
        return
    end
    local unick = "guest_"..math.random( 10000,99999)
    local uface = math.random(1,9)
    local usex = math.random(0,1)
    local sql = "insert into uinfo(`guestkey`, `unick`, `uface`, `usex`, `is_guest`)values(\"%s\", \"%s\", %d, %d, 1)"
	local cmd_sql = string.format(sql, g_key, unick, uface, usex)

    Mysql.query(mysql_conn,cmd_sql,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
            end
        else
            ret_handler(nil,nil)
        end
    end)
end

function edit_profile(uid,unick,uface,usex,ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!!!",nil)
        end
        return
    end
    local sql = "update uinfo set unick = \"%s\",usex = %d, uface = %d where uid = %d"
    local sql_cmd = string.format( sql,unick,usex,uface,uid)

    Mysql.query(mysql_conn,sql_cmd,function(err,ret) 
        if err then 
            if ret_handler then
                ret_handler(err,nil)
            end
        else
            ret_handler(nil,nil)
        end
    end)
end

local mysql_auth_center = 
{
    get_guest_uinfo = get_guest_uinfo,
    insert_guest_user = insert_guest_user,
    edit_profile = edit_profile,
}

return mysql_auth_center