/*
* name;
*/
class Cmd
{
    private static _inst:Cmd = null;
    public static get Instance():Cmd
    {
        return this._inst || (this._inst = new Cmd());
    }

    public static server_type = 1;

    public static eLoginReq         = 1;
	public static eLoginRes         = 2;
	public static eExitReq      = 3;
	public static eExitRes      = 4;
	public static eSendMsgReq       = 5;
	public static eSendMsgRes    = 6;
	public static eOnUserLogin   = 7;
	public static eOnUserExit    = 8;
	public static eOnSendMsg     = 9;
    //-----------------------------------------------------------------------------
    public static LoginRes      = "LoginRes";
    public static ExitRes       = "ExitRes";
    public static SendMsgReq        = "SendMsgReq";
    public static SendMsgRes    = "SendMsgRes";
    public static OnUserLogin    = "OnUserLogin";
    public static OnUserExit    = "OnUserExit";
    public static OnSendMsg     = "OnSendMsg";
    
    private cmd_arr:Array<any> =
    [
        {ctype:Cmd.eLoginReq   ,name:null},
        {ctype:Cmd.eLoginRes   ,name:Cmd.LoginRes},
        {ctype:Cmd.eExitReq    ,name:null},
        {ctype:Cmd.eExitRes    ,name:Cmd.ExitRes},
        {ctype:Cmd.eSendMsgReq ,name:Cmd.SendMsgReq },
        {ctype:Cmd.eSendMsgRes ,name:Cmd.SendMsgRes },
        {ctype:Cmd.eOnUserLogin,name:Cmd.OnUserLogin},
        {ctype:Cmd.eOnUserExit ,name:Cmd.OnUserExit },
        {ctype:Cmd.eOnSendMsg  ,name:Cmd.OnSendMsg  },
    ];
    constructor()
    {
    }
    public get_cmd_data(ctype:number):any
    {
        for(let i = 0; i<this.cmd_arr.length;i++)
        {
            if(this.cmd_arr[i].ctype == ctype)
            {
                return this.cmd_arr[i];
            }
        }
        return null;
    }
}