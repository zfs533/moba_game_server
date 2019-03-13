using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using gprotocol;

public class talkroom : MonoBehaviour
{
    public ScrollRect scroll_view;
    public GameObject status_prefab;
    public GameObject talk_prefab;
    public GameObject self_talk_prefab;
    public InputField input;
    public int count = 1;
    private string send_msg = "";

    void on_login_return(byte[] body)
    {
        LoginRes res = proto_man.protobuf_deserialize<LoginRes>(body);
        if (res.status == 1)
        {
            this.add_status_option("你成功进入聊天室,..");
        }
        else if (res.status == -1)
        {
            this.add_status_option("你已经在聊天室了..");
        }
    }

    void on_exit_return(byte[] body)
    {
        ExitRes res = proto_man.protobuf_deserialize<ExitRes>(body);
        if (res.status == 1)
        {
            this.add_status_option("你离开聊天室!");
        }
        else if (res.status == -1)
        {
            this.add_status_option("你早已不在聊天室了!");
        }
    }

    void on_send_msg_return(byte[] body)
    {
        SendMsgRes res = proto_man.protobuf_deserialize<SendMsgRes>(body);
        if (res.status == "1")
        {
            this.add_self_option(this.send_msg);
        }
        else if (res.status == "-1")
        {
            this.add_status_option("你不在聊天室!");
        }
    }

    void on_other_user_enter(byte[] body)
    {
        OnUserLogin res = proto_man.protobuf_deserialize<OnUserLogin>(body);
        this.add_status_option(res.ip + ":" + res.port + "进入聊天室!");
    }

    void on_other_user_exit(byte[] body)
    {
        OnUserLogin res = proto_man.protobuf_deserialize<OnUserLogin>(body);
        this.add_status_option(res.ip + ":" + res.port + "离开聊天室!");
    }

    void on_other_user_send_msg(byte[] body)
    {
        OnSendMsg res = proto_man.protobuf_deserialize<OnSendMsg>(body);
        this.add_talk_option(res.ip, res.port, res.content);
    }


    void on_trm_server_return(cmd_msg msg)
    {
        switch (msg.ctype)
        { 
            case (int)Cmd.eLoginRes:
                this.on_login_return(msg.body);
                break;
            case (int)Cmd.eExitRes:
                this.on_exit_return(msg.body);
                break;
            case (int)Cmd.eSendMsgRes:
                this.on_send_msg_return(msg.body);
                break;
            case (int)Cmd.eOnSendMsg:
                this.on_other_user_send_msg(msg.body);
                break;
            case (int)Cmd.eOnUserExit:
                this.on_other_user_exit(msg.body);
                break;
            case (int)Cmd.eOnUserLogin:
                this.on_other_user_enter(msg.body);
                break;
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        network.Instance.add_service_listener(1, this.on_trm_server_return);
    }

    void test()
    {

    }
    // Update is called once per frame
    void Update()
    {
        
    }
    void add_status_option(string status_info)
    {
        GameObject opt = GameObject.Instantiate(this.status_prefab);
        opt.transform.SetParent(this.scroll_view.content, false);
        Vector2 content_size = this.scroll_view.content.sizeDelta;
        content_size.y += 120;
        this.scroll_view.content.sizeDelta = content_size;
        opt.transform.Find("src").GetComponent<Text>().text = status_info;
        Vector3 local_pos = this.scroll_view.content.localPosition;
        local_pos.y += 120;
        this.scroll_view.content.localPosition = local_pos;
    }

    void add_talk_option(string ip, int port, string body)
    {
        GameObject opt = GameObject.Instantiate(this.talk_prefab);
        opt.transform.SetParent(this.scroll_view.content, false);
        Vector2 content_size = this.scroll_view.content.sizeDelta;
        content_size.y += 120;
        this.scroll_view.content.sizeDelta = content_size;
        opt.transform.Find("src").GetComponent<Text>().text = body;
        opt.transform.Find("ip").GetComponent<Text>().text = ip + " : " + port;
        Vector3 local_pos = this.scroll_view.content.localPosition;
        local_pos.y += 120;
        this.scroll_view.content.localPosition = local_pos;
    }
    void add_self_option(string body)
    {
        GameObject opt = GameObject.Instantiate(this.self_talk_prefab);
        opt.transform.SetParent(this.scroll_view.content, false);

        Vector2 content_size = this.scroll_view.content.sizeDelta;
        content_size.y += 120;
        this.scroll_view.content.sizeDelta = content_size;

        opt.transform.Find("src").GetComponent<Text>().text = body;

        Vector3 local_pos = this.scroll_view.content.localPosition;
        local_pos.y += 120;
        this.scroll_view.content.localPosition = local_pos;
    }
    public void on_enter_talkroom()
    {
        network.Instance.send_protobuf_cmd(1, (int)Cmd.eLoginReq, null);
    }

    public void on_exit_talkroom()
    {
        network.Instance.send_protobuf_cmd(1, (int)Cmd.eExitReq, null);
    }

    public void on_send_msg() 
    {
        if (this.input.text.Length <= 0)
        {
            return;
        }
        SendMsgReq req = new SendMsgReq();
        req.content = this.input.text;
        this.send_msg = this.input.text;
        network.Instance.send_protobuf_cmd(1, (int)Cmd.eSendMsgReq, req);
    }
}
