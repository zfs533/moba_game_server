using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
//异步加载的方式我们使用协程来做
//异步加载我们的游戏场景
//同步加载SceneManager.LoadScene,加载的时候，当前游戏的进程是卡住的，一直在加载
//异步加载游戏进程不会卡死一直在加载
public class async_loader_scene : MonoBehaviour
{
    private string scene_name = "game_scene";
    public Image proccess;
    private AsyncOperation ao;
    // Start is called before the first frame update
    void Start()
    {
        this.proccess.fillAmount = 0;
        this.StartCoroutine(this.async_load_scene());
    }
    IEnumerator async_load_scene()
    {
        this.ao = SceneManager.LoadSceneAsync(this.scene_name);
        this.ao.allowSceneActivation = false;
        yield return this.ao;
    }
    // Update is called once per frame
    void Update()
    {
        float percent = this.ao.progress;//当前加载进度的百分比，最大值为0.9f
        if (percent >= 0.9f)
        {
            this.ao.allowSceneActivation = true;//加载完了
        }
        this.proccess.fillAmount = percent / 0.9f;//[0-1]
    }
}
