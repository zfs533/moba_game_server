using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class home_scene : MonoBehaviour
{
    public Text coin_label;
    public Text diamond_label;

    public Text uname;
    public Sprite[] faces;
    public Image uface;
    public Text uvip;
    public Text level_exp;
    public Image level_progress;

    private string unick = "";
    private int sex = 0;
    private int face = 1;
    private int vip = 0;
    public GameObject uinfo_dlg_prefab;
    public GameObject login_bonues;

    void Start()
    {
        event_manager.Instance.add_event_listener(event_manager.EVT_SET_MAIN_USER_INFO, this.init_user_info);
        event_manager.Instance.add_event_listener(event_manager.EVT_USER_LOGIN_OUT, this.evt_user_login_out);
        event_manager.Instance.add_event_listener(event_manager.EVT_UPDATE_UGAME_INFO, this.evt_update_ugame_info);
        this.init_user_info(event_manager.EVT_SET_MAIN_USER_INFO, null);
        this.evt_update_ugame_info(event_manager.EVT_UPDATE_UGAME_INFO, null);
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
        this.uface.sprite = this.faces[this.face - 1];
    }

    private void evt_update_ugame_info(string name , object data)
    {
        if (this.coin_label != null)
        {
            this.coin_label.text = ugames.Instance.ugameInfo.uchip + "";
        }
        if (this.diamond_label != null)
        {
            this.diamond_label.text = ugames.Instance.ugameInfo.uchip2 + "";
        }
        int now_exp = 0;
        int next_level_exp = 0;
        int exp = ugames.Instance.ugameInfo.uexp;
        int cur_level = ulevel.Instance.get_level_exp(exp, out now_exp,out next_level_exp);
        if (this.uvip != null)
        {
            this.uvip.text = "Lv\n" + cur_level;
        }
        if (this.level_exp != null)
        {
            this.level_exp.text = now_exp + " / " + next_level_exp;
        }
        if (this.level_progress != null)
        { 
            this.level_progress.fillAmount = (float)now_exp /(float)next_level_exp;
        }
        //login bonues info
        if (ugames.Instance.ugameInfo.bonues_status == 0)
        {
            this.login_bonues.SetActive(true);
            this.login_bonues.GetComponent<login_bonues>().show_login_bonues(ugames.Instance.ugameInfo.days);
        }
        else
        {
            this.login_bonues.SetActive(false);
        }
    }

    public void on_uinfo_click()
    {
        GameObject uinfo_dlg = GameObject.Instantiate(this.uinfo_dlg_prefab);
        uinfo_dlg.transform.SetParent(this.transform, false);
    }

    private void evt_user_login_out(string name, object data)
    {
        PlayerPrefs.SetString("login_guest_key", "");
        SceneManager.LoadScene("login");
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    void OnDestroy()
    {
        event_manager.Instance.remove_event_listener(event_manager.EVT_SET_MAIN_USER_INFO, this.init_user_info);
        event_manager.Instance.remove_event_listener(event_manager.EVT_USER_LOGIN_OUT, this.evt_user_login_out);
        event_manager.Instance.remove_event_listener(event_manager.EVT_UPDATE_UGAME_INFO, this.evt_update_ugame_info);
    }
}
