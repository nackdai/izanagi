#include <WinSock2.h>
#include <stdio.h>

#pragma comment( lib, "WS2_32.lib" )

// http://www.geekpage.jp/programming/linux-network/udp.php

int main(int argc, char** argv)
{
    WSADATA wsaData;

    WSAStartup(
        MAKEWORD(2,0),  // ���p����o�[�W�����ԍ�
        &wsaData);      // �������f�[�^�̕ۑ���

    SOCKET sock = socket(
        AF_INET,        // �A�h���X�t�@�~��
        SOCK_DGRAM,     // �\�P�b�g�^�C�v
        0);             // �v���g�R��
    if (sock == INVALID_SOCKET) {
        printf("socket failed\n");
        return 1;
    }

    int port = 8000;
    const char* ip = "127.0.0.1";

    sockaddr_in serverAddr;
    {
        // PF �� protocol family �̗��� INET �� "internetwork (UDP or TCP)" ���Ӗ����� (ws2def.h �Q��)
        serverAddr.sin_family = PF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
    }

    // ���M
    const char* buf = "test";
    
    sendto(
        sock,
        buf,
        strlen(buf),
        0,
        (const sockaddr*)&serverAddr,
        sizeof(serverAddr));


    // �\�P�b�g�����
    closesocket(sock);

    WSACleanup();

    return 0;
}
