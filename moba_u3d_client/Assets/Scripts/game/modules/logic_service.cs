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
            case (int)Cmd.eEnterZoneRes:
                this.handle_enter_zone_res(msg);
                break;
            case (int)Cmd.eEnterMatch:
                this.handle_enter_match(msg);
                break;
            case (int)Cmd.eUserArrived:
                this.handle_user_arrived(msg);
                break;
            case (int)Cmd.eExitMatchRes:
                this.handle_user_exit_match_res(msg);
                break;
            case (int)Cmd.eUserExitMatch:
                this.handle_other_exit_match_res(msg);
                break;
            case (int)Cmd.eGameStart:
                this.handle_game_start_res(msg);
                break;
            case (int)Cmd.eUdpTest:
                this.handle_udp_test(msg);
                break;
            case (int)Cmd.eLogicFrame:
                this.handle_logic_frame(msg);
                break;
        }
        
    }

    void handle_logic_frame(cmd_msg msg)
    {
        LogicFrame res = proto_man.protobuf_deserialize<LogicFrame>(msg.body);
        if (res == null)
        {
            return;
        }
        event_manager.Instance.dispatch_event(event_manager.EVT_LOGIC_FRAME_UPDATE, res);
    }

    void handle_udp_test(cmd_msg msg)
    {
        UdpTest res = proto_man.protobuf_deserialize<UdpTest>(msg.body);
        if (res == null)
        {
            return;
        }
        Debug.Log("server return " + res.content);
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

    void handle_enter_zone_res(cmd_msg msg)
    {
        EnterZoneRes res = proto_man.protobuf_deserialize<EnterZoneRes>(msg.body);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("enter zone status: " + res.status);
            return;
        }
    }

    void handle_enter_match(cmd_msg msg)
    {
        EnterMatch res = proto_man.protobuf_deserialize<EnterMatch>(msg.body);
        if (res == null)
        {
            return;
        }
        Debug.Log("enter match: " + res.zid + "  " + res.matchid);
        ugames.Instance.zid = res.zid;
        ugames.Instance.matchid = res.matchid;
        ugames.Instance.self_seatid = res.seatid;
        ugames.Instance.self_side = res.side;
    }

    void handle_user_arrived(cmd_msg msg)
    {
        UserArrived res = proto_man.protobuf_deserialize<UserArrived>(msg.body);
        if (res == null)
        {
            return;
        }
        Debug.Log("user arrived " + res.unick + "  " + res.uface + "  " + res.usex);
        ugames.Instance.other_users.Add(res);
        event_manager.Instance.dispatch_event(event_manager.EVT_USER_ARRIVIED_MATCH, res);
    }

    void handle_user_exit_match_res(cmd_msg msg)
    {
        ExitMatchRes res = proto_man.protobuf_deserialize<ExitMatchRes>(msg.body);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("handle_user_exit_match_res: " + res.status);
            return;
        }
        ugames.Instance.zid = -1;
        event_manager.Instance.dispatch_event(event_manager.EVT_USER_EXIT_MATCH_SUCCESS, res);
    }

    void handle_other_exit_match_res(cmd_msg msg)
    {
        UserExitMatch res = proto_man.protobuf_deserialize<UserExitMatch>(msg.body);
        if (res == null)
        {
            return;
        }
        int seatid = res.seatid;
        for (int i = 0; i < ugames.Instance.other_users.Count; i++)
        {
            if (ugames.Instance.other_users[i].seatid == seatid)
            {
                event_manager.Instance.dispatch_event(event_manager.EVT_OTHER_EXIT_MATCH_SUCCESS, i);
                ugames.Instance.other_users.RemoveAt(i);
                return;
            }
        }
    }

    void handle_game_start_res(cmd_msg msg)
    {
        Debug.Log("game_start_res");
        GameStart res = proto_man.protobuf_deserialize<GameStart>(msg.body);
        if (res == null)
        {
            return;
        }
        ugames.Instance.player_math_info = res.playerMathInfo;
        event_manager.Instance.dispatch_event(event_manager.EVT_GAME_START, null);
    }

    public void login_logic_server()
    {
        LoginLogicReq req = new LoginLogicReq();
        req.udp_ip = "127.0.0.1";
        req.udp_port = network.Instance.local_udp_port;
        network.Instance.send_protobuf_cmd((int)Stype.Logic, (int)Cmd.eLoginLogicReq, req);
    }
    public void enter_zone(int zid)
    {
        if (zid < Zone.SGYD || zid > Zone.ASSY)
        {
            return;
        }
        EnterZoneReq req = new EnterZoneReq();
        req.zid = zid;
        network.Instance.send_protobuf_cmd((int)Stype.Logic, (int)Cmd.eEnterZoneReq, req);
    }
    public void on_exit_match_list()
    {
        network.Instance.send_protobuf_cmd((int)Stype.Logic, (int)Cmd.eExitMatchReq, null);
    }

    public void send_udp_test(string content)
    {
        UdpTest req = new UdpTest();
        req.content = content;
        network.Instance.udp_send_protobuf_cmd((int)Stype.Logic, (int)Cmd.eUdpTest, req);
    }

    public void send_next_frame_opts(NextFrameOpts next_frame)
    {
        network.Instance.udp_send_protobuf_cmd((int)Stype.Logic, (int)Cmd.eNextFrameOpts, next_frame);
    }
}
