#include "network/NetworkUDP.h"
#include "network/NetworkRemote.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.ne.jp/asahi/hishidama/home/tech/socket/

namespace izanagi {
namespace net {
    Udp::Udp()
    {
    }

    Udp::~Udp()
    {
        stop();
    }

    // 起動.
    IZ_BOOL Udp::start(const IPv4Endpoint& hostEp)
    {
        if (isValidSocket(m_socket)) {
            return IZ_TRUE;
        }

        IZ_BOOL result = IZ_FALSE;

        // ソケットの生成
        m_socket = socket(
            AF_INET,        // アドレスファミリ
            SOCK_DGRAM,     // ソケットタイプ
            IPPROTO_UDP);   // プロトコル
        VRETURN(isValidSocket(m_socket));

        // 通信ポート・アドレスの設定
        sockaddr_in inAddr;
        {
            FILL_ZERO(&inAddr, sizeof(inAddr));

            inAddr.sin_family = AF_INET;
            inAddr.sin_port = htons(hostEp.getPort());

            auto address = hostEp.getAddress();

            if (address.isAny()) {
                inAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
            }
            else {
                IZ_CHAR ip[64];
                address.toString(ip, COUNTOF(ip));

                inAddr.sin_addr.S_un.S_addr = inet_addr(ip);
            }
        }

        // ソケットにアドレスを結びつける
        result = (bind(m_socket, (const sockaddr*)&inAddr, sizeof(inAddr)) >= 0);
        
        if (result) {
            m_isBindAddr = IZ_TRUE;
            m_host = hostEp;
        }
        else {
            IZ_ASSERT(IZ_FALSE);
            stop();
        }

        return result;
    }

    // 起動.
    IZ_BOOL Udp::start()
    {
        if (isValidSocket(m_socket)) {
            return IZ_TRUE;
        }

        IZ_BOOL result = IZ_FALSE;

        // ソケットの生成
        m_socket = socket(
            AF_INET,        // アドレスファミリ
            SOCK_DGRAM,     // ソケットタイプ
            IPPROTO_UDP);   // プロトコル

        result = isValidSocket(m_socket);

        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    IZ_BOOL Udp::connectTo(const IPv4Endpoint& remoteEp)
    {
        VRETURN(isValidSocket(m_socket));

        IZ_BOOL result = IZ_FALSE;

        // 通信ポート・アドレスの設定
        sockaddr_in destAddr;
        {
            memset(&destAddr, 0, sizeof(destAddr));

            destAddr.sin_family = AF_INET;
            destAddr.sin_port = htons(remoteEp.getPort());

            auto address = remoteEp.getAddress();
            
            result = !address.isAny();
            VGOTO(result, __EXIT__);

            char ip[64];
            address.toString(ip, COUNTOF(ip));

            destAddr.sin_addr.S_un.S_addr = inet_addr(ip);
        }

        // 接続
        result = (connect(m_socket, (const sockaddr*)&destAddr, sizeof(destAddr)) >= 0);
        
        if (result) {
            if (!m_isBindAddr) {
                // ホスト名取得
                sockaddr_in inAddr;
                int32_t inAddrSize = sizeof(inAddr);
                result = (getsockname(m_socket, (sockaddr*)&inAddr, &inAddrSize) >= 0);
                VGOTO(result, __EXIT__);

                m_isBindAddr = IZ_TRUE;
                m_host.set(inAddr);
            }
        }

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    // 停止.
    void Udp::stop()
    {
        endRecieve();

        if (isValidSocket(m_socket)) {
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }
    }

    // 受信したデータを取得.
    IZ_BOOL Udp::recieve(
        void* buf,
        IZ_UINT size)
    {
        IZ_ASSERT(isValidSocket(m_socket));
        IZ_INT ret = recv(m_socket, (char*)buf, size, 0);
        return ret;
    }

    // 受信したデータを取得.
    IZ_INT Udp::recieveFrom(
        void* buf,
        IZ_UINT size,
        IPv4Endpoint& remoteEp)
    {
        IZ_ASSERT(isValidSocket(m_socket));

        sockaddr_in addr;
        sockaddr* paddr = (sockaddr*)&addr;
        IZ_INT len = sizeof(addr);

        IZ_INT ret = recvfrom(
            m_socket,
            (char*)buf,
            size,
            0,
            paddr,
            &len);

        if (ret > 0) {
            remoteEp.set(addr);
        }
        else {
            auto err = getLastError();
            IZ_PRINTF("Udp Recv Err(%d)\n", err);
        }

        return ret;
    }

    // データを送信.
    IZ_INT Udp::sendData(const void* data, IZ_UINT size)
    {
        IZ_INT ret = send(m_socket, (const char*)data, size, 0);
        return ret;
    }

    // 指定した接続先にデータを送信.
    IZ_INT Udp::sendTo(
        const void* data,
        IZ_UINT size,
        IPv4Endpoint& remoteEp)
    {
        IZ_ASSERT(isValidSocket(m_socket));
        VRETURN_VAL(remoteEp != m_host, 0);

        sockaddr_in addr;
        remoteEp.get(addr);

        IZ_INT ret = sendto(
            m_socket,
            (const char*)data,
            size,
            0,
            (sockaddr*)&addr,
            sizeof(addr));

        return ret;
    }

    // NOTE
    // UDPではselectは不要
    // http://stackoverflow.com/questions/19758152/select-for-udp-connection

    IZ_INT Udp::wait(
        IZ_UINT sec/*= 0*/,
        IZ_UINT usec/*= 0*/)
    {
        IZ_ASSERT(isValidSocket(m_socket));

        timeval t_val = { sec, usec };

        timeval* pTval = nullptr;

        if (sec > 0 || usec > 0) {
            pTval = &t_val;
        }

        fd_set readFD;
        fd_set exceptionFD;

        FD_ZERO(&readFD);
        FD_ZERO(&exceptionFD);

        FD_SET(m_socket, &readFD);
        FD_SET(m_socket, &exceptionFD);

        // ファイルディスクリプタ（ソケット）の状態遷移待ち
        auto resSelect = select(
            //FD_SETSIZE,
            0,
            &readFD,
            NULL,
            &exceptionFD,
            pTval);

        if (resSelect <= 0) {
            return 0;
        }

        // 受信.
        if (FD_ISSET(m_socket, &readFD)) {
            return 1;
        }

        if (FD_ISSET(m_socket, &exceptionFD)) {
            // TODO
            IZ_ASSERT(IZ_FALSE);
            return -1;
        }

        return 0;
    }

    void Udp::beginRecieve(
        std::function<void(const Packet*)> onRecieve,
        void* buf,
        IZ_UINT size)
    {
        IZ_ASSERT(onRecieve && buf);

        m_isRunning = IZ_TRUE;

        m_recvThread = std::thread(
            [this, buf, size, onRecieve]{
            while (m_isRunning) {
                // TODO
                if (wait(0, 1000) > 0) {
                    Packet packet;
                    IZ_INT len = recieveFrom(buf, size, packet.endpoint);

                    if (len > 0) {
                        packet.data = (IZ_UINT8*)buf;
                        packet.size = len;
                        onRecieve(&packet);
                    }
                    else {
                        onRecieve(nullptr);
                    }
                }
                sys::CThread::YieldThread();
            }
        });
    }

    void Udp::endRecieve()
    {
        m_isRunning = IZ_FALSE;

        if (m_recvThread.joinable()) {
            m_recvThread.join();
        }
    }
}    // namespace net
}    // namespace izanagi
