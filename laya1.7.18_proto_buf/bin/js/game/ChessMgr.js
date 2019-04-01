class ChessMgr {
    static createChess(type = ChessMgr.type_black) {
        let chess = new Chess(type);
        Grid.Instance.addChild(chess);
        return chess;
    }
    //上
    static _check_top_line(hor, ver, type) {
        let chess_all = Grid.Instance.check_arr;
        let commom_count = 1;
        let line_count = 4;
        let win_count = 5;
        for (let i = 0; i < line_count; i++) {
            if (ver < line_count) {
                break;
            }
            let temp = chess_all[hor][ver - i - 1];
            if (temp && temp.type == type) {
                commom_count++;
            }
            if (win_count == commom_count) {
                return true;
            }
        }
        return false;
    }
    //右斜上方
    static _check_skew_top_right_line(hor, ver, type) {
        let chess_all = Grid.Instance.check_arr;
        let commom_count = 1;
        let line_count = 4;
        let win_count = 5;
        for (let i = 0; i < line_count; i++) {
            if (ver < line_count || hor + line_count >= Grid.Instance.hor_count) {
                break;
            }
            let temp = chess_all[hor + i + 1][ver + i + 1];
            if (temp && temp.type == type) {
                commom_count++;
            }
            if (win_count == commom_count) {
                return true;
            }
        }
        return false;
    }
    //左斜上方
    static _check_skew_top_left_line(hor, ver, type) {
        let chess_all = Grid.Instance.check_arr;
        let commom_count = 1;
        let line_count = 4;
        let win_count = 5;
        for (let i = 0; i < line_count; i++) {
            if (ver < line_count || hor < line_count) {
                break;
            }
            let temp = chess_all[hor - i - 1][ver + i + 1];
            if (temp && temp.type == type) {
                commom_count++;
            }
            if (win_count == commom_count) {
                return true;
            }
        }
        return false;
    }
    //下
    static _check_down_line(hor, ver, type) {
        let chess_all = Grid.Instance.check_arr;
        let commom_count = 1;
        let line_count = 4;
        let win_count = 5;
        for (let i = 0; i < line_count; i++) {
            if (ver + line_count >= Grid.Instance.ver_count) {
                break;
            }
            let temp = chess_all[hor][ver + i + 1];
            if (temp && temp.type == type) {
                commom_count++;
            }
            if (win_count == commom_count) {
                return true;
            }
        }
        return false;
    }
    //右斜下方
    static _check_skey_down_right_line(hor, ver, type) {
        let chess_all = Grid.Instance.check_arr;
        let commom_count = 1;
        let line_count = 4;
        let win_count = 5;
        for (let i = 0; i < line_count; i++) {
            if (ver + line_count >= Grid.Instance.ver_count || hor + line_count >= Grid.Instance.hor_count) {
                break;
            }
            let temp = chess_all[hor + i + 1][ver - i - 1];
            if (temp && temp.type == type) {
                commom_count++;
            }
            if (win_count == commom_count) {
                return true;
            }
        }
        return false;
    }
    //左斜下方
    static _check_skey_down_left_line(hor, ver, type) {
        let chess_all = Grid.Instance.check_arr;
        let commom_count = 1;
        let line_count = 4;
        let win_count = 5;
        for (let i = 0; i < line_count; i++) {
            if (ver + line_count >= Grid.Instance.ver_count || hor < line_count) {
                break;
            }
            let temp = chess_all[hor - i - 1][ver - i - 1];
            if (temp && temp.type == type) {
                commom_count++;
            }
            if (win_count == commom_count) {
                return true;
            }
        }
        return false;
    }
    //右
    static _check_right_line(hor, ver, type) {
        let chess_all = Grid.Instance.check_arr;
        let commom_count = 1;
        let line_count = 4;
        let win_count = 5;
        for (let i = 0; i < line_count; i++) {
            if (hor + line_count >= Grid.Instance.hor_count) {
                break;
            }
            let temp = chess_all[hor + i + 1][ver];
            if (temp && temp.type == type) {
                commom_count++;
            }
            if (win_count == commom_count) {
                return true;
            }
        }
        return false;
    }
    //左
    static _check_left_line(hor, ver, type) {
        let chess_all = Grid.Instance.check_arr;
        let commom_count = 1;
        let line_count = 4;
        let win_count = 5;
        for (let i = 0; i < line_count; i++) {
            if (hor < line_count) {
                break;
            }
            let temp = chess_all[hor - i - 1][ver];
            if (temp && temp.type == type) {
                commom_count++;
            }
            if (win_count == commom_count) {
                return true;
            }
        }
        return false;
    }
    static check_game_over(hor, ver, type) {
        if (this._check_top_line(hor, ver, type)) {
            return true;
        }
        if (this._check_down_line(hor, ver, type)) {
            return true;
        }
        if (this._check_right_line(hor, ver, type)) {
            return true;
        }
        if (this._check_left_line(hor, ver, type)) {
            return true;
        }
        if (this._check_skew_top_right_line(hor, ver, type)) {
            return true;
        }
        if (this._check_skew_top_left_line(hor, ver, type)) {
            return true;
        }
        if (this._check_skey_down_right_line(hor, ver, type)) {
            return true;
        }
        if (this._check_skey_down_left_line(hor, ver, type)) {
            return true;
        }
        return false;
    }
}
ChessMgr.type_white = 1;
ChessMgr.type_black = 2;
//# sourceMappingURL=ChessMgr.js.map