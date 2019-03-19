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

    private int usex = 0;
    private int uface = 1;
    
	// Use this for initialization
	void Start () {
		// 是否显示账号升级的按钮，如果你是游客，那么就要显示;
        if (ugames.Instance.is_guest) {
            this.guest_upgrade.SetActive(true);
        }
        else {
            this.guest_upgrade.SetActive(false);
        }
        // end

        // unick
        if (ugames.Instance.unick != null)
        {
            this.unick_edit.text = ugames.Instance.unick;
        }

        // uface
        if (ugames.Instance.face >= 1 && ugames.Instance.face <= 9)
        {
            this.uface = ugames.Instance.face;
        }
        this.avator_img.sprite = this.avator_sprites[this.uface - 1];

        // usex
        if (ugames.Instance.sex == 0 || ugames.Instance.sex == 1)
        {
            this.usex = ugames.Instance.sex;
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

	// Update is called once per frame
	void Update () {
		
	}
}
