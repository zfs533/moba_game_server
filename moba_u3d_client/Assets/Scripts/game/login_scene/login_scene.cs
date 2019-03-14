using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class login_scene : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
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
