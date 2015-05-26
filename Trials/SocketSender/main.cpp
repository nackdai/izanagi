// http://www.geekpage.jp/programming/winsock/
// http://www.geocities.jp/kandou_quester/Knowledge/Programming/Win32/_minimum_socket.html
// http://yonex1.cis.ibaraki.ac.jp/~yonekura/2002kadai/lecture03.html

#include <WinSock2.h>
#include <stdio.h>

#pragma comment( lib, "WS2_32.lib" )

int main(int argc, char** argv)
{
    WSADATA wsaData;

    WSAStartup(
        MAKEWORD(2,0),  // 利用するバージョン番号
        &wsaData);      // 初期化データの保存先

    SOCKET sock = socket(
        AF_INET,        // アドレスファミリ
        SOCK_STREAM,    // ソケットタイプ
        0);             // プロトコル
    if (sock == INVALID_SOCKET) {
        printf("socket failed\n");
        return 1;
    }

    int port = 8000;
    const char* ip = "127.0.0.1";

    sockaddr_in serverAddr;
    {
        // PF は protocol family の略で INET は "internetwork (UDP or TCP)" を意味する (ws2def.h 参照)
        serverAddr.sin_family = PF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
    }

	// 相手に接続
    connect(
        sock,   // 受け入れるソケット
        (sockaddr*)&serverAddr,
        sizeof(serverAddr));

    // 送信
    const char* buf = "test";
    send(
        sock,
        buf,
        strlen(buf),
        0);

    // ソケットを閉じる
    closesocket(sock);

    WSACleanup();

    return 0;
}
