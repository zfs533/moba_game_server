/*
* name;
*/
class Network
{
    private static _inst:Network = null;
    public static get Instance():Network
    {
        return this._inst || (this._inst = new Network());
    }
    private socket:Laya.Socket = null;
    private HEADER:number = 8;
    constructor()
    {
        this.socket = new Laya.Socket();
        this.socket.connectByUrl("ws:\\10.0.7.65:3000");
        this.socket.on(Laya.Event.OPEN,this,this.socket_open);
        this.socket.on(Laya.Event.MESSAGE,this,this.socket_recv_message)
        this.socket.on(Laya.Event.CLOSE,this,this.socket_close);
        this.socket.on(Laya.Event.ERROR,this,this.socket_error);
    }
    private socket_close():void
    {
        console.log("server_closed");
    }
    private socket_error(e):void
    {
        console.log(e);
    }
    private socket_open(e:any = null):void
    {
        console.log("connect success..."+e);
    }
    private socket_recv_message(message:any):void
    {
        console.log(message);
        if (typeof message == "string")
        {
        }
        else if(message instanceof ArrayBuffer)
        {
            let byte:Laya.Byte = new Laya.Byte(message);
            let stp = byte.getInt16();
            let ctp = byte.getInt16();
            let utg = byte.getInt16();
            let bdy = byte.getUint8Array(this.HEADER,byte.length-this.HEADER);
            EventManager.Instance.dispatch_register_event(ctp,bdy);
        }
    }
    public send_msg(stype:number,ctype:number,proto_obj:any,proto_data):void
    {
        let buffer:ArrayBuffer = this.encode_data(stype,ctype,proto_obj,proto_data);
        this.socket.send(buffer);
    }
    // {stype:2,ctype:2,utag:4,body:....}
    private encode_data(stype:number,ctype:number,proto_obj:any,proto_data):ArrayBuffer
    {
        return ProtoManager.Instance.encode_protobuf(stype,ctype,proto_obj,proto_data);
    }
}

