using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RUDP
{
    public class ReliableUDPListener : ReliableUDP
    {
        AutoResetEvent AcceptWaitEvent = new AutoResetEvent(false);

        /// <summary>
        /// コネクションされたクライアントのリスト.
        /// </summary>
        SafeList<ReliableUDP> AcceptedClientList = new SafeList<ReliableUDP>();

        /// <summary>
        /// 受信したクライアントのリスト.
        /// </summary>
        Dictionary<IPEndPoint, ReliableUDPClient> RecievedClientMap = new Dictionary<IPEndPoint, ReliableUDPClient>();

        Thread RecieveThread;

        public ReliableUDPListener(UdpClient udp)
        {
            this.Udp = udp;

            this.RecieveThread = new Thread(ProcRecieve);
            this.RecieveThread.Start();
        }

        public ReliableUDP Accept()
        {
            if (this.IsClosed)
            {
                //throw new 
            }

            if (this.AcceptedClientList.Count == 0)
            {
                this.AcceptWaitEvent.WaitOne();
            }

            var ret = this.AcceptedClientList[this.AcceptedClientList.Count - 1];
            this.AcceptedClientList.Remove(ret);

            return ret;
        }

        private void ProcRecieve()
        {
            while (!this.WillClose)
            {
                IPEndPoint remoteEp = new IPEndPoint(IPAddress.Any, 0);
                var segment = base.OnRecieveSegment(ref remoteEp);

                if (segment != null)
                {
                    ReliableUDPClient client = null;

                    lock (this.RecievedClientMap)
                    {
                        if (!this.IsClosed)
                        {
                            if (segment is SynchronousSegment)
                            {
                                // まずは登録済みかどうか探す.
                                if (!this.RecievedClientMap.ContainsKey(remoteEp))
                                {
                                    // 未登録の場合は登録する.

                                    client = ReliableUDPClient.Create(this, remoteEp);
                                    this.RecievedClientMap.Add(remoteEp, client);
                                }
                            }
                        }

                        if (client == null)
                        {
                            client = this.RecievedClientMap[remoteEp];
                        }

                        if (client != null)
                        {
                            client.EnqueueSegment(segment);
                        }
                    }
                }
            }
        }

        class ReliableUDPClient : ReliableUDP
        {
            IPEndPoint RemoteEp;

            ReliableUDPListener Parent;

            SafeQueue<Segment> RecievedSegmentQueue = new SafeQueue<Segment>();

            AutoResetEvent QueueWaitEvent = new AutoResetEvent(false);

            static public ReliableUDPClient Create(
                ReliableUDPListener server,
                IPEndPoint ep)
            {
                var ret = new ReliableUDPClient(server, ep);
                return ret;
            }

            private ReliableUDPClient(ReliableUDPListener server, IPEndPoint ep)
                : base(server.Udp, new Parameter())
            {
                this.RemoteEp = ep;
                this.Parent = server;
            }

            public void EnqueueSegment(Segment segment)
            {
                if (!this.IsClosed)
                {
                    this.RecievedSegmentQueue.Enqueue(segment);
                    this.QueueWaitEvent.Set();
                }
            }

            protected override void OnOpenConnection()
            {
#if false
                // 一定サイズを超えていたいら
                if (this.Parent.AcceptedClientList.Count > MAX)
                {
                    // Wait
                }
#endif

                this.Parent.AcceptedClientList.Add(this);

                this.Parent.AcceptWaitEvent.Set();
            }

            protected override Segment OnRecieveSegment()
            {
                if (!this.IsClosed)
                {
                    if (this.RecievedSegmentQueue.Count == 0)
                    {
                        this.QueueWaitEvent.WaitOne();
                    }

                    this.QueueWaitEvent.Reset();

                    return this.RecievedSegmentQueue.Dequeue();
                }

                return null;
            }

            protected override void OnSendSegment(Segment segment)
            {
                Console.WriteLine("Send Segment [{0}]", segment.ToString());

                using (var ws = new MemoryStream())
                {
                    segment.WriteBytes(ws);
                    var bytes = ws.GetBuffer();
                    var length = ws.Position;
                    this.Udp.Send(bytes, (int)length, this.RemoteEp);
                }
            }

            // TODO
            // close
        }
    }
}
