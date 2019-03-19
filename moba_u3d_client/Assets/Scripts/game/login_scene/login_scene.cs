using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using gprotocol;

public class login_scene : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        event_manager.Instance.add_event_listener(event_manager.EVT_LOGIN_SUCCESS, this.on_login_success);
    }

    void on_login_success(string name, object udata)
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
}
