/*
* name;
*/
var Cmd = /** @class */ (function () {
    function Cmd() {
        this.cmd_arr = [
            { ctype: Cmd.eGuestLoginReq, name: Cmd.GuestLoginReq },
            { ctype: Cmd.eGuestLoginRes, name: Cmd.GuestLoginRes },
        ];
    }
    Object.defineProperty(Cmd, "Instance", {
        get: function () {
            return this._inst || (this._inst = new Cmd());
        },
        enumerable: true,
        configurable: true
    });
    Cmd.prototype.get_cmd_data = function (ctype) {
        for (var i = 0; i < this.cmd_arr.length; i++) {
            if (this.cmd_arr[i].ctype == ctype) {
                return this.cmd_arr[i];
            }
        }
        return null;
    };
    Cmd._inst = null;
    Cmd.server_type = 1;
    Cmd.eGuestLoginReq = 1;
    Cmd.eGuestLoginRes = 2;
    //-----------------------------------------------------------------------------
    Cmd.GuestLoginReq = "GuestLoginReq";
    Cmd.GuestLoginRes = "GuestLoginRes";
    return Cmd;
}());
//# sourceMappingURL=Cmd.js.map