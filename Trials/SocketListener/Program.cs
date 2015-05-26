using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

// http://sabulinsprog.seesaa.net/article/130925253.html

namespace SocketListener
{
    class Program
    {
        static void Main(string[] args)
        {
            var listener = new Socket(
                AddressFamily.InterNetwork,
                SocketType.Stream,
                ProtocolType.Tcp);

            var ip = IPAddress.Parse("127.0.0.1");
            var ep = new IPEndPoint(ip, 8000);

            listener.Bind(ep);
            listener.Listen(1);

            for (; ; )
            {
                Console.WriteLine("接続待機中...");
                Socket connection = listener.Accept();

                // Accepted
                Console.WriteLine("Accepted!!");
                IPEndPoint remoteEP = (IPEndPoint)connection.RemoteEndPoint;
                Console.WriteLine("Remote IP = {0}", remoteEP.Address);
                Console.WriteLine("Remote Port = {0}", remoteEP.Port);

                // Receive
                byte[] receiveData = new byte[1000];
                int size = connection.Receive(receiveData);
                string receiveString = Encoding.UTF8.GetString(receiveData, 0, size);
                Console.WriteLine("受信完了!");
                Console.WriteLine("受信文字列 = {0}\n", receiveString);


            }
        }
    }
}
