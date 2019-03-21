class utils
{
    public static get_rand_str(len:number):string
    {
        let str:string = "";
        str += "0123456789";
        str += "abcdefghijklmnopqrstuvwxyz";
        str += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        let ret:string = "";
        for(let i = 0; i<len; i++)
        {
            let rand:number = Math.floor(Math.random()*str.length);
            ret += str[rand];
        }
        return ret;
    }

    public static md5(str:string):string
    {
        return Md5.Instance.get_md5(str);
    }
}