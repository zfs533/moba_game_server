/*
* name;
*/
var ProtoManager = /** @class */ (function () {
    function ProtoManager() {
        this.Pbf = Laya.Browser.window.protobuf;
        this.HEADER = 8;
        this.Pbf.load("res/game.proto", this.load_proto_finish);
    }
    Object.defineProperty(ProtoManager, "Instance", {
        get: function () {
            return this._inst || (this._inst = new ProtoManager());
        },
        enumerable: true,
        configurable: true
    });
    ProtoManager.prototype.load_proto_finish = function (err, root) {
        if (err) {
            throw (err);
        }
        console.log("success");
        ProtoManager.root = root;
        console.log(ProtoManager.root);
    };
    ProtoManager.prototype.decode_protobuf = function (ctp, body) {
        if (!ProtoManager.root) {
            console.log("please load proto file first");
        }
        var data_module = this.get_protobuf_Data(ctp);
        var data = data_module.decode(body);
        return data;
        // let cmd = root.lookup("Cmd").values.eOnSendMsg;
    };
    ProtoManager.prototype.get_protobuf_Data = function (ctp) {
        if (!ProtoManager.root) {
            console.log("please load proto file first");
        }
        var name = Cmd.Instance.get_cmd_data(ctp).name;
        var data_module = ProtoManager.root.lookup(name);
        return data_module;
    };
    // {stype:2,ctype:2,utag:4,body:....}
    ProtoManager.prototype.encode_protobuf = function (stype, ctype, proto_obj, proto_data) {
        var buffer = proto_obj.encode(proto_data).finish();
        var len = buffer.length;
        var cmd_len = len + this.HEADER;
        var arrbuf = new ArrayBuffer(cmd_len);
        var s_type = new Int16Array(arrbuf, 0, 1); //0-1
        s_type[0] = stype;
        var c_type = new Int16Array(arrbuf, 2, 1); //2-3
        c_type[0] = ctype;
        var utag = new Int16Array(arrbuf, 4, 2); //4-7
        var body = new Uint8Array(arrbuf, 8, buffer.length);
        for (var i = 0; i < buffer.length; i++) {
            body[i] = buffer[i];
        }
        return arrbuf;
    };
    ProtoManager._inst = null;
    ProtoManager.root = null;
    return ProtoManager;
}());
//# sourceMappingURL=ProtoManager.js.map