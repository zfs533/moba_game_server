using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class game_scene : MonoBehaviour
{
    private void on_auth_server_return(cmd_msg msg)
    {
        
    }

    // Start is called before the first frame update
    void Start()
    {
        network.instance.add_service_listener((int)Stype.Auth, this.on_auth_server_return);
        this.Invoke("test", 3.0f);
    }

    void test()
    {
        Debug.Log("test-------");
        network.instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eLoginReq, null);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

}
