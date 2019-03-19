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

    public static eGuestLoginReq         = 1;
    public static eGuestLoginRes         = 2;
    public static eRelogin               = 3;
    public static eUserLostConn          = 4;
    //-----------------------------------------------------------------------------
    public static GuestLoginReq      = "GuestLoginReq";
    public static GuestLoginRes      = "GuestLoginRes";
    public static Relogin            = "Relogin";
    public static UserLostConn       = "UserLostConn";
    
    private cmd_arr:Array<any> =
    [
        {ctype:Cmd.eGuestLoginReq   ,name:Cmd.GuestLoginReq},
        {ctype:Cmd.eGuestLoginRes   ,name:Cmd.GuestLoginRes},
        {ctype:Cmd.eRelogin         ,name:Cmd.Relogin},
        {ctype:Cmd.eUserLostConn    ,name:Cmd.UserLostConn},
    ];
    
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