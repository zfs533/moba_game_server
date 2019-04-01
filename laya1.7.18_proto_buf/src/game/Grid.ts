class Grid extends Laya.Image
{
    private _is_mouse_down:boolean = false;
    private _chess:Chess = null;
    private _touch_panel:Laya.Panel;
    public check_arr:Array<any> = new Array<any>();
    //--------------------------
    private _gap:number = GameConfig.grid_size;
    private static _inst:Grid;
    public hor_count:number;
    public ver_count:number;
    public static get Instance():Grid
    {
        return this._inst || (this._inst = new Grid());
    }
    constructor()
    {
        super();
        Grid._inst = this;
        this.size(GameConfig.grid_width,GameConfig.grid_height);
        this._draw_chess_grid();
        this._add_touch_layer();
    }
    private _draw_chess_grid():void
    {
        let hor:number = Math.ceil(Math.floor(GameConfig.grid_width/this._gap));
        let ver:number = Math.ceil(Math.floor(GameConfig.grid_height/this._gap));
        this.hor_count = hor;
        this.ver_count = ver;
        for(let i = 0; i<hor; i++)
        {
            this.graphics.drawLine(i*this._gap,0,i*this._gap,GameConfig.grid_height,"#ffffff",1);
        }
        for(let j = 0; j<ver; j++)
        {
            this.graphics.drawLine(0,j*this._gap,GameConfig.grid_width,j*this._gap,"#ffffff",1);
        }
        for(let m = 0; m <hor; m++)
        {
            this.check_arr.push([]);
            for(let n = 0; n < ver;n++)
            {
                this.check_arr[m][n] = null;
            }
        }
    }
    private _add_touch_layer():void
    {
        let touch_panel:Laya.Panel = new Laya.Panel();
        touch_panel.size(this.width,this.height);
        this.addChild(touch_panel);
        touch_panel.zOrder = 100;
        this._touch_panel = touch_panel;
        this._addTouchEventlistener();
    }
    private _addTouchEventlistener():void
    {
        this._touch_panel.on(Laya.Event.MOUSE_DOWN,this,this._on_mouse_down);
        this._touch_panel.on(Laya.Event.MOUSE_MOVE,this,this._on_mouse_move);
        this._touch_panel.on(Laya.Event.MOUSE_UP,this,this._on_mouse_up);
    }
    private _on_mouse_down(event:Laya.Event):void
    {
        let target = event.currentTarget;
        this._is_mouse_down = true;
        this._chess = ChessMgr.createChess();
        this._chess.pos(target.mouseX,target.mouseY);
    }
    private _on_mouse_move(event:any):void
    {
        let target = event.currentTarget;
        if(this._is_mouse_down)
        {
            this._chess.pos(target.mouseX,target.mouseY);
        }
    }
    private _on_mouse_up(event:any):void
    {
        let target = event.currentTarget;
        let point:Laya.Point = new Laya.Point(target.mouseX,target.mouseY)
        this._is_mouse_down = false;
        let hor:number = 0;
        let ver:number = 0;
        let forwardx:number = Math.floor(point.x/this._gap);
        let behindx:number  = Math.ceil(point.x/this._gap);
        hor = behindx;
        if(point.x < forwardx*this._gap + this._gap/2)
        {
            hor = forwardx;
        }
        let forwardy:number = Math.floor(point.y/this._gap);
        let behindy:number  = Math.ceil(point.y/this._gap);
        ver = behindy;
        if(point.y < forwardy*this._gap + this._gap/2)
        {
            ver = forwardy;
        }
        if(hor>=this.hor_count || ver >=this.ver_count)
        {
            this._chess.destroy();
            this._chess = null;
            Logger.debug("超出范围-----------------");
            return;
        }
        this._chess.pos(hor*this._gap,ver*this._gap);
        this._chess.set_hor_ver(hor,ver);
        this.check_arr[hor][ver] = this._chess;
        this._check_game_over(this._chess);
    }
    private _check_game_over(chess:Chess):void
    {
        let is_game_over = ChessMgr.check_game_over(chess.hor,chess.ver,chess.type);
        if(is_game_over)
        {
            Logger.debug("game_is_over!!!");
        }
        this._chess = null;
    }
}