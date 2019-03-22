using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class user_login : Singletom<user_login>
{
    private string g_key = null;
    private bool is_save_gkey = false;
    public EditProfileReq editProfileReq;


    public void init()
    {
        network.Instance.add_service_listener((int)Stype.Auth, this.on_auth_server_return);
    }

    void on_auth_server_return(cmd_msg msg)
    {
        Debug.Log("------------------------------------------");
        Debug.Log("recv_msg_type=> " + msg.ctype);
        switch (msg.ctype)
        {
            case (int)Cmd.eGuestLoginRes:
                this.handle_guest_login_res(msg);
                break;
            case (int)Cmd.eEditProfileRes:
                this.handle_edit_profile_res(msg);
                break;
            case (int)Cmd.eAccountUpgradeRes:
                this.handle_accout_upgrade_res(msg);
                break;
            case (int)Cmd.eUnameLoginRes:
                this.handle_uname_login_res(msg);
                break;
            case (int)Cmd.eLoginOutRes:
                this._handle_login_out_res(msg);
                break;
            default:
                Debug.Log("recv_msg not register call function===> ctype=> "+msg.ctype);
                break;
        }
    }


    void handle_guest_login_res(cmd_msg msg)
    {
        GuestLoginRes res = proto_man.protobuf_deserialize<GuestLoginRes>(msg.body);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("Guest login status: " + res.status);
            return;
        }
        UserCenterInfo uinfo = res.info;
        ugames.Instance.save_uinfo(uinfo, true);

        if (this.is_save_gkey)
        {
            this.is_save_gkey = false;
            PlayerPrefs.SetString("login_guest_key", this.g_key);
        }
        event_manager.Instance.dispatch_event(event_manager.EVT_LOGIN_SUCCESS, uinfo);
    }

    void handle_uname_login_res(cmd_msg msg)
    {
        UnameLoginRes res = proto_man.protobuf_deserialize<UnameLoginRes>(msg.body);
        if (res == null)
        {
            return;
        }
        if (res.status != Respones.OK)
        {
            Debug.Log("uname login status: " + res.status);
            return;
        }
        UserCenterInfo uinfo = res.info;
        ugames.Instance.save_uinfo(uinfo, false);

        event_manager.Instance.dispatch_event(event_manager.EVT_LOGIN_SUCCESS, uinfo);
    }

    void handle_edit_profile_res(cmd_msg msg)
    {
        EditProfileRes e_res = proto_man.protobuf_deserialize<EditProfileRes>(msg.body);
        if (e_res == null)
        {
            return;
        }
        if (e_res.status != Respones.OK)
        {
            Debug.Log("profile status: " + e_res.status);
            return;
        }
        UserCenterInfo info = new UserCenterInfo();
        info.unick = this.editProfileReq.unick;
        info.face = this.editProfileReq.uface;
        info.sex = this.editProfileReq.usex;
        ugames.Instance.save_uinfo(info, ugames.Instance.is_guest);
    }

    void _handle_login_out_res(cmd_msg msg)
    {
        event_manager.Instance.dispatch_event(event_manager.EVT_USER_LOGIN_OUT, msg);
    }

    void handle_accout_upgrade_res(cmd_msg msg)
    {
        AccountUpgradeRes res = proto_man.protobuf_deserialize<AccountUpgradeRes>(msg.body);
        Debug.Log(res.status);
        if (res.status == Respones.OK)
        {
            ugames.Instance.is_guest = false;
            event_manager.Instance.dispatch_event(event_manager.EVT_ACCOUNT_UPGRADE, res);
        }
    }

   
    public void guest_login()
    {
        this.g_key = PlayerPrefs.GetString("login_guest_key");
        this.is_save_gkey = false;
        if (this.g_key == null || this.g_key.Length != 32)
        {
            this.g_key = utils.rand_str(32);
            this.is_save_gkey = true;
        }

        GuestLoginReq req = new GuestLoginReq();
        req.guestkey = this.g_key;
        network.Instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eGuestLoginReq, req);
    }

    public void uname_login(string uname, string pwd)
    {
        UnameLoginReq req = new UnameLoginReq();
        req.uname = uname;
        req.upwd = pwd;
        network.Instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eUnameLoginReq, req);
    }

    public void edit_profile(string unick,int uface,int usex)
    {
        if (unick.Length < 1) { return; }
        if (uface <= 0 || uface > 9) { return; }
        if (usex != 0 && usex != 1) { return; }
        EditProfileReq req = new EditProfileReq();
        req.unick = unick;
        req.uface = uface;
        req.usex = usex;
        this.editProfileReq = req;
        network.Instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eEditProfileReq,req);
    }

    public void do_account_upgrade(string uname, string upwd_md5)
    {
        AccountUpgradeReq req = new AccountUpgradeReq();
        req.uname = uname;
        req.upwdmd5 = upwd_md5;
        network.Instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eAccountUpgradeReq, req);
    }

    public void on_login_out()
    {
        network.Instance.send_protobuf_cmd((int)Stype.Auth, (int)Cmd.eLoginOutReq, null);
    }

}
