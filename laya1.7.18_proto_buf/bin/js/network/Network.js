/*
* name;
*/
class Network {
    constructor() {
        this.socket = null;
        this.HEADER = 8;
        this.socket = new Laya.Socket();
        this.socket.connectByUrl("ws:\\10.0.7.65:3000");
        this.socket.on(Laya.Event.OPEN, this, this.socket_open);
        this.socket.on(Laya.Event.MESSAGE, this, this.socket_recv_message);
        this.socket.on(Laya.Event.CLOSE, this, this.socket_close);
        this.socket.on(Laya.Event.ERROR, this, this.socket_error);
    }
    static get Instance() {
        return this._inst || (this._inst = new Network());
    }
    socket_close() {
        console.log("server_closed");
    }
    socket_error(e) {
        console.log(e);
    }
    socket_open(e = null) {
        console.log("connect success..." + e);
    }
    socket_recv_message(message) {
        console.log(message);
        if (typeof message == "string") {
        }
        else if (message instanceof ArrayBuffer) {
            let byte = new Laya.Byte(message);
            let stp = byte.getInt16();
            let ctp = byte.getInt16();
            let utg = byte.getInt16();
            let bdy = byte.getUint8Array(this.HEADER, byte.length - this.HEADER);
            EventManager.Instance.dispatch_register_event(ctp, bdy);
        }
    }
    send_msg(stype, ctype, proto_obj, proto_data) {
        let buffer = this.encode_data(stype, ctype, proto_obj, proto_data);
        this.socket.send(buffer);
    }
    // {stype:2,ctype:2,utag:4,body:....}
    encode_data(stype, ctype, proto_obj, proto_data) {
        return ProtoManager.Instance.encode_protobuf(stype, ctype, proto_obj, proto_data);
    }
}
Network._inst = null;
//# sourceMappingURL=Network.js.map