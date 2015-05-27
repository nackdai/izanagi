using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace UdpServer
{
    class Program
    {
        static void Main(string[] args)
        {
            var localAddr = IPAddress.Parse("127.0.0.1");
            int port = 8000;

            var ep = new IPEndPoint(localAddr, port);

            var udp = new UdpClient(ep);

            for (; ; )
            {

            }
        }
    }
}
