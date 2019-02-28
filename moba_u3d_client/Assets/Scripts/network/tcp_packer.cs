using System;
public class tcp_packer
{
    private const int HEADER_SIZE = 2;
    public static byte[] pack(byte[] cmd_data)
    {
        int len = cmd_data.Length;
        if (len > 65535 - 2)
        {
            return null;
        }
        int cmd_len = len + HEADER_SIZE;
        byte[] cmd = new byte[cmd_len];
        data_viewer.write_ushort_le(cmd, 0, (ushort)(cmd_len));
        data_viewer.write_bytes(cmd, HEADER_SIZE, cmd_data);
        return cmd;
    }
    public static bool read_header(byte[] data, int data_len, out int pkg_size, out int head_size)
    {
        pkg_size = 0;
        head_size = 0;

        if (data_len < 2)
        {
            return false;
        }
        head_size = 2;
        pkg_size = (data[0] | (data[1] << 8));

        return true;
    }
}
