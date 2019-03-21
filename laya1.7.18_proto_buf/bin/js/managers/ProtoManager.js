/*
* name;
*/
class ProtoManager {
    constructor() {
        this.Pbf = Laya.Browser.window.protobuf;
        this.HEADER = 8;
        this.Pbf.load("res/game.proto", this.load_proto_finish);
    }
    static get Instance() {
        return this._inst || (this._inst = new ProtoManager());
    }
    load_proto_finish(err, root) {
        if (err) {
            throw (err);
        }
        console.log("success");
        ProtoManager.root = root;
        console.log(ProtoManager.root);
    }
    decode_protobuf(ctp, body) {
        if (!ProtoManager.root) {
            console.log("please load proto file first");
        }
        let data_module = this.get_protobuf_Data(ctp);
        let data = data_module.decode(body);
        return data;
        // let cmd = root.lookup("Cmd").values.eOnSendMsg;
    }
    get_protobuf_Data(ctp) {
        if (!ProtoManager.root) {
            console.log("please load proto file first");
        }
        let name = Cmd.Instance.get_cmd_data(ctp).name;
        let data_module = ProtoManager.root.lookup(name);
        return data_module;
    }
    // {stype:2,ctype:2,utag:4,body:....}
    encode_protobuf(stype, ctype, proto_obj, proto_data) {
        let buffer = proto_obj.encode(proto_data).finish();
        let len = buffer.length;
        let cmd_len = len + this.HEADER;
        let arrbuf = new ArrayBuffer(cmd_len);
        let s_type = new Int16Array(arrbuf, 0, 1); //0-1
        s_type[0] = stype;
        let c_type = new Int16Array(arrbuf, 2, 1); //2-3
        c_type[0] = ctype;
        let utag = new Int16Array(arrbuf, 4, 2); //4-7
        let body = new Uint8Array(arrbuf, 8, buffer.length);
        for (let i = 0; i < buffer.length; i++) {
            body[i] = buffer[i];
        }
        return arrbuf;
    }
}
ProtoManager._inst = null;
ProtoManager.root = null;
//http://ask.layabox.com/question/2687
/**
1、工程新建的内容我就不罗嗦了。先写一个awesome.proto文件，内容：
package awesomepackage;
message AwesomeMessage {
    required string awesomeField = 1;
}
 
放在laya/proto下面，proto这个文件夹没有请自己创建。
 
2、在bin下的index.html里的这个位置，看粗体部分，protobuf.js在项目里已经存在了，不需要再引用外部的protobuf库：

<!--用户自定义顺序文件添加到这里-->
    <!--jsfile--Custom-->
    <script type="text/javascript" src="libs/protobuf.js"></script>
    <!--jsfile--Custom-->
3、测试代码参考了上面那个连接，但稍微做了修改：

// 程序入口
class GameMain{
    constructor(){
        var Loader = Laya.Loader;
        var Browser = Laya.Browser;
        var Handler = Laya.Handler;
        var ProtoBuf = Browser.window.protobuf;
        Laya.init(550, 400);
        //加载协议文件，相当于加载那个资源
        ProtoBuf.load("../laya/proto/awesome.proto", onAssetsLoaded);
        //回调函数
        function onAssetsLoaded(err, root){
            if (err)
                throw err;
            // Obtain a message type
            var AwesomeMessage = root.lookup("awesomepackage.AwesomeMessage");

            // Create a new message  创建一条协议内容
            var message = AwesomeMessage.create({
                awesomeField: "AwesomeString"
            });

            // Verify the message if necessary (i.e. when possibly incomplete or invalid)
            var errMsg = AwesomeMessage.verify(message);
            if (errMsg)
                throw Error(errMsg);

            // Encode a message to an Uint8Array (browser) or Buffer (node)
            var buffer = AwesomeMessage.encode(message).finish();

            // ... do something with buffer
            // Or, encode a plain object  也可以用这种方式创建这个数据然后编码
            var buffer = AwesomeMessage.encode({
                awesomeField: "AwesomeString"
            }).finish();

            // ... do something with buffer
            // Decode an Uint8Array (browser) or Buffer (node) to a message   解码处理
            var message = AwesomeMessage.decode(buffer);
 
            // 在这里把解码的数据控制台打印，看看结果
            console.log(message);
            // ... do something with message
            // If your application uses length-delimited buffers, there is also encodeDelimited and decodeDelimited.
        }
    }
}
new GameMain();
 
4、最后在控制台输出：
AwesomeMessage {awesomeField: "AwesomeString"}
 */ 
//# sourceMappingURL=ProtoManager.js.map