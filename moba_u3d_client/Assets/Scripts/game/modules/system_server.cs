using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;
//system_service_proxy
public class system_server : Singletom<system_server>
{
    private int sys_msg_version = 0;
    private List<string> sys_msgs = null;
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
            case (int)Cmd.eGetWorldRankUchipRes:
                this.handle_recv_world_rank(msg);
                break;
            case (int)Cmd.eGetSystemMessageRes:
                this.handle_recv_sys_msg(msg);
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

    public void handle_recv_world_rank(cmd_msg msg)
    {
        GetWorldRankUchipRes res = proto_man.protobuf_deserialize<GetWorldRankUchipRes>(msg.body);
        Debug.Log(res);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("world_rank_status " + res.status);
            return;
        }
        List<WorldChipRankInfo> list = res.rankinfo;
        event_manager.Instance.dispatch_event(event_manager.EVT_GET_RANK_LIST, list);
    }

    public void handle_recv_sys_msg(cmd_msg msg)
    {
        GetSystemMessageRes res = proto_man.protobuf_deserialize<GetSystemMessageRes>(msg.body);
        Debug.Log(res);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("sys_msg_status: " + res.status);
            return;
        }
        if (this.sys_msg_version == res.vernum)
        {
            Debug.Log("use the local data");
        }
        else
        {
            this.sys_msg_version = res.vernum;
            this.sys_msgs = res.info;//List<string> info = res.info;
        }
        event_manager.Instance.dispatch_event(event_manager.EVT_GET_SYS_EMAIL, this.sys_msgs);
    }

    public void load_user_ugame_info()
    {
        network.Instance.send_protobuf_cmd((int)Stype.System, (int)Cmd.eGetUgameInfoReq, null);
    }

    public void recv_login_bonues()
    {
        network.Instance.send_protobuf_cmd((int)Stype.System, (int)Cmd.eRecvLoginBonuesReq, null);
    }

    public void game_rank_test()
    {
        GetWorldRankUchipReq req = new GetWorldRankUchipReq();
        req.ranknum = 10;
        network.Instance.send_protobuf_cmd((int)Stype.System, (int)Cmd.eGetWorldRankUchipReq, req);
    }
    public void get_sys_msg_info()
    {
        GetSystemMessageReq req = new GetSystemMessageReq();
        req.vernum = this.sys_msg_version;
        network.Instance.send_protobuf_cmd((int)Stype.System, (int)Cmd.eGetSystemMessageReq, req);
    }
}
