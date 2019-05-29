using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

public enum OptType
{ 
    JoyStick = 1,
    Attack1 =2,
    Attack2 = 3,
    Attack3 = 4,
    Skill1 = 5,
    Skill2 = 6,
    Skill3 = 7,
}

public class game_zygote : MonoBehaviour
{
    public joystick stick;

    public GameObject[] hero_characters = null; // 男, 女;

    public GameObject entry_A; // 
                               // Use this for initialization
    private int sync_frameid = 0;
    private FrameOpts last_frame_opt = null;
    private hero test_hero;//测试
    void Start()
    {
        event_manager.Instance.add_event_listener(event_manager.EVT_LOGIC_FRAME_UPDATE, this.evt_logic_frame_update);
        GameObject hero = GameObject.Instantiate(this.hero_characters[ugames.Instance.usex]);
        hero.transform.SetParent(this.transform, false);
        hero.transform.position = this.entry_A.transform.position;
        hero ctrl = hero.AddComponent<hero>();
        ctrl.is_ghost = false; // 自己来控制;
        ctrl.logic_init(hero.transform.position);//逻辑数据部分的初始化
        this.test_hero = ctrl;
    }

    // Update is called once per frame
    void Update()
    {

    }
    //采集玩家的下一个操作帧
    void capture_player_opts()
    {
        NextFrameOpts next_frame = new NextFrameOpts();
        next_frame.frameid = this.sync_frameid + 1;
        next_frame.zid = ugames.Instance.zid;
        next_frame.matchid = ugames.Instance.matchid;
        next_frame.seatid = ugames.Instance.self_seatid;
        //遥杆
        OptionEvent opt_stick = new OptionEvent();
        opt_stick.seatid = ugames.Instance.self_seatid;
        opt_stick.opttype = (int)OptType.JoyStick;
        //Debug.Log(this.stick.dir.y + " ---dirx--- " + this.stick.dir.x);
        opt_stick.x = (int)(this.stick.dir.x * (1 << 16));
        opt_stick.y = (int)(this.stick.dir.y * (1 << 16));
        //Debug.Log(opt_stick.y + " ---ed---- " + opt_stick.x);
        //opt_stick.x = (int)this.stick.dir.x;
        //opt_stick.y = (int)this.stick.dir.y;
        next_frame.opts.Add(opt_stick);
        //end

        //攻击
        //end
        
        //发送给服务器
        logic_service.Instance.send_next_frame_opts(next_frame);
        //end
    }

    void on_handler_frame_event(FrameOpts frame)
    { 
        //把所有英雄的输入进行处理
        for (int i = 0; i < frame.opts.Count; i++)
        { 
            //这里暂时只管玩家自己的英雄
            if (frame.opts[i].seatid == ugames.Instance.self_seatid)
            {
                this.test_hero.on_handler_frame_event(frame.opts[i]);
            }
        }
        //end

        //怪物AI 根据我们的处理，来进行处理...
        
        //end
    }

    void evt_logic_frame_update(string name, object udata)
    {
        LogicFrame frame = (LogicFrame)udata;
        if (frame.frameid < this.sync_frameid)
        {
            return;
        }
        /*
        Debug.Log(frame.unsyncFrames.Count);
        for (int i = 0; i < frame.unsyncFrames.Count; i++)
        {
            for (int j = 0; j < frame.unsyncFrames[i].opts.Count; j++)
            {
                Debug.Log(frame.unsyncFrames[i].opts[i].x + "------------------:--------------------" + frame.unsyncFrames[i].opts[i].y);
            }
        }*/
        //同步自己客户端上一帧逻辑操作，调整我们的位置；调整完成后，客户端同步的是sync_frameid
        //end

        //从sync_frameid+1开始----------------》frame.frameid-1;
        //同步丢失的帧，所有的客户端数据同步到 frame.frameid-1
        //end

        //获取最后一个操作，frame.frameid操作，根据这个操作来处理播放动画
        this.sync_frameid = frame.frameid;//同步到的事件帧id
        if (frame.unsyncFrames.Count > 0)
        {
            this.last_frame_opt = frame.unsyncFrames[frame.unsyncFrames.Count - 1];
            this.on_handler_frame_event(this.last_frame_opt);
        }
        else
        {
            this.last_frame_opt = null;
        }
        //end
        
        //采集下一个帧的事件，发送给服务器
        //采集玩家当前的操作帧
        this.capture_player_opts();
    }
}
