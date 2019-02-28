using System.Net;
using System.Net.Sockets;
using System;
using System.Threading;

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class network : MonoBehaviour
{
    public string server_ip = "127.0.0.1";
    public int port = 6080;
    
    private Socket client_socket = null;
    private bool is_connect = false;
    private Thread recv_thread = null;
    private const int RECV_LEN = 8192;
    private byte[] recv_buf = new byte[RECV_LEN];

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
    public static network _instance;
    public static network instance
    {
        get
        {
            return _instance;
        }
    }

    void Awake()
    {
        _instance = this;
        DontDestroyOnLoad(this.gameObject);
    }
    void Start()
    {
        this.connect_to_server();
    }

    void OnDestroy()
    {
        this.close_socket();
    }

    void OnApplicationQuit()
    {
        this.close_socket();
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

    void on_recv_tcp_cmd(byte[] data, int start, int data_len)
    {
        cmd_msg msg;
        proto_man.unpack_cmd_msg(data, start, data_len,out msg);
        if (msg != null)
        {
            lock (this.net_events)//recv thread
            {
                this.net_events.Enqueue(msg);
            }
            //gprotocol.LoginRes res = proto_man.protobuf_deserialize<gprotocol.LoginRes>(msg.body);
            //Debug.Log("----------------------------------------------------------------res = " + res.status);
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
            
            on_recv_tcp_cmd(pkg_data, raw_data_start, raw_data_len);
            
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
                this.client_socket.Disconnect(true);
                this.client_socket.Shutdown(SocketShutdown.Both);
                this.client_socket.Close();
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
            this.recv_thread.Abort();
        }
        if (this.client_socket != null && this.client_socket.Connected)
        {
            this.client_socket.Close();
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
}
