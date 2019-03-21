/*
* name;
*/
class Cmd {
    constructor() {
        this.cmd_arr = [
            { ctype: Cmd.eGuestLoginReq, name: Cmd.GuestLoginReq },
            { ctype: Cmd.eGuestLoginRes, name: Cmd.GuestLoginRes },
            { ctype: Cmd.eRelogin, name: Cmd.Relogin },
            { ctype: Cmd.eUserLostConn, name: Cmd.UserLostConn },
        ];
    }
    static get Instance() {
        return this._inst || (this._inst = new Cmd());
    }
    get_cmd_data(ctype) {
        for (let i = 0; i < this.cmd_arr.length; i++) {
            if (this.cmd_arr[i].ctype == ctype) {
                return this.cmd_arr[i];
            }
        }
        return null;
    }
}
Cmd._inst = null;
Cmd.server_type = 1;
Cmd.eGuestLoginReq = 1;
Cmd.eGuestLoginRes = 2;
Cmd.eRelogin = 3;
Cmd.eUserLostConn = 4;
//-----------------------------------------------------------------------------
Cmd.GuestLoginReq = "GuestLoginReq";
Cmd.GuestLoginRes = "GuestLoginRes";
Cmd.Relogin = "Relogin";
Cmd.UserLostConn = "UserLostConn";
//# sourceMappingURL=Cmd.js.map