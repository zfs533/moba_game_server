using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class event_manager : Singletom<event_manager>
{
    public static string EVT_LOGIN_SUCCESS = "login_success";
    public static string EVT_UPDATE_USERINFO = "update_userinfo";
    public static string EVT_SET_MAIN_USER_INFO = "set_main_user_info";
    public static string EVT_ACCOUNT_UPGRADE = "account_apgrade";
    public static string EVT_USER_LOGIN_OUT = "user_login_out";

    public static string EVT_GET_UGAME_INFO_SUCCESS = "get_ugame_info_success";
    public static string EVT_UPDATE_UGAME_INFO = "updata_ugame_info";
    public static string EVT_GET_RANK_LIST = "get_rank_list";
    //-----------------------------------------------------
    public static string EVENT_TEST = "event_test";

    public delegate void on_event_handler(string name, object udata);
    private Dictionary<string, on_event_handler> event_listeners = new Dictionary<string, on_event_handler>();

    public void init()
    { 
    }
    //订阅/注册事件
    public void add_event_listener(string name, on_event_handler handler)
    {
        if (this.event_listeners.ContainsKey(name))
        {
            this.event_listeners[name] += handler;
        }
        else
        {
            this.event_listeners.Add(name, handler);
        }
    }
    public void remove_event_listener(string name, on_event_handler handler)
    {
        if (!this.event_listeners.ContainsKey(name))
        {
            return;
        }
        this.event_listeners[name] -= handler;
        if(this.event_listeners[name] == null)
        {
            this.event_listeners.Remove(name);
        }
    }

    //触发事件
    public void dispatch_event(string name, object udata)
    {
        if (!this.event_listeners.ContainsKey(name))
        {
            return;
        }
        if (this.event_listeners[name] != null)
        {
            this.event_listeners[name](name, udata);
        }
    }
}