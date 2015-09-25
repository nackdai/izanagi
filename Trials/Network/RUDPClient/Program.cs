using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace RUDPClient
{
    class Program
    {
        static void Main(string[] args)
        {
            var addr = IPAddress.Parse("127.0.0.1");
            var remoteEp = new IPEndPoint(addr, 8000);

            var udp = new UdpClient(new IPEndPoint(addr, 8100));

            var client = new RUDP.ReliableUDP(udp, new RUDP.Parameter());

            client.Connect(remoteEp);

            Console.WriteLine("Connected");

            byte[] bytes = new byte[1024];

            var len = client.Recieve(ref bytes);

            var returnData = Encoding.UTF8.GetString(bytes, 0, len);

            Console.WriteLine("Recieved {0}bytes", len);
            Console.WriteLine("[{0}]", returnData);

            var sendData = System.Text.Encoding.UTF8.GetBytes("Send From Client");

            len = client.Send(sendData, 0, sendData.Length);

            Console.WriteLine("Send {0}bytes", len);

            sendData = System.Text.Encoding.UTF8.GetBytes("Send From Client -- 2");

            len = client.Send(sendData, 0, sendData.Length);

            Console.WriteLine("Send {0}bytes", len);
        }
    }
}
