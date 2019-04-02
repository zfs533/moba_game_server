using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ulevel : Singletom<ulevel>
{
    private int[] level_exp = new int[] 
    {0,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,20000,30000,40000,50000};
    public void init()
    { 

    }
    public int get_level_exp(int cur_exp,out int now_exp,out int next_level_exp)
    {
        now_exp = 0;
        next_level_exp = 0;
        int level = 0;
        while (level + 1 <= this.level_exp.Length - 1 && cur_exp >= this.level_exp[level+1])
        {
            cur_exp -= this.level_exp[level + 1];
            level++;
        }
        now_exp = cur_exp;
        if (level == this.level_exp.Length - 1)
        {
            next_level_exp = now_exp;
        }
        else
        {
            next_level_exp = this.level_exp[level + 1];
        }

        return level;
    }
}
