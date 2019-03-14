/*
* name;
*/
var Network = /** @class */ (function () {
    function Network() {
        this.socket = null;
        this.HEADER = 8;
        this.socket = new Laya.Socket();
        this.socket.connectByUrl("ws:\\10.0.7.65:3000");
        this.socket.on(Laya.Event.OPEN, this, this.socket_open);
        this.socket.on(Laya.Event.MESSAGE, this, this.socket_recv_message);
        this.socket.on(Laya.Event.CLOSE, this, this.socket_close);
        this.socket.on(Laya.Event.ERROR, this, this.socket_error);
    }
    Object.defineProperty(Network, "Instance", {
        get: function () {
            return this._inst || (this._inst = new Network());
        },
        enumerable: true,
        configurable: true
    });
    Network.prototype.socket_close = function () {
        console.log("server_closed");
    };
    Network.prototype.socket_error = function (e) {
        console.log(e);
    };
    Network.prototype.socket_open = function (e) {
        if (e === void 0) { e = null; }
        console.log("connect success..." + e);
    };
    Network.prototype.socket_recv_message = function (message) {
        console.log(message);
        if (typeof message == "string") {
        }
        else if (message instanceof ArrayBuffer) {
            var byte = new Laya.Byte(message);
            var stp = byte.getInt16();
            var ctp = byte.getInt16();
            var utg = byte.getInt16();
            var bdy = byte.getUint8Array(this.HEADER, byte.length - this.HEADER);
            EventManager.Instance.dispatch_register_event(ctp, bdy);
        }
    };
    Network.prototype.send_msg = function (stype, ctype, proto_obj, proto_data) {
        var buffer = this.encode_data(stype, ctype, proto_obj, proto_data);
        this.socket.send(buffer);
    };
    // {stype:2,ctype:2,utag:4,body:....}
    Network.prototype.encode_data = function (stype, ctype, proto_obj, proto_data) {
        return ProtoManager.Instance.encode_protobuf(stype, ctype, proto_obj, proto_data);
    };
    Network._inst = null;
    return Network;
}());
//# sourceMappingURL=Network.js.map