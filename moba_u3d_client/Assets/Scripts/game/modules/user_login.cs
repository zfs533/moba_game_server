using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class user_login : Singletom<user_login>
{
    void on_auth_server_return(cmd_msg msg)
    {
        
    }
    public void init()
    {
        network.Instance.add_service_listener((int)Stype.Auth, this.on_auth_server_return);
    }

    public void guest_login()
    {
        string g_key = utils.rand_str(32);
        Debug.Log(g_key);

        GuestLoginReq req = new GuestLoginReq();
        req.guest_key = g_key;
        network.Instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eGuestLoginReq, req);
    }
}
