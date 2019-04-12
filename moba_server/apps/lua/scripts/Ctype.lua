--[[
server cmd order: common with *.proto file
]]
local Cmd = 
{
	eGuestLoginReq = 1,
	eGuestLoginRes = 2,
	eRelogin = 3,
	eUserLostConn = 4,
	eEditProfileReq = 5,
	eEditProfileRes = 6,
	eAccountUpgradeReq = 7,
	eAccountUpgradeRes = 8,
	eUnameLoginReq = 9,
	eUnameLoginRes = 10,
	eLoginOutReq = 11,
	eLoginOutRes = 12,
	--------------system server-----------------
	eGetUgameInfoReq = 13,
	eGetUgameInfoRes = 14,
	eRecvLoginBonuesReq = 15,
	eRecvLoginBonuesRes = 16,
	eGetWorldRankUchipReq = 17,
	eGetWorldRankUchipRes = 18,
	eGetSystemMessageReq = 19,
	eGetSystemMessageRes = 20,
	eLoginLogicReq = 21,
	eLoginLogicRes = 22,
}

return Cmd
