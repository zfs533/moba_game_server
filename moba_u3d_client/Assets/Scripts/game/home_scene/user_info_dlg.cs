using System.Collections;

using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class user_info_dlg : MonoBehaviour {
    public InputField unick_edit;
    public GameObject guest_upgrade = null;
    public Image avator_img = null;
    public Sprite[] avator_sprites = null;
    public GameObject womale_check = null;
    public GameObject male_check = null;

    public GameObject face_dlg = null;

    public GameObject account_ugrade_dlg = null;
    public InputField uname_edit;
    public InputField upwd_edit;
    public InputField upwd_again;

    private int usex = 0;
    private int uface = 1;
    
	// Use this for initialization
	void Start () {
        event_manager.Instance.add_event_listener(event_manager.EVT_ACCOUNT_UPGRADE, this.handler_upgrade_success);
        
		// 是否显示账号升级的按钮，如果你是游客，那么就要显示;
        if (ugames.Instance.is_guest) {
            this.guest_upgrade.SetActive(true);
        }
        else {
            this.guest_upgrade.SetActive(false);
        }
        // end

        // unick
        if (ugames.Instance.unick != null) {
            this.unick_edit.text = ugames.Instance.unick;
        }

        // uface
        if (ugames.Instance.uface >= 1 && ugames.Instance.uface <= 9) {
            this.uface = ugames.Instance.uface;
        }
        this.avator_img.sprite = this.avator_sprites[this.uface - 1];

        // usex
        if (ugames.Instance.usex == 0 || ugames.Instance.usex == 1) {
            this.usex = ugames.Instance.usex;
        }
        if (this.usex == 0) { // man
            this.male_check.SetActive(true);
            this.womale_check.SetActive(false);
        }
        else {
            this.male_check.SetActive(false);
            this.womale_check.SetActive(true);
        }
	}

    public void on_show_account_upgrade() {
        this.account_ugrade_dlg.SetActive(true);
    }

    public void on_hide_account_upgrade() {
        this.account_ugrade_dlg.SetActive(false);
    }

    public void on_do_account_upgrade() {
        if (!ugames.Instance.is_guest) {
            return;
        }

        if (this.uname_edit.text.Length <= 0 || this.upwd_edit.text.Length <= 0 ||
            !this.upwd_edit.text.Equals(this.upwd_again.text)) {
            return;
        }

        string md5_pwd = utils.md5(this.upwd_edit.text);
        user_login.Instance.do_account_upgrade(this.uname_edit.text, md5_pwd);
    }

    public void on_sex_change(int usex) {
        this.usex = usex;
        if (this.usex == 0) { // man
            this.male_check.SetActive(true);
            this.womale_check.SetActive(false);
        }
        else
        {
            this.male_check.SetActive(false);
            this.womale_check.SetActive(true);
        }
    }

    public void on_avator_click() {
        this.face_dlg.SetActive(true);
    }

    public void on_uface_select_close() {
        this.face_dlg.SetActive(false);
    }

    public void on_uface_select_click(int uface) {
        this.uface = uface;
        this.avator_img.sprite = this.avator_sprites[this.uface - 1];
        this.face_dlg.SetActive(false);
    }

    public void on_close_uinfo_dlg_click() {
        // this.gameObject.SetActive(false);
        GameObject.Destroy(this.gameObject);
    }

    public void on_editprofile_commit() {
        Debug.Log("on_editprofile_commit called" + this.unick_edit.text);
        // unick, usex, uface
        if (this.unick_edit.text.Length <= 0) {
            this.on_close_uinfo_dlg_click();
            return;
        }

        Debug.Log(this.unick_edit.text + " " + this.uface + " " + this.usex);
        // 提交修改资料请求到服务器
        user_login.Instance.edit_profile(this.unick_edit.text, this.uface, this.usex);
        // end 

        this.on_close_uinfo_dlg_click();
    }
    void handler_upgrade_success(string name, object info)
    {
        this.on_hide_account_upgrade();
        this.guest_upgrade.SetActive(false);
    }
	// Update is called once per frame
	void Update () {
		
	}
    void OnDestroy()
    {
        event_manager.Instance.remove_event_listener(event_manager.EVT_ACCOUNT_UPGRADE, this.handler_upgrade_success);
    }
    public void on_user_login_out()
    {
        user_login.Instance.on_login_out();
    }
}
