using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using gprotocol;

public class team_match : MonoBehaviour
{
    public ScrollRect scrollview;
    public GameObject opt_prefab;
    public Sprite[] uface_img = null;
    private int member_count;

    void Start()
    {
        this.member_count = 0;
        event_manager.Instance.add_event_listener(event_manager.EVT_USER_ARRIVIED_MATCH, this.on_user_arrived);
        event_manager.Instance.add_event_listener(event_manager.EVT_USER_EXIT_MATCH_SUCCESS, this.on_user_exit_success);
        event_manager.Instance.add_event_listener(event_manager.EVT_OTHER_EXIT_MATCH_SUCCESS, this.on_other_exit_success);
        event_manager.Instance.add_event_listener(event_manager.EVT_GAME_START, this.evt_game_start);
    }
    public void on_begin_match_click()
    {
        int zid = ugames.Instance.zid;
        logic_service.Instance.enter_zone(zid);
    }
    void on_user_arrived(string event_name, object udata) {
        UserArrived user_info = (UserArrived)udata;
        this.member_count++;

        GameObject user = GameObject.Instantiate(this.opt_prefab);
        user.transform.SetParent(this.scrollview.content.transform, false);
        this.scrollview.content.sizeDelta = new Vector2(0, this.member_count * 106);

        user.transform.Find("name").GetComponent<Text>().text = user_info.unick;
        user.transform.Find("header/avator").GetComponent<Image>().sprite = this.uface_img[user_info.uface - 1];
        user.transform.Find("sex").GetComponent<Text>().text = (user_info.usex == 0)? "male" : "wmale";
    }
    void evt_game_start(string name, object udata)
    {
        GameObject.Destroy(this.gameObject);
    }
    public void on_exit_match_click()
    {
        logic_service.Instance.on_exit_match_list();
    }
    
    private void on_user_exit_success(string name, object udata)
    {
        GameObject.Destroy(this.gameObject);
    }

    private void on_other_exit_success(string name, object udata)
    {
        int index = (int)udata;
        this.member_count--;
        GameObject.Destroy(this.scrollview.content.GetChild(index).gameObject);
        this.scrollview.content.sizeDelta = new Vector2(0, this.member_count * 106);
    }

    public void OnDestroy() {
        event_manager.Instance.remove_event_listener(event_manager.EVT_USER_ARRIVIED_MATCH, this.on_user_arrived);
        event_manager.Instance.remove_event_listener(event_manager.EVT_USER_EXIT_MATCH_SUCCESS, this.on_user_exit_success);
        event_manager.Instance.remove_event_listener(event_manager.EVT_OTHER_EXIT_MATCH_SUCCESS, this.on_other_exit_success);
        event_manager.Instance.remove_event_listener(event_manager.EVT_GAME_START, this.evt_game_start);
    }
}
