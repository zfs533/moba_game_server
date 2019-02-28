using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class main_game_scene : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        //test
        this.Invoke("test", 5);
        network.instance.add_service_listener(1, this.on_service_event);
    }

    void on_service_event(cmd_msg msg)
    {
        switch (msg.ctype)
        {
            case 2:
                gprotocol.LoginRes res = proto_man.protobuf_deserialize<gprotocol.LoginRes>(msg.body);
                Debug.Log("listener----------------------------------------------------------------res = " + res.status);
            break;
        }
        
    }

    void OnDestroy()
    {
        if (network.instance)
        {
            network.instance.remove_service_listener(1, this.on_service_event);
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void test()
    {
        gprotocol.LoginReq req = new gprotocol.LoginReq();
        req.name = "test_name";
        req.email = "2415933434@qq.com";
        req.age = 28;
        req.int_set = 8;
        network.instance.send_protobuf_cmd(1, 1, req);
    }
}
