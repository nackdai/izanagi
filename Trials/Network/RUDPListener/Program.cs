using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RUDPListener
{
    class Program
    {
        static void Main(string[] args)
        {
            var udp = new UdpClient(new IPEndPoint(IPAddress.Any, 8000));

            var listener = new RUDP.ReliableUDPListener(udp);

            var client = listener.Accept();

            Console.WriteLine("Accepted");

            var bytes = System.Text.Encoding.UTF8.GetBytes("Send From Listener");

            var len = client.Send(bytes, 0, bytes.Length);

            Console.WriteLine("Send {0}bytes", len);

            byte[] recvData = new byte[1024];

            len = client.Recieve(ref recvData);

            var returnData = Encoding.UTF8.GetString(recvData, 0, len);

            Console.WriteLine("Recieved {0}bytes", len);
            Console.WriteLine("[{0}]", returnData);

            len = client.Recieve(ref recvData);

            returnData = Encoding.UTF8.GetString(recvData, 0, len);

            Console.WriteLine("Recieved {0}bytes", len);
            Console.WriteLine("[{0}]", returnData);
        }
    }
}
