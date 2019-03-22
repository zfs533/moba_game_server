using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class system_server : Singletom<system_server>
{
    public void init()
    {
        network.Instance.add_service_listener((int)Stype.System, this.on_system_server_return);
    }
    void on_system_server_return(cmd_msg msg)
    {
        Debug.Log("------------------------------------------");
        Debug.Log("recv_msg_system_type=> " + msg.ctype);
    }
    public void load_user_ugame_info()
    {
        network.Instance.send_protobuf_cmd((int)Stype.System, (int)Cmd.eGetUgameInfoReq, null);
    }
}
