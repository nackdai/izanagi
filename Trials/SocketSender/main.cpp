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
        MAKEWORD(2,0),  // ���p����o�[�W�����ԍ�
        &wsaData);      // �������f�[�^�̕ۑ���

    SOCKET sock = socket(
        AF_INET,        // �A�h���X�t�@�~��
        SOCK_STREAM,    // �\�P�b�g�^�C�v
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

	// ����ɐڑ�
    connect(
        sock,   // �󂯓����\�P�b�g
        (sockaddr*)&serverAddr,
        sizeof(serverAddr));

    // ���M
    const char* buf = "test";
    send(
        sock,
        buf,
        strlen(buf),
        0);

    // �\�P�b�g�����
    closesocket(sock);

    WSACleanup();

    return 0;
}
