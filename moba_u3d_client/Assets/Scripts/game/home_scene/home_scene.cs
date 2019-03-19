using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class home_scene : MonoBehaviour
{
    public Text uname;
    public Sprite[] faces;
    public Image uface;
    public Text uvip;

    private string unick = "";
    private int sex = 0;
    private int face = 1;
    private int vip = 0;
    public GameObject uinfo_dlg_prefab;

    void Start()
    {
        event_manager.Instance.add_event_listener(event_manager.EVT_SET_MAIN_USER_INFO, this.init_user_info);
        this.init_user_info(event_manager.EVT_SET_MAIN_USER_INFO, null);
    }
    private void init_user_info(string name,object data)
    {
        if (ugames.Instance.unick!=null && ugames.Instance.unick.Length > 0)
        {
            this.unick = ugames.Instance.unick;
        }
        Debug.Log("change palyer info");
        this.sex = ugames.Instance.sex;
        this.face = ugames.Instance.face;
        this.vip = ugames.Instance.vip;
        this.uname.text = this.unick;
        this.uvip.text ="Lv" + this.vip.ToString();
        this.uface.sprite = this.faces[this.face - 1];
    }

    public void on_uinfo_click()
    {
        GameObject uinfo_dlg = GameObject.Instantiate(this.uinfo_dlg_prefab);
        uinfo_dlg.transform.SetParent(this.transform, false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    void OnDestroy()
    {
        event_manager.Instance.remove_event_listener(event_manager.EVT_SET_MAIN_USER_INFO, this.init_user_info);
    }
}
