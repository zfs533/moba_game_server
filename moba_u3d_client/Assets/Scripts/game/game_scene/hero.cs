using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gprotocol;

enum charactor_state
{
    walk = 1,
    free = 2,
    idle = 3,
    attack = 4,
    attack2 = 5,
    attack3 = 6,
    skill = 7,
    skill2 = 8,
    death = 9,
}

public class hero : MonoBehaviour
{
    public bool is_ghost = false; // is_ghost: 标记是否为别人控制的 ghost;
    public float speed = 8.0f; // 给我们的角色定义一个速度

    private CharacterController ctrl;
    private Animation anim;
    private charactor_state anim_state = charactor_state.idle;
    private Vector3 camera_offset; // 主角离摄像机的相对距离;
                                   // Use this for initialization
    private int stick_x = 0;
    private int stick_y = 0;
    private charactor_state logic_state = charactor_state.idle;
    private Vector3 logic_position;//保存当前额逻辑帧的位置

    void Start()
    {
        GameObject ring = Resources.Load<GameObject>("effect/other/guangquan_fanwei");
        this.ctrl = this.GetComponent<CharacterController>();
        this.anim = this.GetComponent<Animation>();

        if (!this.is_ghost)
        {  // 玩家控制角色;
            GameObject r = GameObject.Instantiate(ring);
            r.transform.SetParent(this.transform, false);
            r.transform.localPosition = Vector3.zero;
            r.transform.localScale = new Vector3(2, 1, 2);
            this.camera_offset = Camera.main.transform.position - this.transform.position;
        }

        this.anim.Play("idle");
    }
    public void logic_init(Vector3 logic_pos)
    {
        this.stick_x = 0;
        this.stick_y = 0;
        this.logic_position = logic_pos;
        this.logic_state = charactor_state.idle;
    }
    //同步英雄坐标
    private void do_joystick_event(float dt)
    {
        if (this.stick_x == 0 && this.stick_y == 0)
        {
            this.logic_state = charactor_state.idle;
            return;
        }
        this.logic_state = charactor_state.walk;
        Debug.Log(this.stick_x + " ---ed---- " + this.stick_y);
        float dir_x = ((float)this.stick_x/(float)(1<<16));
        float dir_y = ((float)this.stick_y/(float)(1<<16));
        Debug.Log(dir_x + " diry= " + dir_y);
        //float dir_x = (float)this.stick_x;
        //float dir_y = (float)this.stick_y;
        float r = Mathf.Atan2(dir_y,dir_x);

        float s = this.speed * dt;
        float sx = s * Mathf.Cos(r - Mathf.PI * 0.25f);
        float sz = s * Mathf.Sin(r - Mathf.PI * 0.25f);
        this.ctrl.Move(new Vector3(sx, 0, sz));

        float degree = r * 180 / Mathf.PI;
        degree = 360 - degree + 90 + 45;
        this.transform.localEulerAngles = new Vector3(0, degree, 0);
    }

    //播放动画
    private void on_joystic_anima_update()
    {
        if (this.anim_state != charactor_state.idle && this.anim_state != charactor_state.walk)
        {
            //这里要播放动画等其他操作
            return;
        }
        if (this.stick_x == 0 && this.stick_y == 0)
        {
            if (this.anim_state == charactor_state.walk)
            {
                this.anim.CrossFade("idle");
                this.anim_state = charactor_state.idle;
            }
            return;
        }
        if (this.anim_state == charactor_state.idle)
        {
            this.anim.CrossFade("walk");
            this.anim_state = charactor_state.walk;
        }

        this.do_joystick_event(Time.deltaTime);//同步英雄坐标

        if (!this.is_ghost)
        {
            Camera.main.transform.position = this.transform.position + this.camera_offset;
        }
    }
    // Update is called once per frame
    void Update()
    {
        //播放动画
        this.on_joystic_anima_update();
        /*
        if (this.anim_state != charactor_state.idle && this.anim_state != charactor_state.walk)
        {
            return;
        }

        if (this.stick_x == 0 && this.stick_y == 0)
        {
            if (this.anim_state == charactor_state.walk)
            {
                this.anim.CrossFade("idle");
                this.anim_state = charactor_state.idle;
            }
            return;
        }

        if (this.anim_state == charactor_state.idle)
        {
            this.anim.CrossFade("walk");
            this.anim_state = charactor_state.walk;
        }
        float r = Mathf.Atan2(this.stick_y, stick_x);
        float s = this.speed * Time.deltaTime;
        float sx = s * Mathf.Cos(r - Mathf.PI * 0.25f);
        float sz = s * Mathf.Sin(r - Mathf.PI * 0.25f);
        this.ctrl.Move(new Vector3(sx, 0, sz));

        float degree = r * 180 / Mathf.PI;
        degree = 360 - degree + 90 + 45;
        this.transform.localEulerAngles = new Vector3(0, degree, 0);

        if (!this.is_ghost)
        {
            Camera.main.transform.position = this.transform.position + this.camera_offset;
        }
        */
    }
    //处理遥杆事件
    private void handle_joystic_event(OptionEvent opt)
    {
        this.stick_x = opt.x;
        this.stick_y = opt.y;
        if (this.stick_x == 0 && this.stick_y == 0)
        {
            this.logic_state = charactor_state.idle;
        }
        else
        {
            this.logic_state = charactor_state.walk;
        }
    }
    //英雄来处理我们的帧事件
    public void on_handler_frame_event(OptionEvent opt)
    {
        switch (opt.opttype)
        { 
            case (int)OptType.JoyStick:
                this.handle_joystic_event(opt);
            break;
        }
    }
}
