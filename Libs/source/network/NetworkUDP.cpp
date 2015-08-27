#include "network/NetworkUDP.h"
#include "network/NetworkRemote.h"
#include "network/IPv4Endpoint.h"

// NOTE
// http://www.ne.jp/asahi/hishidama/home/tech/socket/

namespace izanagi {
namespace net {
    Udp::Udp()
    {
        m_allocator = nullptr;

        m_socket = IZ_INVALID_SOCKET;

        m_isRunning.store(IZ_FALSE);
    }

    Udp::~Udp()
    {
        stop();
    }

    // サーバーとして起動.
    IZ_BOOL Udp::startAsServer(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp)
    {
        if (m_isRunning.load()) {
            return IZ_TRUE;
        }

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // ソケットの生成
        m_socket = socket(
            AF_INET,    // アドレスファミリ
            SOCK_DGRAM, // ソケットタイプ
            0);         // プロトコル
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

        m_host = hostEp;

        m_isRunning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    // クライアントとして起動.
    IZ_BOOL Udp::startAsClient(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp)
    {
        if (m_isRunning.load()) {
            return IZ_TRUE;
        }

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // ソケットの生成
        m_socket = socket(
            AF_INET,        // アドレスファミリ
            SOCK_DGRAM,    // ソケットタイプ
            0);             // プロトコル
        VRETURN(isValidSocket(m_socket));

        sockaddr_in inAddr;
        {
            FILL_ZERO(&inAddr, sizeof(inAddr));

            inAddr.sin_family = AF_INET;
            inAddr.sin_port = htons(hostEp.getPort());

            auto address = hostEp.getAddress();
            result = address.isAny();

            VGOTO(result, __EXIT__);

            IZ_CHAR ip[64];
            address.toString(ip, COUNTOF(ip));

            inAddr.sin_addr.S_un.S_addr = inet_addr(ip);
        }

        // ソケットにアドレスを結びつける
        result = (bind(m_socket, (const sockaddr*)&inAddr, sizeof(inAddr)) >= 0);
        VGOTO(result, __EXIT__);

        m_host = hostEp;

        m_isRunning.store(IZ_TRUE);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    IZ_BOOL Udp::startAsClientToServer(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& remoteEp)
    {
        if (m_isRunning.load()) {
            return IZ_TRUE;
        }

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        // ソケットの生成
        m_socket = socket(
            AF_INET,        // アドレスファミリ
            SOCK_DGRAM,     // ソケットタイプ
            IPPROTO_UDP);   // プロトコル
        VRETURN(isValidSocket(m_socket));

        // 通信ポート・アドレスの設定
        sockaddr_in destAddr;
        {
            memset(&destAddr, 0, sizeof(destAddr));

            destAddr.sin_family = AF_INET;
            destAddr.sin_port = htons(remoteEp.getPort());

            auto address = remoteEp.getAddress();
            VGOTO(!address.isAny(), __EXIT__);

            char ip[64];
            address.toString(ip, COUNTOF(ip));

            destAddr.sin_addr.S_un.S_addr = inet_addr(ip);
        }

        // 接続
        result = (connect(m_socket, (const sockaddr*)&destAddr, sizeof(destAddr)) >= 0);
        VGOTO(result, __EXIT__);

        // ホスト名取得
        sockaddr_in inAddr;
        int32_t inAddrSize = sizeof(inAddr);
        result = (getsockname(m_socket, (sockaddr*)&inAddr, &inAddrSize) >= 0);
        VGOTO(result, __EXIT__);

        m_host.set(inAddr);

        auto remote = UdpRemote::create(allocator);
        remote->m_endpoint = remoteEp;
        m_remotes.AddTail(remote->getListItem());

        m_isRunning.store(IZ_TRUE);

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
        m_isRunning.store(IZ_FALSE);

        onStop();

        if (isValidSocket(m_socket)) {
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        {
            auto item = m_remotes.GetTop();

            while (item != IZ_NULL) {
                auto data = item->GetData();
                item = item->GetNext();

                UdpRemote::deteteRemote(
                    m_allocator,
                    data);
            }

            m_remotes.Clear();
        }
    }

    // 受信したデータを取得.
    IZ_BOOL Udp::recieve(
        void* buf,
        IZ_UINT size)
    {
        IPv4Endpoint ep;

        IZ_INT ret = recieveFrom(buf, size, ep);

        return ret;
    }

    // 受信したデータを取得.
    IZ_INT Udp::recieveFrom(
        void* buf,
        IZ_UINT size,
        IPv4Endpoint& remoteEp)
    {
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

        auto remote = findRemote(addr);

        if (ret > 0) {
            if (!remote) {
                // リモートを追加
                remote = UdpRemote::create(m_allocator);
                remote->m_endpoint.set(addr);

                std::unique_lock<std::mutex> lock(m_remotesLocker);
                m_remotes.AddTail(remote->getListItem());
            }

            sys::Lock lock(*remote);

            remoteEp = remote->m_endpoint;
        }
        else {
            // TODO
            // 切断された

            if (remote) {
                // TODO
            }
        }

        return ret;
    }

    // データを送信.
    IZ_INT Udp::send(const void* data, IZ_UINT size)
    {
        IZ_INT ret = 0;

        auto item = m_remotes.GetAt(0);

        if (item != IZ_NULL) {
            auto remote = item->GetData();

            if (remote->isActive()) {
                sockaddr_in addr;
                remote->m_endpoint.get(addr);

                ret = sendto(
                    m_socket,
                    (const char*)data,
                    size,
                    0,
                    (sockaddr*)&addr,
                    sizeof(addr));

                if (ret < 0) {
                    // TODO
                    // 切断された
                }
            }
        }

        return ret;
    }

    // 指定した接続先にデータを送信.
    IZ_INT Udp::sendTo(
        IPv4Endpoint& endpoint,
        const void* data, IZ_UINT size)
    {        
        sockaddr_in addr;
        endpoint.get(addr);

        IZ_INT ret = sendto(
            m_socket,
            (const char*)data,
            size,
            0,
            (sockaddr*)&addr,
            sizeof(addr));

        if (ret < 0) {
            // TODO
            // 切断された

            auto remote = findRemote(endpoint);
            if (remote) {
                // TODO
            }
        }

        return ret;
    }

    UdpRemote* Udp::findRemote(const IPv4Endpoint& ep)
    {
        std::unique_lock<std::mutex> lock(m_remotesLocker);

        auto item = m_remotes.GetTop();

        while (item != IZ_NULL) {
            auto remote = item->GetData();

            if (remote->m_endpoint == ep) {
                return remote;
            }

            item = item->GetNext();
        }

        return nullptr;
    }

    UdpRemote* Udp::findRemote(const sockaddr_in& addr)
    {
        IPv4Endpoint ep;
        ep.set(addr);

        UdpRemote* ret = findRemote(ep);

        return ret;
    }

    IZ_BOOL Udp::isRunning()
    {
        return m_isRunning;
    }

    // NOTE
    // UDPではselectは不要
    // http://stackoverflow.com/questions/19758152/select-for-udp-connection

    IZ_INT Udp::wait()
    {
        IZ_ASSERT(m_isRunning.load());

        // TODO
        timeval t_val = { 0, 1000 };

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
            &t_val);

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
}    // namespace net
}    // namespace izanagi
