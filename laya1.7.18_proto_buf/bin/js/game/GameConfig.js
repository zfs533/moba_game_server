class GameConfig {
    static init() {
        Laya.init(this.scene_width, this.scene_height, Laya.WebGL);
        Laya.stage.scaleMode = Laya.Stage.SCALE_SHOWALL;
        Laya.stage.alignH = Laya.Stage.ALIGN_CENTER;
        Laya.stage.alignV = Laya.Stage.ALIGN_CENTER;
    }
}
GameConfig.grid_size = 50;
GameConfig.scene_width = 640;
GameConfig.scene_height = 1136;
GameConfig.grid_width = 640;
GameConfig.grid_height = 1136;
//# sourceMappingURL=GameConfig.js.map