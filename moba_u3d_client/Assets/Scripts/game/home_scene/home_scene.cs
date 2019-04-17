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
    public GameObject rank_list_prefab;
    public GameObject sys_email_prefab;
    public GameObject match_prefab;
    public GameObject home_page;
    public GameObject war_page;
    public Sprite[] btn_normal;
    public Sprite[] btn_selected;
    public Image[] tab_bottom_btns;

    void Start()
    {
        event_manager.Instance.add_event_listener(event_manager.EVT_SET_MAIN_USER_INFO, this.init_user_info);
        event_manager.Instance.add_event_listener(event_manager.EVT_USER_LOGIN_OUT, this.evt_user_login_out);
        event_manager.Instance.add_event_listener(event_manager.EVT_UPDATE_UGAME_INFO, this.evt_update_ugame_info);
        this.init_user_info(event_manager.EVT_SET_MAIN_USER_INFO, null);
        this.evt_update_ugame_info(event_manager.EVT_UPDATE_UGAME_INFO, null);
        this.show_home_page_ui();
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
            GameObject login_bonues = GameObject.Instantiate(this.login_bonues);
            login_bonues.SetActive(true);
            login_bonues.transform.SetParent(this.transform, false);
            login_bonues.GetComponent<login_bonues>().show_login_bonues(ugames.Instance.ugameInfo.days);
        }
    }
    public void show_login_bonues_ui()
    {
        GameObject login_bonues = GameObject.Instantiate(this.login_bonues);
        login_bonues.SetActive(true);
        login_bonues.transform.SetParent(this.transform, false);
        login_bonues.GetComponent<login_bonues>().show_login_bonues(ugames.Instance.ugameInfo.days);
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

    public void show_game_rank_ui()
    {
        GameObject rank_list = GameObject.Instantiate(this.rank_list_prefab);
        rank_list.transform.SetParent(this.transform, false);
    }

    public void show_sys_email_ui()
    {
        GameObject sys_email = GameObject.Instantiate(this.sys_email_prefab);
        sys_email.transform.SetParent(this.transform, false);
    }

    public void show_home_page_ui()
    {
        this.home_page.SetActive(true);
        this.war_page.SetActive(false);
        this.tab_bottom_btns[0].sprite = this.btn_selected[0];
        this.tab_bottom_btns[1].sprite = this.btn_normal[1];
    }

    public void show_war_page_ui()
    {
        this.home_page.SetActive(false);
        this.war_page.SetActive(true);
        this.tab_bottom_btns[0].sprite = this.btn_normal[0];
        this.tab_bottom_btns[1].sprite = this.btn_selected[1];
    }

    public void math_sgyd()
    {
        ugames.Instance.set_zid(Zone.SGYD);
        GameObject math = GameObject.Instantiate(this.match_prefab);
        math.transform.SetParent(this.transform, false);
    }

    public void math_assy()
    {
        ugames.Instance.set_zid(Zone.ASSY);
        GameObject math = GameObject.Instantiate(this.match_prefab);
        math.transform.SetParent(this.transform, false);
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
