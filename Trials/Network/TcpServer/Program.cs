using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

// http://dobon.net/vb/dotnet/internet/tcpclientserver.html

namespace TcpServer
{
    class Program
    {
        static void Main(string[] args)
        {
            var ip = IPAddress.Parse("127.0.0.1");

            var listener = new TcpListener(ip, 8000);

            listener.Start();
            Console.WriteLine(
                "Listenを開始しました({0}:{1})。",
               ((IPEndPoint)listener.LocalEndpoint).Address,
               ((IPEndPoint)listener.LocalEndpoint).Port);

            for (; ; )
            {
                var connection = listener.AcceptTcpClient();

                //NetworkStreamを取得
                var ns = connection.GetStream();

                //読み取り、書き込みのタイムアウトを10秒にする
                //デフォルトはInfiniteで、タイムアウトしない
                //(.NET Framework 2.0以上が必要)
                ns.ReadTimeout = 10000;
                ns.WriteTimeout = 10000;

                byte[] receiveData = new byte[1000];
                int size = ns.Read(receiveData, 0, receiveData.Length);
                string receiveString = Encoding.UTF8.GetString(receiveData, 0, size);
                Console.WriteLine("受信完了!");
                Console.WriteLine("受信文字列 = {0}\n", receiveString);
            }
        }
    }
}
