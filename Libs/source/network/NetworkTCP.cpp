#include "network/NetworkTCP.h"
#include "network/NetworkRemote.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.jenkinssoftware.com/raknet/manual/systemoverview.html

namespace izanagi {
namespace net {
    Tcp::Tcp()
    {
    }

    Tcp::~Tcp()
    {
        stop();
    }

    // サーバーとして起動.
    IZ_BOOL Tcp::start(
        const IPv4Endpoint& hostEp,
        IZ_UINT maxConnections/*= 0*/)
    {
        if (isValidSocket(m_socket)) {
            return IZ_TRUE;
        }

        IZ_BOOL result = IZ_FALSE;

        // ソケットの生成
        m_socket = socket(
            AF_INET,        // アドレスファミリ
            SOCK_STREAM,    // ソケットタイプ
            IPPROTO_TCP);   // プロトコル
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
        VGOTO(result, __EXIT__);

        // コネクト要求をいくつまで待つかを設定
        if (maxConnections > 0) {
            result = (listen(m_socket, maxConnections) >= 0);
            VGOTO(result, __EXIT__);
        }

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    // 停止.
    void Tcp::stop()
    {
        if (isValidSocket(m_socket)) {
            shutdown(m_socket, SD_BOTH);
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }
    }

    ////////////////////////////////////////////////

    TcpClient::TcpClient()
    {
    }

    TcpClient::~TcpClient()
    {
    }

    // 起動.
    IZ_BOOL TcpClient::start(const IPv4Endpoint& hostEp)
    {
        auto ret = Tcp::start(hostEp);
        return ret;
    }

    IZ_BOOL TcpClient::connectTo(const IPv4Endpoint& remoteEp)
    {
        VRETURN(isValidSocket(m_socket));

        IZ_BOOL result = IZ_TRUE;

        // 通信ポート・アドレスの設定
        sockaddr_in serverAddr;
        {
            FILL_ZERO(&serverAddr, sizeof(serverAddr));

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(remoteEp.getPort());

            auto address = remoteEp.getAddress();
            result = !address.isAny();

            VGOTO(result, __EXIT__);

            IZ_CHAR ip[64];
            address.toString(ip, COUNTOF(ip));

            serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
        }

        result = (connect(
            m_socket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)) >= 0);

        if (result) {
            m_remote = remoteEp;
        }

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            stop();
        }

        return result;
    }

    IZ_INT TcpClient::recieve(
        void* buf,
        IZ_UINT size)
    {
        //IZ_ASSERT(!m_remote.getAddress().isAny());

        auto ret = recv(m_socket, (char*)buf, size, 0);
        return ret;
    }

    IZ_INT TcpClient::sendData(
        const void* data,
        IZ_UINT size)
    {
        //IZ_ASSERT(!m_remote.getAddress().isAny());

        auto ret = send(m_socket, (const char*)data, size, 0);
        return ret;
    }

    IZ_BOOL TcpClient::isActive()
    {
        return isValidSocket(m_socket);
    }

    ////////////////////////////////////////////////

    TcpListener::TcpListener()
    {
    }

    TcpListener::~TcpListener()
    {
    }

    IZ_BOOL TcpListener::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp,
        IZ_UINT maxConnections)
    {
        IZ_ASSERT(maxConnections > 0);
        auto ret = Tcp::start(hostEp, maxConnections);

        if (ret) {
            m_maxConnections = maxConnections;

            m_remotes.init(allocator, maxConnections);
        }

        return ret;
    }

    void TcpListener::stop()
    {
        Tcp::stop();

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpClient& remote = m_remotes.at(i);
            remote.stop();
        }

        m_remotes.clear();
    }

    TcpClient* TcpListener::acceptRemote()
    {
        sockaddr_in addr;
        IZ_INT addrlen = sizeof(addr);

        IZ_SOCKET socket = accept(m_socket, (sockaddr*)&addr, &addrlen);

        std::unique_lock<std::mutex> lock(m_remotesLocker);

        for (IZ_UINT i = 0; i < m_maxConnections; i++) {
            auto& remote = m_remotes.at(i);

            if (!remote.isActive()) {
                remote.m_socket = socket;
                return &remote;
            }
        }

        IZ_ASSERT(IZ_FALSE);
        return nullptr;
    }

    IZ_INT TcpListener::recieveFrom(
        void* buf,
        IZ_UINT size,
        const IPv4Endpoint& remoteEp)
    {
        auto remote = find(remoteEp);

        IZ_INT ret = 0;

        if (remote && remote->isActive()) {
            ret = recv(remote->m_socket, (char*)buf, size, 0);

            if (ret < 0) {
                // TODO
            }
        }

        return ret;
    }

    TcpClient* TcpListener::find(const IPv4Endpoint& remoteEp)
    {
        std::unique_lock<std::mutex> lock(m_remotesLocker);

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            auto& remote = m_remotes.at(i);

            if (remote.m_host == remoteEp) {
                return &remote;
            }
        }

        return nullptr;
    }

    // データを送信.
    IZ_BOOL TcpListener::sendTo(
        const void* data, 
        IZ_UINT size,
        const IPv4Endpoint& remoteEp)
    {
        auto remote = find(remoteEp);

        IZ_INT ret = 0;

        if (remote && remote->isActive()) {
            ret = send(remote->m_socket, (const char*)data, size, 0);

            if (ret < 0) {
                // TODO
            }
        }

        return ret;
    }

    // 全ての接続先にデータを送信.
    IZ_INT TcpListener::sendToAll(
        const void* data, 
        IZ_UINT size)
    {
        IZ_UINT succeededNum = 0;

        std::unique_lock<std::mutex> lock(m_remotesLocker);

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            auto& remote = m_remotes.at(i);

            if (remote.isActive()) {
                // TODO
                IZ_INT len = send(remote.m_socket, (const char*)data, size, 0);

                if (len > 0) {
                    succeededNum++;
                }
                else {
                    // TODO
                }
            }
        }

        return succeededNum;
    }

#if 0
    // NOTE
    // select(2)の第一引数にディスクリプタの最大値を渡すのは間違い？Add Star
    //  http://mattn.kaoriya.net/software/lang/c/20090114140035.htm
    // libevent
    //  http://www.ninxit.com/blog-data/src/libevent/main.cpp.html

    IZ_INT Tcp::wait(
        IZ_UINT sec/*= 0*/,
        IZ_UINT usec/*= 0*/)
    {
        VRETURN(m_isRunnning.load());

        timeval t_val = { sec, usec };

        timeval* pTval = nullptr;

        if (sec > 0 || usec > 0) {
            pTval = &t_val;
        }

        fd_set readFD;
        fd_set exceptionFD;
        fd_set writeFD;

        FD_ZERO(&readFD);
        FD_ZERO(&exceptionFD);
        FD_ZERO(&writeFD);

        FD_SET(m_socket, &readFD);
        FD_SET(m_socket, &exceptionFD);

        {
            std::unique_lock<std::mutex> lock(m_remotesLocker);

            for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
                TcpRemote& remote = m_remotes.at(i);

                sys::Lock lock(remote);

                if (remote.isActive()) {
                    FD_SET(remote.m_socket, &readFD);
                    FD_SET(remote.m_socket, &exceptionFD);

                    // 送信するデータがあるときだけ
                    if (remote.isRegistered()) {
                        FD_SET(remote.m_socket, &writeFD);
                    }
                }
            }
        }

        // ファイルディスクリプタ（ソケット）の状態遷移待ち
        auto resSelect = select(
            //FD_SETSIZE,
            0,
            &readFD,
            &writeFD,
            &exceptionFD,
            &t_val);

        if (resSelect <= 0) {
            return 0;
        }

        if (FD_ISSET(m_socket, &exceptionFD)) {
            return -1;
        }


        {
            std::unique_lock<std::mutex> lock(m_remotesLocker);

            for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
                TcpRemote& remote = m_remotes.at(i);

                sys::Lock locker(remote);

                if (FD_ISSET(remote.m_socket, &exceptionFD)) {
                    return -1;
                }
            }
        }

        return 1;
    }
#endif
}    // namespace net
}    // namespace izanagi
