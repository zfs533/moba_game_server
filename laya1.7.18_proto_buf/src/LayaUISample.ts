import test = ui.test.TestPageUI;
import Label = Laya.Label;
import Handler = Laya.Handler;
import Loader = Laya.Loader;

class TestUI extends ui.test.TestPageUI 
{
	private Pbf = Laya.Browser.window.protobuf;
	constructor() 
	{
		super();

		Network.Instance;
		ProtoManager.Instance;


		this.btn.on(Laya.Event.CLICK, this, this.onBtnClick);
		this.btn2.on(Laya.Event.CLICK, this, this.onBtn2Click);
		
		EventManager.Instance.add_event_listener("test",this.event_callback);
		EventManager.Instance.register_event_listener(Cmd.eGuestLoginRes,this.event_back);
	}
	private event_back(ctype:number,data:Uint8Array):void
	{
		console.log("event_type= "+ctype);
		let recv_data = ProtoManager.Instance.decode_protobuf(ctype,data);
		console.log(recv_data);
	}
	private event_callback(event_type,data):void
	{
		console.log(event_type);
		console.log(data);
	}
	private onBtnClick(): void {
		// EventManager.Instance.dispatch_event("test","hahahha");
		let guestkey = utils.get_rand_str(32);
		Logger.debug(guestkey);
	}

	private onBtn2Click(): void {
		// EventManager.Instance.remove_event_listener("test",this.event_callback);
		let GuestLoginReq = ProtoManager.Instance.get_protobuf_Data(Cmd.eGuestLoginReq);
		let msg = GuestLoginReq.create(
		{
			// guestkey:utils.get_rand_str(32);
			guestkey:"FIUMQSgbsZyHWDYs6hFfkLT1oCABGd8y",
		});
		Network.Instance.send_msg(Cmd.server_type,Cmd.eGuestLoginReq,GuestLoginReq,msg);
	}

}

//程序入口
Laya.init(600, 400);
//激活资源版本控制
Laya.ResourceVersion.enable("version.json", Handler.create(null, beginLoad), Laya.ResourceVersion.FILENAME_VERSION);

function beginLoad(){
	Laya.loader.load("res/atlas/comp.atlas", Handler.create(null, onLoaded));
}


function lalal_test():void
{
	let arrbuffer:ArrayBuffer = new ArrayBuffer(8);
	let buffer:DataView = new DataView(arrbuffer);
	buffer.setUint16(0,1002,true);
	buffer.setUint16(2,2001,true);
	buffer.setUint32(4,0,true);

	// let byte:Laya.Byte = new Laya.Byte(arrbuffer);
	let byte:DataView = new DataView(arrbuffer);
	console.log(byte.getUint16(0,true));
	console.log(byte.getUint16(2,true));
	console.log(byte.getUint32(4,true));

}

function onLoaded(): void {
	//实例UI界面
	var testUI: TestUI = new TestUI();
	Laya.stage.addChild(testUI);
	// lalal_test();
}
