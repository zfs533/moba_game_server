using System;
using System.IO;
using System.Text;
using ProtoBuf;

public class cmd_msg 
{
    public int stype;
    public int ctype;
    public byte[] body;//protoubf utf8,string json type
}

public class proto_man
{
    private const int HEADER_SIZE = 8;//2 stype,2 ctype, 4 utag, msg-->body
    private static byte[] protobuf_serializer(ProtoBuf.IExtensible data)
    {
        using (MemoryStream m = new MemoryStream())
        {
            byte[] buffer = null;
            Serializer.Serialize(m, data);
            m.Position = 0;
            int length = (int)m.Length;
            buffer = new byte[length];
            m.Read(buffer, 0, length);
            return buffer;
        }
    }
    public static byte[] pack_protobuf_cmd(int stype, int ctype, ProtoBuf.IExtensible msg)
    {
        int cmd_len = HEADER_SIZE;
        byte[] cmd_body = null;
        if (msg != null)
        {
            cmd_body = protobuf_serializer(msg);
            cmd_len += cmd_body.Length;
        }
        byte[] cmd = new byte[cmd_len];
        //stype ctype utag,cmd_body
        data_viewer.write_ushort_le(cmd, 0, (ushort)stype);
        data_viewer.write_ushort_le(cmd, 2, (ushort)ctype);
        if (cmd_body != null)
        {
            data_viewer.write_bytes(cmd, HEADER_SIZE, cmd_body);
        }
        return cmd;
    }

    public static byte[] pack_json_cmd(int stype, int ctype, string json_msg)
    {
        int cmd_len = HEADER_SIZE;
        byte[] cmd_body = null;

        if (json_msg.Length > 0)
        { // utf8
            cmd_body = Encoding.UTF8.GetBytes(json_msg);
            cmd_len += cmd_body.Length;
        }

        byte[] cmd = new byte[cmd_len];
        // stype, ctype, utag(4保留), cmd_body
        data_viewer.write_ushort_le(cmd, 0, (ushort)stype);
        data_viewer.write_ushort_le(cmd, 2, (ushort)ctype);
        if (cmd_body != null)
        {
            data_viewer.write_bytes(cmd, HEADER_SIZE, cmd_body);
        }
        return cmd;
    }
    //解码
    public static bool unpack_cmd_msg(byte[] data, int start, int cmd_len, out cmd_msg msg)
    {
        msg = new cmd_msg();
        msg.stype = data_viewer.read_ushort_len(data, start);
        msg.ctype = data_viewer.read_ushort_len(data, start + 2);
        int body_len = cmd_len - HEADER_SIZE;
        msg.body = new byte[body_len];
        Array.Copy(data, start + HEADER_SIZE, msg.body, 0, body_len);
        return true;
    }
    //protobuf解码函数
    public static T protobuf_deserialize<T>(byte[] _data)
    {
        using (MemoryStream m = new MemoryStream(_data))
        {
            return Serializer.Deserialize<T>(m);
        }
    }
}
