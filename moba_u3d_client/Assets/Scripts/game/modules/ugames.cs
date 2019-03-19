using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public class ugames : Singletom<ugames>
{
    public string unick = "";
    public int sex = 0;
    public int face = 1;
    public int vip = 0;
    public bool is_guest = false;

    public void save_uinfo(UserCenterInfo info, bool is_guest)
    {
        this.unick = info.unick;
        this.sex = info.sex;
        this.face = info.face;
        this.vip = info.uvip;
        this.is_guest = is_guest;
        event_manager.Instance.dispatch_event(event_manager.EVT_SET_MAIN_USER_INFO, info);
        
    }
    // Update is called once per frame
    void Update()
    {
        
    }
}
