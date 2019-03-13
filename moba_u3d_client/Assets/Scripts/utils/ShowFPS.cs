using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShowFPS : MonoBehaviour
{
    //固定一个时间间隔
    private float time_delta = 0.5f;
    //Time.realtimeSinceStartup:指的是我们当前从启动开始到现在的运行时间，单位(s)
    private float prev_time = 0.0f;//上次统计FPS的时间
    private float fps = 0.0f;//计算出来的FPS的值
    private int i_frames = 0;//累计我们刷新的帧数

    private GUIStyle style;

    void Awake()
    {
        //假设CPU 100% 工作的工作状态下 FPS 300
        //当你设置了这个以后，他就维持在60FPS左右，不会继续冲高
        //设为-1，游戏引擎就会不断的刷新我们的画面，有多高刷多高；一般保持在60FPS左右就够了
        Application.targetFrameRate = 60;
    }

    // Start is called before the first frame update
    void Start()
    {
        this.prev_time = Time.realtimeSinceStartup;
        this.style = new GUIStyle();
        this.style.fontSize = 18;
        this.style.normal.textColor = new Color(255, 255, 255);
    }

    void OnGUI()
    {
        GUI.Label(new Rect(0,Screen.height-20,200,200),"FPS:"+this.fps.ToString("f2"),this.style);
    }

    // Update is called once per frame
    void Update()
    {
        this.i_frames++;
        if (Time.realtimeSinceStartup >= this.prev_time + this.time_delta)
        {
            this.fps = (float)this.i_frames / (Time.realtimeSinceStartup - this.prev_time);
            this.prev_time = Time.realtimeSinceStartup;
            this.i_frames = 0;//重新累计FPS
        }
    }
}
