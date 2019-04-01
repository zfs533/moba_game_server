class Chess extends Laya.Image
{
    public hor:number = 0;
    public ver:number = 0;
    public type:number = 0;
    constructor(type:number = ChessMgr.type_black)
    {
        super();
        this.size(30,30);
        this._draw_chess(type);
    }
    private _draw_chess(type:number):void
    {
        this.type = type;
        if(type == ChessMgr.type_black)
        {
            this.graphics.drawCircle(0,0,20,"#ffff00");
        }
        else
        {
            this.graphics.drawCircle(0,0,20,"#ff00ff");
        }
    }
    public set_hor_ver(hor:number,ver:number):void
    {
        this.hor = hor;
        this.ver = ver;
    }
}