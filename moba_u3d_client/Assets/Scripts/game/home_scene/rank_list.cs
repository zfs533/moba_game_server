using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using gprotocol;

public class rank_list : MonoBehaviour {
    public GameObject rank_opt_prefab;
    public GameObject content_root;
    public ScrollRect rank;

    public Sprite[] uface_img = null;

    void on_get_rank_list_data(string event_name, object udata) {
        List<WorldChipRankInfo> rank_info = (List<WorldChipRankInfo>)udata;

        this.rank.content.sizeDelta = new Vector2(0, rank_info.Count * 170);
        // 获取得到排行榜的数据
        for (int i = 0; i < rank_info.Count; i++)
        {
            // Debug.Log(rank_info[i].unick + " " + rank_info[i].uchip);
            GameObject opt = GameObject.Instantiate(this.rank_opt_prefab);
            opt.transform.SetParent(content_root.transform, false);

            opt.transform.Find("order").GetComponent<Text>().text = "" + (i + 1);
            opt.transform.Find("unick_label").GetComponent<Text>().text = rank_info[i].unick;
            opt.transform.Find("uchip_label").GetComponent<Text>().text = "" + rank_info[i].uchip;
            int uface = rank_info[i].uface;
            opt.transform.Find("header/avator").GetComponent<Image>().sprite = this.uface_img[uface - 1];
        }
        // end
    }

	// Use this for initialization
	void Start () {
        event_manager.Instance.add_event_listener(event_manager.EVT_GET_RANK_LIST, this.on_get_rank_list_data);
        // 发送拉取排行榜的请求
        system_server.Instance.game_rank_test();
        // end 
	}

    void OnDestroy() {
        event_manager.Instance.remove_event_listener(event_manager.EVT_GET_RANK_LIST, this.on_get_rank_list_data);
    }

	// Update is called once per frame
	void Update () {
		
	}

    public void on_rank_list_close() {
        GameObject.Destroy(this.gameObject);
    }
}
