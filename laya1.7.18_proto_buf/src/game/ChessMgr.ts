class ChessMgr
{
    public static type_white:number = 1;
    public static type_black:number = 2;
    public static createChess(type:number = ChessMgr.type_black):Chess
    {
        let chess:Chess = new Chess(type);
        Grid.Instance.addChild(chess);
        return chess;
    }
    
    //上
    public static _check_top_line(hor:number,ver:number,type:number):boolean
    {
        let chess_all = Grid.Instance.check_arr;
        let commom_count:number = 1;
        let line_count:number = 4;
        let win_count:number = 5;
        for(let i = 0; i<line_count; i++)
        {
            if(ver < line_count)
            {
                break;
            }
            let temp = chess_all[hor][ver-i-1];
            if(temp && temp.type == type)
            {
                commom_count++;
            }
            if(win_count == commom_count)
            {
                return true;
            }
        }
        return false;
    }
    //右斜上方
    public static _check_skew_top_right_line(hor:number,ver:number,type:number):boolean
    {
        let chess_all = Grid.Instance.check_arr;
        let commom_count:number = 1;
        let line_count:number = 4;
        let win_count:number = 5;
        for(let i = 0; i<line_count; i++)
        {
            if(ver < line_count || hor + line_count>=Grid.Instance.hor_count)
            {
                break;
            }
            let temp = chess_all[hor+i+1][ver+i+1];
            if(temp && temp.type == type)
            {
                commom_count++;
            }
            if(win_count == commom_count)
            {
                return true;
            }
        }
        return false;
    }
    //左斜上方
    public static _check_skew_top_left_line(hor:number,ver:number,type:number):boolean
    {
        let chess_all = Grid.Instance.check_arr;
        let commom_count:number = 1;
        let line_count:number = 4;
        let win_count:number = 5;
        for(let i = 0; i<line_count; i++)
        {
            if(ver < line_count || hor < line_count)
            {
                break;
            }
            let temp = chess_all[hor-i-1][ver+i+1];
            if(temp && temp.type == type)
            {
                commom_count++;
            }
            if(win_count == commom_count)
            {
                return true;
            }
        }
        return false;
    }
    //下
    public static _check_down_line(hor:number,ver:number,type:number):boolean
    {
        let chess_all = Grid.Instance.check_arr;
        let commom_count:number = 1;
        let line_count:number = 4;
        let win_count:number = 5;
        for(let i = 0; i<line_count; i++)
        {
            if(ver + line_count>=Grid.Instance.ver_count)
            {
                break;
            }
            let temp = chess_all[hor][ver+i+1];
            if(temp && temp.type == type)
            {
                commom_count++;
            }
            if(win_count == commom_count)
            {
                return true;
            }
        }
        return false;
    }
    //右斜下方
    public static _check_skey_down_right_line(hor:number,ver:number,type:number):boolean
    {
        let chess_all = Grid.Instance.check_arr;
        let commom_count:number = 1;
        let line_count:number = 4;
        let win_count:number = 5;
        for(let i = 0; i<line_count; i++)
        {
            if(ver + line_count>=Grid.Instance.ver_count || hor+line_count >= Grid.Instance.hor_count)
            {
                break;
            }
            let temp = chess_all[hor+i+1][ver-i-1];
            if(temp && temp.type == type)
            {
                commom_count++;
            }
            if(win_count == commom_count)
            {
                return true;
            }
        }
        return false;
    }
    //左斜下方
    public static _check_skey_down_left_line(hor:number,ver:number,type:number):boolean
    {
        let chess_all = Grid.Instance.check_arr;
        let commom_count:number = 1;
        let line_count:number = 4;
        let win_count:number = 5;
        for(let i = 0; i<line_count; i++)
        {
            if(ver + line_count>=Grid.Instance.ver_count || hor<line_count)
            {
                break;
            }
            let temp = chess_all[hor-i-1][ver-i-1];
            if(temp && temp.type == type)
            {
                commom_count++;
            }
            if(win_count == commom_count)
            {
                return true;
            }
        }
        return false;
    }
    //右
    public static _check_right_line(hor:number,ver:number,type:number):boolean
    {
        let chess_all = Grid.Instance.check_arr;
        let commom_count:number = 1;
        let line_count:number = 4;
        let win_count:number = 5;
        for(let i = 0; i<line_count; i++)
        {
            if(hor + line_count>=Grid.Instance.hor_count)
            {
                break;
            }
            let temp = chess_all[hor+i+1][ver];
            if(temp && temp.type == type)
            {
                commom_count++;
            }
            if(win_count == commom_count)
            {
                return true;
            }
        }
        return false;
    }
    //左
    public static _check_left_line(hor:number,ver:number,type:number):boolean
    {
        let chess_all = Grid.Instance.check_arr;
        let commom_count:number = 1;
        let line_count:number = 4;
        let win_count:number = 5;
        for(let i = 0; i<line_count; i++)
        {
            if(hor < line_count)
            {
                break;
            }
            let temp = chess_all[hor-i-1][ver];
            if(temp && temp.type == type)
            {
                commom_count++;
            }
            if(win_count == commom_count)
            {
                return true;
            }
        }
        return false;
    }
    public static check_game_over(hor:number,ver:number,type:number):boolean
    {
        if(this._check_top_line(hor,ver,type))
        {
            return true;
        }
        if(this._check_down_line(hor,ver,type))
        {
            return true;
        }
        if(this._check_right_line(hor,ver,type))
        {
            return true;
        }
        if(this._check_left_line(hor,ver,type))
        {
            return true;
        }
        if(this._check_skew_top_right_line(hor,ver,type))
        {
            return true;
        }
        if(this._check_skew_top_left_line(hor,ver,type))
        {
            return true;
        }
        if(this._check_skey_down_right_line(hor,ver,type))
        {
            return true;
        }
        if(this._check_skey_down_left_line(hor,ver,type))
        {
            return true;
        }
        return false
    }
    public static remove_grid_chess(hor:number,ver:number):void
    {
        let chess_all = Grid.Instance.check_arr;
        if(chess_all[hor][ver])
        {
            chess_all[hor][ver].destroy();
            chess_all[hor][ver] = null;
        }
    }
}