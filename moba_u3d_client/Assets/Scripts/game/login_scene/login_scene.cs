using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using gprotocol;
using UnityEngine.UI;

public class login_scene : MonoBehaviour
{
    // Start is called before the first frame update
    public InputField uname_edit;
    public InputField upwd_edit;
    void Start()
    {
        event_manager.Instance.add_event_listener(event_manager.EVT_LOGIN_SUCCESS, this.evt_login_success);
        event_manager.Instance.add_event_listener(event_manager.EVT_GET_UGAME_INFO_SUCCESS, this.evt_get_ugame_info);
        event_manager.Instance.add_event_listener(event_manager.EVT_LOGIN_LOGIC_SERVER_SUCCESS, this.evt_login_logic_server);
    }
    
    void evt_login_success(string name, object udata)
    {
        system_server.Instance.load_user_ugame_info();
    }
    void evt_get_ugame_info(string name, object udata)
    {
//        SceneManager.LoadScene("home_scene");
        logic_service.Instance.login_logic_server();
    }
    void evt_login_logic_server(string name, object udata)
    {
        SceneManager.LoadScene("home_scene");
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public void on_guest_login_click()
    {
        user_login.Instance.guest_login();
    }
    public void on_uname_pwd_login_click()
    {
        string name = this.uname_edit.text;
        string pwd = this.upwd_edit.text;
        Debug.Log(name + "--" + pwd);
        if (name.Length <= 0 || pwd.Length <= 0)
        {
            return;
        }
        pwd = utils.md5(pwd);
        Debug.Log(name + "--" + pwd);
        user_login.Instance.uname_login(name, pwd);
    }

    void OnDestroy()
    {
        event_manager.Instance.remove_event_listener(event_manager.EVT_LOGIN_SUCCESS, this.evt_login_success);
        event_manager.Instance.remove_event_listener(event_manager.EVT_GET_UGAME_INFO_SUCCESS, this.evt_get_ugame_info);
        event_manager.Instance.remove_event_listener(event_manager.EVT_LOGIN_LOGIC_SERVER_SUCCESS, this.evt_login_logic_server);
    }
}
