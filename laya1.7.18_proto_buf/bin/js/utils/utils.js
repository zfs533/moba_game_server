class utils {
    static get_rand_str(len) {
        let str = "";
        str += "0123456789";
        str += "abcdefghijklmnopqrstuvwxyz";
        str += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        let ret = "";
        for (let i = 0; i < len; i++) {
            let rand = Math.floor(Math.random() * str.length);
            ret += str[rand];
        }
        return ret;
    }
    static md5(str) {
        return Md5.Instance.get_md5(str);
    }
}
//# sourceMappingURL=utils.js.map