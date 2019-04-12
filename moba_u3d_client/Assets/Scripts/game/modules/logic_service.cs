using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;
public class logic_service : Singletom<logic_service>
{
    public void init()
    {
        network.Instance.add_service_listener((int)Stype.Logic, this.on_logic_server_return);
    }
    
    void on_logic_server_return(cmd_msg msg)
    {
        Debug.Log("logic server ctype: " + msg.ctype);
        switch (msg.ctype)
        {
            case (int)Cmd.eLoginLogicRes:
                this.handle_login_logic_res(msg);
                break;
        }
        
    }
    
    void handle_login_logic_res(cmd_msg msg)
    {
        LoginLogicRes res = proto_man.protobuf_deserialize<LoginLogicRes>(msg.body);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("login logic server status: " + res.status);
            return;
        }
        //登录逻辑服务器成功
        event_manager.Instance.dispatch_event(event_manager.EVT_LOGIN_LOGIC_SERVER_SUCCESS, null);
    }

    public void login_logic_server()
    {
        network.Instance.send_protobuf_cmd((int)Stype.Logic, (int)Cmd.eLoginLogicReq, null);
    }
}
