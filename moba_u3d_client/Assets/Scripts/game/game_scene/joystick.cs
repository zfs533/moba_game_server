using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class joystick : MonoBehaviour {
    public Canvas cs;
    public Transform stick;
    public float max_R = 80;

    private Vector2 touch_dir = Vector2.zero;
    public Vector2 dir {
        get {
            return this.touch_dir;
        }
    }

	// Use this for initialization
	void Start () {
        this.stick.localPosition = Vector2.zero;
        this.touch_dir = Vector2.zero;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void on_stick_drag() {
        Vector2 pos = Vector2.zero;
        RectTransformUtility.ScreenPointToLocalPointInRectangle(this.transform as RectTransform, Input.mousePosition, this.cs.worldCamera, out pos);

        float len = pos.magnitude;
        if (len <= 0) {
            this.touch_dir = Vector2.zero;
            return;
        }

        // 归一化, 
        this.touch_dir.x = pos.x / len; // cos(r)
        this.touch_dir.y = pos.y / len; // (sinr) cos^2 + sin ^ 2 = 1;

        if (len >= this.max_R) { // this.max_R / len = x` / x = y` / y;
            pos.x = pos.x * this.max_R / len;
            pos.y = pos.y * this.max_R / len;
        }

        this.stick.localPosition = pos;
    }

    public void on_stick_end_drag() {
        this.stick.localPosition = Vector2.zero;
        this.touch_dir = Vector2.zero;
    }

}
