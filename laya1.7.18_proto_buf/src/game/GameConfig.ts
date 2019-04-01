class GameConfig
{
    public static grid_size:number = 50;
    public static scene_width:number = 640;
    public static scene_height:number = 1136;
    public static grid_width:number = 640;
    public static grid_height:number = 1136;
    public static init():void
    {
        Laya.init(this.scene_width,this.scene_height,Laya.WebGL);
        Laya.stage.scaleMode = Laya.Stage.SCALE_SHOWALL;
        Laya.stage.alignH = Laya.Stage.ALIGN_CENTER;
        Laya.stage.alignV = Laya.Stage.ALIGN_CENTER;
    }
}