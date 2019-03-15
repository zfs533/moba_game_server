/*
* name;
*/
class ProtoManager
{
    private static _inst:ProtoManager = null;
    public static get Instance():ProtoManager
    {
        return this._inst || (this._inst = new ProtoManager());
    }
    private Pbf = Laya.Browser.window.protobuf;
    private static root = null;
    private HEADER:number = 8;
    constructor()
    {
        this.Pbf.load("res/game.proto",this.load_proto_finish);
    
    }
    private load_proto_finish(err,root)
    {
        if(err)
        {
            throw(err);
        }
        console.log("success");
        ProtoManager.root = root;
        console.log(ProtoManager.root);
    }
    public decode_protobuf(ctp:number,body:Uint8Array):any
    {
        if(!ProtoManager.root){console.log("please load proto file first");}

        let data_module = this.get_protobuf_Data(ctp);
        let data = data_module.decode(body);
        return data;
        // let cmd = root.lookup("Cmd").values.eOnSendMsg;
    }

    public get_protobuf_Data(ctp:number):any
    {
        if(!ProtoManager.root){console.log("please load proto file first");}
        
        let name = Cmd.Instance.get_cmd_data(ctp).name;
        let data_module = ProtoManager.root.lookup(name);
        return data_module;
    }
    // {stype:2,ctype:2,utag:4,body:....}
    public encode_protobuf(stype:number,ctype:number,proto_obj:any,proto_data):ArrayBuffer
    {
        let buffer = proto_obj.encode(proto_data).finish();
        let len:number = buffer.length;
        let cmd_len = len + this.HEADER;
        let arrbuf:ArrayBuffer = new ArrayBuffer(cmd_len);
        let s_type:Int16Array = new Int16Array(arrbuf,0,1);//0-1
        s_type[0] = stype;
        let c_type:Int16Array = new Int16Array(arrbuf,2,1);//2-3
        c_type[0] = ctype;
        let utag:Int16Array  = new Int16Array(arrbuf,4,2);//4-7
        let body:Uint8Array  = new Uint8Array(arrbuf,8,buffer.length);
        for(let i = 0; i<buffer.length;i++)
        {
            body[i] = buffer[i];
        }
        return arrbuf;
    }
}