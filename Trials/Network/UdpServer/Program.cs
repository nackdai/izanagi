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

            try
            {
                for (; ; )
                {
                    var remoteEP = new IPEndPoint(IPAddress.Any, 0);

                    var recBuf = udp.Receive(ref remoteEP);

                    var returnData = Encoding.UTF8.GetString(recBuf, 0, recBuf.Length);

                    Console.WriteLine("受信: {0}Bytes {1}", recBuf.Length, returnData);
                    Console.WriteLine(
                        "送信元 IP={0}, PORT={1}",
                        remoteEP.Address.ToString(),
                        remoteEP.Port);
#if false
                    returnData = returnData.ToUpper();

                    byte[] sendBytes = System.Text.Encoding.UTF8.GetBytes(returnData);

                    udp.Send(sendBytes, sendBytes.Length, remoteEP);
#endif
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

            udp.Close();
        }
    }
}
