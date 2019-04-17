using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class team_match : MonoBehaviour
{
    public void on_begin_match_click()
    {
        int zid = ugames.Instance.zid;
        logic_service.Instance.enter_zone(zid);
    }
}
