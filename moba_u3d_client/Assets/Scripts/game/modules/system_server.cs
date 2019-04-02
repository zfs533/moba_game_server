﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;
//system_service_proxy
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
        switch (msg.ctype)
        {
            case (int)Cmd.eGetUgameInfoRes:
                this.handle_get_ugame_info(msg);
                break;
            case (int)Cmd.eRecvLoginBonuesRes:
                this.handle_recv_login_bonues(msg);
                break;
            default:
                Debug.Log("recv_msg not register call function===> ctype=> " + msg.ctype);
                break;
        }
    }

    public void handle_recv_login_bonues(cmd_msg msg)
    {
        RecvLoginBonuesRes res = proto_man.protobuf_deserialize<RecvLoginBonuesRes>(msg.body);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("Guest login status: " + res.status);
            return;
        }
        ugames.Instance.ugameInfo.uchip += ugames.Instance.ugameInfo.bonues;
        ugames.Instance.ugameInfo.bonues_status = 1;
        event_manager.Instance.dispatch_event(event_manager.EVT_UPDATE_UGAME_INFO, res);
    }

    private void handle_get_ugame_info(cmd_msg msg)
    {
        GetUgameInfoRes res = proto_man.protobuf_deserialize<GetUgameInfoRes>(msg.body);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("Guest login status: " + res.status);
            return;
        }
        UserGameInfo info = res.info;
        ugames.Instance.save_ugame_info(info);
        event_manager.Instance.dispatch_event(event_manager.EVT_GET_UGAME_INFO_SUCCESS, info);
    }

    public void load_user_ugame_info()
    {
        network.Instance.send_protobuf_cmd((int)Stype.System, (int)Cmd.eGetUgameInfoReq, null);
    }

    public void recv_login_bonues()
    {
        network.Instance.send_protobuf_cmd((int)Stype.System, (int)Cmd.eRecvLoginBonuesReq, null);
    }
}
