using System.Net;
using System.Net.Sockets;
using System;
using System.Threading;

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class network : UnitySingletom<network>
{
    public string server_ip = "127.0.0.1";
    public int port = 6080;
    
    private Socket client_socket = null;
    private bool is_connect = false;
    private Thread recv_thread = null;
    private const int RECV_LEN = 8192;
    private byte[] recv_buf = new byte[RECV_LEN];
    //udp
    private string udp_server_ip = "127.0.0.1";
    private int udp_port = 8800;
    IPEndPoint udp_remote_point;
    Socket udp_socket = null;
    private byte[] udp_recv_buf = new byte[60 * 1024];
    private Thread udp_recv_thread = null;
    public int local_udp_port = 8888;
    //end

    private int recved;
    private byte[] long_pkg = null;
    private int long_pkg_size = 0;
    //----------event
    //event queue/list
    private Queue<cmd_msg> net_events = new Queue<cmd_msg>();
    //event listener delegate -监听者
    public delegate void net_message_handler(cmd_msg msg);
    //监听和监听的map
    private Dictionary<int, net_message_handler> event_listeners = new Dictionary<int, net_message_handler>();
    //----------

    void Start()
    {
        this.connect_to_server();
        this.udp_socket_init();
        //test udp
        //this.InvokeRepeating("test_udp", 5, 5);
    }

    private void test_udp()
    {
        logic_service.Instance.send_udp_test("Hello World------------");
    }

    void OnDestroy()
    {
        this.close_socket();
    }

    void OnApplicaitonQuit()
    {
        this.close_socket();
        this.udp_close();
    }

    void Update()
    {
        lock (this.net_events)
        {
            while (this.net_events.Count > 0)
            {
                cmd_msg msg = this.net_events.Dequeue();
                //recv a package then call callfun
                if (this.event_listeners.ContainsKey(msg.stype))
                {
                    this.event_listeners[msg.stype](msg);
                }
            }
        }
    }

    void on_connect_error(string err)
    { }

    void on_recv_cmd(byte[] data, int start, int data_len)
    {
        cmd_msg msg;
        proto_man.unpack_cmd_msg(data, start, data_len,out msg);
        if (msg != null)
        {
            lock (this.net_events)//recv thread
            {
                this.net_events.Enqueue(msg);
            }
        }
    }

    void on_recv_tcp_data()
    {
        byte[] pkg_data = (this.long_pkg != null) ? this.long_pkg : this.recv_buf;
        while (this.recved > 0)
        {
            int pkg_size = 0;
            int head_size = 0;
            if (!tcp_packer.read_header(pkg_data, this.recved, out pkg_size, out head_size))
            {
                break;
            }
            if (this.recved < pkg_size)
            {
                break;
            }
            int raw_data_start = head_size;
            int raw_data_len = pkg_size - head_size;
            
            on_recv_cmd(pkg_data, raw_data_start, raw_data_len);
            
            if (this.recved > pkg_size)
            {
                this.recv_buf = new byte[RECV_LEN];
                Array.Copy(pkg_data, pkg_size, this.recv_buf, 0, this.recved - pkg_size);
                pkg_data = this.recv_buf;
            }
            this.recved -= pkg_size;
            if (this.recved == 0 && this.long_pkg != null)
            {
                this.long_pkg = null;
                this.long_pkg_size = 0;
            }
        }
    }

    void connect_to_server()
    {
        try
        {
            this.client_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress ipAddress = IPAddress.Parse(this.server_ip);
            IPEndPoint ipEndPoint = new IPEndPoint(ipAddress, this.port);
            IAsyncResult result = this.client_socket.BeginConnect(ipEndPoint, new AsyncCallback(this.on_connected), this.client_socket);
            bool success = result.AsyncWaitHandle.WaitOne(5000, true);
            if (!success)
            {//timeout
                this.on_connect_timeout();
            }
        }
        catch (System.Exception e)
        {
            Debug.Log(e.ToString());
            this.on_connect_error(e.ToString());
        }
    }

    void thread_recv_worker()
    {
        if (!this.is_connect)
        {
            return;
        }
        while (true)
        {
            if (!this.client_socket.Connected)
            {
                break;
            }
            try
            {
                int recv_len = 0;
                if (this.recved < RECV_LEN)
                {
                    recv_len = this.client_socket.Receive(this.recv_buf, this.recved, RECV_LEN - this.recved, SocketFlags.None);
                }
                else
                {
                    if (this.long_pkg == null)
                    {
                        int pkg_size;
                        int head_size;
                        tcp_packer.read_header(this.recv_buf, this.recved, out pkg_size, out head_size);
                        this.long_pkg_size = pkg_size;
                        this.long_pkg = new byte[pkg_size];
                        Array.Copy(this.recv_buf, 0, this.long_pkg, 0, this.recved);
                    }
                    recv_len = this.client_socket.Receive(this.long_pkg, this.recved, this.long_pkg_size - this.recved, SocketFlags.None);
                }
                if (recv_len > 0)
                {
                    this.recved += recv_len;
                    this.on_recv_tcp_data();
                }
            }
            catch (Exception e) 
            {
                Debug.Log(e.ToString());
                this.on_connect_error(e.ToString());
                if (this.client_socket != null)
                {
                    this.client_socket.Disconnect(true);
                    this.client_socket.Shutdown(SocketShutdown.Both);
                    this.client_socket.Close();
                }
                this.is_connect = false;
            }
        }
    }

    void on_connected(IAsyncResult iar)
    {
        try 
        {
            Socket client = (Socket)iar.AsyncState;
            client.EndConnect(iar);

            this.is_connect = true;
            //connect finish then start a thread to recv data
            this.recv_thread = new Thread(new ThreadStart(this.thread_recv_worker));
            this.recv_thread.Start();

            Debug.Log("connect to server success" + this.server_ip + ":" + this.port + "!");
        }
        catch (Exception e) 
        {
            Debug.Log(e.ToString());
            this.on_connect_error(e.ToString());
            this.is_connect = false;
        }
    }
    void on_connect_timeout()
    { 
    
    }
    void close_socket()
    {
        if (!this.is_connect)
        {
            return;
        }
        if (this.recv_thread != null)
        {
            this.recv_thread.Interrupt();
            this.recv_thread.Abort();
            this.recv_thread = null;
        }
        if (this.client_socket != null && this.client_socket.Connected)
        {
            this.client_socket.Close();
            this.client_socket = null;
        }
    }

    private void on_send_data_cb(IAsyncResult iar)
    {
        try
        {
            Socket client = (Socket)iar.AsyncState;
            client.EndSend(iar);
        }
        catch (Exception e)
        {
            Debug.Log(e.ToString());
            this.on_connect_error(e.ToString());
        }
    }

    public void send_protobuf_cmd(int stype, int ctype, ProtoBuf.IExtensible body)
    {
        byte[] cmd_data = proto_man.pack_protobuf_cmd(stype, ctype, body);
        if (cmd_data == null)
        {
            Debug.Log("cmd_data is null");
        }
        byte[] tcp_pkg = tcp_packer.pack(cmd_data);
        this.client_socket.BeginSend(tcp_pkg, 0, tcp_pkg.Length, SocketFlags.None, new AsyncCallback(this.on_send_data_cb), this.client_socket);
    }

    public void send_json_cmd(int stype, int ctype, string json_body)
    {
        byte[] cmd_data = proto_man.pack_json_cmd(stype, ctype, json_body);
        if (cmd_data == null)
        {
            return;
        }

        byte[] tcp_pkg = tcp_packer.pack(cmd_data);
        // end 
        this.client_socket.BeginSend(tcp_pkg, 0, tcp_pkg.Length, SocketFlags.None, new AsyncCallback(this.on_send_data_cb), this.client_socket);
        // end 
    }

    public void add_service_listener(int stype, net_message_handler handler)
    {
        if (this.event_listeners.ContainsKey(stype))
        {
            this.event_listeners[stype] += handler;
        }
        else
        {
            this.event_listeners.Add(stype, handler);
        }
    }
    public void remove_service_listener(int stype, net_message_handler handler)
    {
        if (!this.event_listeners.ContainsKey(stype))
        {
            return;
        }
        this.event_listeners[stype] -= handler;
        if (this.event_listeners[stype] == null)
        {
            this.event_listeners.Remove(stype);
        }
    }
    //udp
    private void udp_thread_recv_worker()
    {
        while (true)
        {
            EndPoint remote = (EndPoint)new IPEndPoint(IPAddress.Parse(this.udp_server_ip), this.udp_port);
            int recved = this.udp_socket.ReceiveFrom(this.udp_recv_buf, ref remote);
            this.on_recv_cmd(this.udp_recv_buf, 0, recved);
        }
    }

    private void udp_socket_init()
    { 
        //udp远程的端口
        this.udp_remote_point = new IPEndPoint(IPAddress.Parse(this.udp_server_ip), this.udp_port);
        //创建一个udp socket
        this.udp_socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        //收数据，用另外一个线程，如果不绑定，就无法收到数据
        IPEndPoint local_point = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8888);
        this.udp_socket.Bind(local_point);
        //启动一个线程来收Udp的数据在特定的端口上
        this.udp_recv_thread = new Thread(new ThreadStart(this.udp_thread_recv_worker));
        this.udp_recv_thread.Start();
    }

    private void udp_close()
    {
        if (this.udp_recv_thread != null)
        {
            this.udp_recv_thread.Interrupt();
            this.udp_recv_thread.Abort();
            this.udp_recv_thread = null;
        }
        if (this.udp_socket != null)
        {
            this.udp_socket.Close();
            this.udp_socket = null;
        }
    }

    private void on_udp_send_data(IAsyncResult iar)
    {
        try
        {
            Socket client = (Socket)iar.AsyncState;
            client.EndSend(iar);
        }
        catch (System.Exception e)
        {
            Debug.Log(e.ToString());
        }
    }

    public void udp_send_protobuf_cmd(int stype, int ctype, ProtoBuf.IExtensible body)
    {
        byte[] cmd_data = proto_man.pack_protobuf_cmd(stype, ctype, body);
        if (cmd_data == null)
        {
            return;
        }
        this.udp_socket.BeginSendTo(cmd_data, 0, cmd_data.Length, SocketFlags.None, this.udp_remote_point, new AsyncCallback(this.on_udp_send_data), this.udp_socket);
    }
}
















