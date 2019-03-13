using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class game_scene : MonoBehaviour
{
    private void on_auth_server_return(cmd_msg msg)
    {
        //LoginRes res = proto_man.protobuf_deserialize<LoginRes>(body);
        LoginRes res = proto_man.protobuf_deserialize<LoginRes>(msg.body);
        Debug.Log(msg.stype + "--" + msg.ctype + "--" + res.status);
        event_manager.Instance.dispatch_event(event_manager.EVENT_TEST, msg);
    }

    private void event_test_handler(string name, object msg)
    {
        Debug.Log("event_name= " + name);
        cmd_msg msgg = (cmd_msg)msg;
        Debug.Log(msgg.stype + "--" + msgg.ctype);
    }

    // Start is called before the first frame update
    void Start()
    {
        network.Instance.add_service_listener((int)Stype.Auth, this.on_auth_server_return);
        event_manager.Instance.add_event_listener(event_manager.EVENT_TEST, this.event_test_handler);
        this.Invoke("test", 3.0f);
    }

    void test()
    {
        Debug.Log("test-------");

        network.Instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eLoginReq, null);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

}
