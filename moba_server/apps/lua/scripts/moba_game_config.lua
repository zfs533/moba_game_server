local moba_game_config = 
{
    ugame = --默认游戏初始数据
    {
        uchip = 2000,--金币
        uvip = 0,
        uexp = 0,
    },
    --连续登录5天，超过5天重新开始
    login_bonues = {1000,2000,3000,4000,5000},
}

return moba_game_config