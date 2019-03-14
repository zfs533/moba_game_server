/*
* name;
*/
var Cmd = /** @class */ (function () {
    function Cmd() {
        this.cmd_arr = [
            { ctype: Cmd.eLoginReq, name: null },
            { ctype: Cmd.eLoginRes, name: Cmd.LoginRes },
            { ctype: Cmd.eExitReq, name: null },
            { ctype: Cmd.eExitRes, name: Cmd.ExitRes },
            { ctype: Cmd.eSendMsgReq, name: Cmd.SendMsgReq },
            { ctype: Cmd.eSendMsgRes, name: Cmd.SendMsgRes },
            { ctype: Cmd.eOnUserLogin, name: Cmd.OnUserLogin },
            { ctype: Cmd.eOnUserExit, name: Cmd.OnUserExit },
            { ctype: Cmd.eOnSendMsg, name: Cmd.OnSendMsg },
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
    Cmd.eLoginReq = 1;
    Cmd.eLoginRes = 2;
    Cmd.eExitReq = 3;
    Cmd.eExitRes = 4;
    Cmd.eSendMsgReq = 5;
    Cmd.eSendMsgRes = 6;
    Cmd.eOnUserLogin = 7;
    Cmd.eOnUserExit = 8;
    Cmd.eOnSendMsg = 9;
    //-----------------------------------------------------------------------------
    Cmd.LoginRes = "LoginRes";
    Cmd.ExitRes = "ExitRes";
    Cmd.SendMsgReq = "SendMsgReq";
    Cmd.SendMsgRes = "SendMsgRes";
    Cmd.OnUserLogin = "OnUserLogin";
    Cmd.OnUserExit = "OnUserExit";
    Cmd.OnSendMsg = "OnSendMsg";
    return Cmd;
}());
//# sourceMappingURL=Cmd.js.map