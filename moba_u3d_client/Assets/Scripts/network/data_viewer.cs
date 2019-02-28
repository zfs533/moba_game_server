using System;

public class data_viewer
{
    public static void write_ushort_le(byte[] buf, int offset, ushort value)
    { 
        //value convert to byte[]
        byte[] byte_value = BitConverter.GetBytes(value);
        //小尾还是大尾，取决于系统是小尾还是大尾，这里统统用小尾
        if (!BitConverter.IsLittleEndian)
        {
            Array.Reverse(byte_value);
        }
        Array.Copy(byte_value, 0, buf, offset, byte_value.Length);
    }
    public static void write_uint_le(byte[] buf, int offset, ushort value)
    {
        //value convert to byte[]
        byte[] byte_value = BitConverter.GetBytes(value);
        //小尾还是大尾，取决于系统是小尾还是大尾，这里统统用小尾
        if (!BitConverter.IsLittleEndian)
        {
            Array.Reverse(byte_value);
        }
        Array.Copy(byte_value, 0, buf, offset, byte_value.Length);
    }

    public static void write_bytes(byte[] dst, int offset, byte[] value)
    {
        Array.Copy(value, 0, dst,offset, value.Length);
    }

    public static ushort read_ushort_len(byte[] data, int offset)
    {
        int ret = data[offset] | (data[offset + 1] << 8);
        return (ushort)ret;
    }
}
