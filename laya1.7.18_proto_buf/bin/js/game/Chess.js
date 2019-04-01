class Chess extends Laya.Image {
    constructor(type = ChessMgr.type_black) {
        super();
        this.hor = 0;
        this.ver = 0;
        this.type = 0;
        this.size(30, 30);
        this._draw_chess(type);
    }
    _draw_chess(type) {
        this.type = type;
        if (type == ChessMgr.type_black) {
            this.graphics.drawCircle(0, 0, 20, "#ffff00");
        }
        else {
            this.graphics.drawCircle(0, 0, 20, "#ff00ff");
        }
    }
    set_hor_ver(hor, ver) {
        this.hor = hor;
        this.ver = ver;
    }
}
//# sourceMappingURL=Chess.js.map