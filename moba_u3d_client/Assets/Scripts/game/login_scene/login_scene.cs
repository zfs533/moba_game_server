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
        event_manager.Instance.add_event_listener(event_manager.EVT_LOGIN_SUCCESS, this.on_login_success);
    }

    void on_login_success(string name, object udata)
    {
        //SceneManager.LoadScene("home_scene");
        system_server.Instance.load_user_ugame_info();
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
}
