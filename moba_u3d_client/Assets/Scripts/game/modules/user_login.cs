using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class user_login : Singletom<user_login>
{
    void on_auth_server_return(cmd_msg msg)
    {
        GuestLoginRes res = proto_man.protobuf_deserialize<GuestLoginRes>(msg.body);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("Guest login status: " + res.status);
            return;
        }
        UserCenterInfo uinfo = res.info;
        Debug.Log(uinfo.unick + " - " + uinfo.sex);
    }
    public void init()
    {
        network.Instance.add_service_listener((int)Stype.Auth, this.on_auth_server_return);
    }

    public void guest_login()
    {
        string g_key = utils.rand_str(32);
        g_key = "8JvrDstUNDuTNnnCKFEw4pKFs27z9xSr";
        Debug.Log(g_key);

        GuestLoginReq req = new GuestLoginReq();
        req.guestkey = g_key;
        network.Instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eGuestLoginReq, req);
    }
}
