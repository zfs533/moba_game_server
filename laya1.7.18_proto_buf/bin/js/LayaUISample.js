var test = ui.test.TestPageUI;
var Label = Laya.Label;
var Handler = Laya.Handler;
var Loader = Laya.Loader;
class TestUI extends ui.test.TestPageUI {
    constructor() {
        super();
        this.Pbf = Laya.Browser.window.protobuf;
        Network.Instance;
        ProtoManager.Instance;
        this.btn.on(Laya.Event.CLICK, this, this.onBtnClick);
        this.btn2.on(Laya.Event.CLICK, this, this.onBtn2Click);
        EventManager.Instance.add_event_listener("test", this.event_callback);
        EventManager.Instance.register_event_listener(Cmd.eGuestLoginRes, this.event_back);
        EventManager.Instance.register_event_listener(Cmd.eRelogin, this.event_back);
    }
    event_back(ctype, data) {
        console.log("event_type= " + ctype);
        let recv_data = ProtoManager.Instance.decode_protobuf(ctype, data);
        console.log(recv_data);
    }
    event_callback(event_type, data) {
        console.log(event_type);
        console.log(data);
    }
    onBtnClick() {
        // EventManager.Instance.dispatch_event("test","hahahha");
        let guestkey = utils.get_rand_str(32);
        Logger.debug(guestkey);
        // let str = Md5.get_md5("123456");
        let str = utils.md5("123456");
        console.log(str);
    }
    onBtn2Click() {
        // EventManager.Instance.remove_event_listener("test",this.event_callback);
        let GuestLoginReq = ProtoManager.Instance.get_protobuf_Data(Cmd.eGuestLoginReq);
        let msg = GuestLoginReq.create({
            // guestkey:utils.get_rand_str(32),
            guestkey: "41pbRR7fwA4986hvx9DcHQpgVTC25TMv",
        });
        Network.Instance.send_msg(Cmd.server_type, Cmd.eGuestLoginReq, GuestLoginReq, msg);
    }
}
//程序入口
Laya.init(600, 400);
//激活资源版本控制
Laya.ResourceVersion.enable("version.json", Handler.create(null, beginLoad), Laya.ResourceVersion.FILENAME_VERSION);
function beginLoad() {
    Laya.loader.load("res/atlas/comp.atlas", Handler.create(null, onLoaded));
}
function lalal_test() {
    let arrbuffer = new ArrayBuffer(8);
    let buffer = new DataView(arrbuffer);
    buffer.setUint16(0, 1002, true);
    buffer.setUint16(2, 2001, true);
    buffer.setUint32(4, 0, true);
    // let byte:Laya.Byte = new Laya.Byte(arrbuffer);
    let byte = new DataView(arrbuffer);
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