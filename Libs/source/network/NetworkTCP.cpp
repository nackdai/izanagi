#include "network/NetworkTCP.h"
#include "network/NetworkRemote.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.jenkinssoftware.com/raknet/manual/systemoverview.html

namespace izanagi {
namespace net {
    Tcp::Packet* Tcp::Packet::create(IMemoryAllocator* allocator, IZ_UINT len)
    {
        // 最後に０を入れるため１多くする
        auto alignedLen = len + 1;

        // TODO
        // ４バイトアラインしておく
        auto rest = alignedLen & 0x03;
        if (rest > 0) {
            rest = 4 - rest;
            alignedLen += rest;
        }

        size_t size = sizeof(Packet) + alignedLen;

        IZ_CHAR* buf = (IZ_CHAR*)ALLOC(allocator, size);
        IZ_ASSERT(buf != nullptr);

        Packet* p = new(buf)Packet;
        buf += sizeof(Packet);

        p->data = buf;

        p->size = len;
        p->data[len] = 0;

        return p;
    }

    ///////////////////////////////////////////////////////////

    Tcp::Tcp()
    {
        m_allocator = nullptr;

        m_socket = IZ_INVALID_SOCKET;

        m_isRunnning.store(IZ_FALSE);
    }

    Tcp::~Tcp()
    {
        stop();
    }

    // サーバーとして起動.
    IZ_BOOL Tcp::startAsServer(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint,
        IZ_UINT maxConnections)
    {
        VRETURN(maxConnections > 0);

        if (m_isRunnning.load()) {
            return IZ_TRUE;
        }

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // ソケットの生成
        m_socket = socket(
            AF_INET,        // アドレスファミリ
            SOCK_STREAM,    // ソケットタイプ
            0);             // プロトコル
        VRETURN(isValidSocket(m_socket));

        // 通信ポート・アドレスの設定
        sockaddr_in inAddr;
        {
            FILL_ZERO(&inAddr, sizeof(inAddr));

            inAddr.sin_family = AF_INET;
            inAddr.sin_port = htons(endpoint.getPort());

            auto address = endpoint.getAddress();

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
        result = (listen(m_socket, maxConnections) >= 0);
        VGOTO(result, __EXIT__);

        result = m_remotes.init(m_allocator, maxConnections);
        VGOTO(result, __EXIT__);

        m_isRunnning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

#if 0
            if (isValidSocket(m_socket)) {
                closesocket(m_socket);
            }

            m_remotes.clear();
#else
            stop();
#endif
        }

        return result;
    }

    // クライアントとして起動.
    IZ_BOOL Tcp::startAsClient(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint)
    {
        if (m_isRunnning.load()) {
            return IZ_TRUE;
        }

        auto address = endpoint.getAddress();

        VRETURN(!address.isAny());

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // ソケットの生成
        m_socket = socket(
            AF_INET,        // アドレスファミリ
            SOCK_STREAM,    // ソケットタイプ
            0);             // プロトコル
        VRETURN(isValidSocket(m_socket));

        // サーバー１つのみ
        result = m_remotes.init(m_allocator, 1);
        VGOTO(result, __EXIT__);

        m_remotes.at(0).m_endpoint = endpoint;

        m_isRunnning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

#if 0
            if (isValidSocket(m_socket)) {
                closesocket(m_socket);
            }

            m_remotes.clear();
#else
            stop();
#endif
        }
        return result;
    }

    // 停止.
    void Tcp::stop()
    {
        m_isRunnning.store(IZ_FALSE);

        onStop();

        if (isValidSocket(m_socket)) {
            shutdown(m_socket, SD_BOTH);
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        m_remotes.traverse(
            [this] (TcpRemote& remote) {
            remote.close();
        });

        m_remotes.clear();

        auto item = m_recvData.GetTop();

        while (item != IZ_NULL) {
            auto packet = item->GetData();
            auto next = item->GetNext();

            item->Leave();
            FREE(m_allocator, packet);

            item = next;
        }

        m_recvData.Clear();
    }

    IZ_UINT Tcp::getRemoteNum() const
    {
        IZ_UINT ret = m_remotes.getNum();
        return ret;
    }

    const IPv4Endpoint* Tcp::getRemote(IZ_UINT idx) const
    {
        const TcpRemote& remote = m_remotes.at(idx);
        
        if (!remote.isActive()) {
            return IZ_NULL;
        }

        return &remote.m_endpoint;
    }

    IZ_BOOL Tcp::recieve(std::function<void(const net::Packet&)> func)
    {
        Packet* src = nullptr;

        {
            std::unique_lock<std::mutex> lock(m_recvDataLocker);

            auto item = m_recvData.GetTop();
            if (item == IZ_NULL) {
                return IZ_FALSE;
            }

            item->Leave();
            src = item->GetData();
        }

        func(*src);

        FREE(m_allocator, src);

        return IZ_TRUE;
    }

    IZ_BOOL Tcp::recieveAll(std::function<void(const net::Packet&)> func)
    {
        std::unique_lock<std::mutex> lock(m_recvDataLocker);

        auto item = m_recvData.GetTop();
        if (item == IZ_NULL) {
            return IZ_FALSE;
        }

        while (item != IZ_NULL) {
            auto next = item->GetNext();

            auto src = item->GetData();
            item->Leave();

            func(*src);

            FREE(m_allocator, src);

            item = next;
        }

        return IZ_TRUE;
    }

    IZ_BOOL Tcp::sendData(const void* data, IZ_UINT size)
    {
        Remote& remote = m_remotes.at(0);
        return remote.registerData(m_allocator, 1, &data, &size);
    }

    IZ_BOOL Tcp::sendData(
        const IPv4Endpoint& endpoint,
        const void* data, IZ_UINT size)
    {
        Remote* remote = nullptr;

        // 対象となるリモート情報を探す
        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpRemote& r = m_remotes.at(i);

            if (r.isActive() && r.m_endpoint == endpoint) {
                remote = &r;
                break;
            }
        }

        VRETURN(remote != nullptr);

        // TODO
        IZ_BOOL result = remote->registerData(
            m_allocator,
            1,
            &data, &size);
        VRETURN(result);

        return result;
    }

    IZ_UINT Tcp::sendDataToAllRemote(const void* data, IZ_UINT size)
    {
        IZ_UINT succeededNum = 0;;

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpRemote& remote = m_remotes.at(i);

            if (remote.isActive()) {
                // TODO
                IZ_BOOL result = remote.registerData(
                    m_allocator,
                    1,
                    &data, &size);

                succeededNum = (result ? succeededNum + 1 : succeededNum);
            }
        }

        return succeededNum;
    }

    IZ_BOOL Tcp::connectServer()
    {
        VRETURN(m_isRunnning.load());

        const IPv4Endpoint& endpoint = m_remotes.at(0).m_endpoint;

        // 通信ポート・アドレスの設定
        sockaddr_in serverAddr;
        {
            FILL_ZERO(&serverAddr, sizeof(serverAddr));

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(endpoint.getPort());

            auto address = endpoint.getAddress();

            IZ_CHAR ip[64];
            address.toString(ip, COUNTOF(ip));

            serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
        }

        IZ_BOOL result = (connect(
            m_socket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)) >= 0);

        if (result) {
            TcpRemote& remote = m_remotes.at(0);

            sys::Lock lock(remote);
            remote.setSocket(m_socket);
        }
        else {
            // TODO
            IZ_ASSERT(IZ_FALSE);
        }

        return result;
    }

    // NOTE
    // select(2)の第一引数にディスクリプタの最大値を渡すのは間違い？Add Star
    //  http://mattn.kaoriya.net/software/lang/c/20090114140035.htm
    // libevent
    //  http://www.ninxit.com/blog-data/src/libevent/main.cpp.html

    IZ_BOOL Tcp::run(IZ_CHAR* recvBuf, IZ_UINT size)
    {
        VRETURN(m_isRunnning.load());

        // TODO
        timeval t_val = { 0, 1000 };

        fd_set readFD;
        fd_set exceptionFD;
        fd_set writeFD;

        FD_ZERO(&readFD);
        FD_ZERO(&exceptionFD);
        FD_ZERO(&writeFD);

        FD_SET(m_socket, &readFD);
        FD_SET(m_socket, &exceptionFD);

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpRemote& remote = m_remotes.at(i);

            sys::Lock lock(remote);

            if (remote.isActive()) {
                FD_SET(remote.m_socket, &readFD);
                FD_SET(remote.m_socket, &writeFD);
                FD_SET(remote.m_socket, &exceptionFD);
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
            return IZ_FALSE;
        }

        if (FD_ISSET(m_socket, &readFD)) {
            sockaddr_in addr;
            sockaddr* paddr = (sockaddr*)&addr;
            IZ_INT len = sizeof(addr);

            // クライアントからの接続待ち
            auto socket = accept(m_socket, paddr, &len);

            if (!isValidSocket(socket)) {
                return IZ_FALSE;
            }

            for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
                TcpRemote& remote = m_remotes.at(i);

                sys::Lock lock(remote);

                // 空いているものを探す
                if (!remote.isActive()) {
                    remote.setSocket(socket);
                    remote.m_endpoint.set(addr);
                    break;
                }
            }
        }
        else if (FD_ISSET(m_socket, &exceptionFD)) {
            // TODO
        }

        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpRemote& remote = m_remotes.at(i);

            if (FD_ISSET(remote.m_socket, &exceptionFD)) {
                // TODO
            }
            else {
                // 受信
                if (FD_ISSET(remote.m_socket, &readFD)) {
                    sys::Lock lock(remote);

                    auto len = remote.recieveData(recvBuf, size);

                    if (len > 0) {
                        auto packet = Packet::create(m_allocator, len);
                        packet->endpoint = remote.m_endpoint;
                        memcpy(packet->data, recvBuf, len);

                        std::unique_lock<std::mutex> lock(m_recvDataLocker);
                        m_recvData.AddTail(&packet->listItem);
                    }
                    else {
                        // 切断された
                        remote.reset();
                    }
                }

                // 送信
                if (FD_ISSET(remote.m_socket, &writeFD)) {
                    sys::Lock lock(remote);

                    auto len = remote.sendData();

                    if (len < 0) {
                        // 切断された
                        remote.reset();
                    }
                }
            }
        }

        return IZ_TRUE;
    }
    
    void Tcp::loop()
    {
        // TODO
        IZ_UINT size = 1 * 1024 * 1024;
        IZ_CHAR* recvBuf = (IZ_CHAR*)ALLOC_ZERO(m_allocator, size);

        if (recvBuf == nullptr) {
            IZ_ASSERT(IZ_FALSE);
            return;
        }

        for (;;) {
            if (!m_isRunnning.load()) {
                break;
            }

            run(recvBuf, size);
        }

        FREE(m_allocator, recvBuf);
    }
}    // namespace net
}    // namespace izanagi
