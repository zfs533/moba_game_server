var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
var test = ui.test.TestPageUI;
var Label = Laya.Label;
var Handler = Laya.Handler;
var Loader = Laya.Loader;
var TestUI = /** @class */ (function (_super) {
    __extends(TestUI, _super);
    function TestUI() {
        var _this = _super.call(this) || this;
        _this.Pbf = Laya.Browser.window.protobuf;
        Network.Instance;
        ProtoManager.Instance;
        _this.btn.on(Laya.Event.CLICK, _this, _this.onBtnClick);
        _this.btn2.on(Laya.Event.CLICK, _this, _this.onBtn2Click);
        EventManager.Instance.add_event_listener("test", _this.event_callback);
        EventManager.Instance.register_event_listener(Cmd.eGuestLoginRes, _this.event_back);
        return _this;
    }
    TestUI.prototype.event_back = function (ctype, data) {
        console.log("event_type= " + ctype);
        var recv_data = ProtoManager.Instance.decode_protobuf(ctype, data);
        console.log(recv_data);
    };
    TestUI.prototype.event_callback = function (event_type, data) {
        console.log(event_type);
        console.log(data);
    };
    TestUI.prototype.onBtnClick = function () {
        // EventManager.Instance.dispatch_event("test","hahahha");
        var guestkey = utils.get_rand_str(32);
        Logger.debug(guestkey);
    };
    TestUI.prototype.onBtn2Click = function () {
        // EventManager.Instance.remove_event_listener("test",this.event_callback);
        var GuestLoginReq = ProtoManager.Instance.get_protobuf_Data(Cmd.eGuestLoginReq);
        var msg = GuestLoginReq.create({
            guestkey: "jdslfkjaslkdfjasdklfjaslkdfj",
        });
        Network.Instance.send_msg(Cmd.server_type, Cmd.eGuestLoginReq, GuestLoginReq, msg);
    };
    return TestUI;
}(ui.test.TestPageUI));
//程序入口
Laya.init(600, 400);
//激活资源版本控制
Laya.ResourceVersion.enable("version.json", Handler.create(null, beginLoad), Laya.ResourceVersion.FILENAME_VERSION);
function beginLoad() {
    Laya.loader.load("res/atlas/comp.atlas", Handler.create(null, onLoaded));
}
function lalal_test() {
    var arrbuffer = new ArrayBuffer(8);
    var buffer = new DataView(arrbuffer);
    buffer.setUint16(0, 1002, true);
    buffer.setUint16(2, 2001, true);
    buffer.setUint32(4, 0, true);
    // let byte:Laya.Byte = new Laya.Byte(arrbuffer);
    var byte = new DataView(arrbuffer);
    console.log(byte.getUint16(0, true));
    console.log(byte.getUint16(2, true));
    console.log(byte.getUint32(4, true));
}
function onLoaded() {
    //实例UI界面
    var testUI = new TestUI();
    Laya.stage.addChild(testUI);
    // lalal_test();
}
//# sourceMappingURL=LayaUISample.js.map