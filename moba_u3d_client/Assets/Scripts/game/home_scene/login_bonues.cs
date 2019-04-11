using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class login_bonues : MonoBehaviour
{
    public GameObject[] fingerprint;
    public GameObject recv_button;
    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void show_login_bonues(int days)
    {
        int i;
        for (i = 0; i < days; i++)
        {
            this.fingerprint[i].SetActive(true);
        }
        for (; i < 5; i++)
        {
            this.fingerprint[i].SetActive(false);
        }
        if (ugames.Instance.ugameInfo.bonues_status == 0)
        {
            this.recv_button.SetActive(true);
        }
        else
        {
            this.recv_button.SetActive(false);
        }
    }
    public void on_recv_login_bonues_click()
    {
        this.on_close_login_bonues();
        system_server.Instance.recv_login_bonues();
    }

    public void on_close_login_bonues()
    {
        GameObject.Destroy(this.gameObject);
    }
}
