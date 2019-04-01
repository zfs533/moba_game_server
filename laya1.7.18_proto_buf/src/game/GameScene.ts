class GameScene extends ui.GameSceneUI
{
    constructor()
    {
        super();
        this._init();
    }
    private _init():void
    {
        this.addChild(Grid.Instance);
    }
}