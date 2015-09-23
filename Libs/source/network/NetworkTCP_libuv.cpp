#include "network/NetworkTCP_libuv.h"
#include "network/IPv4Endpoint.h"
#include "network\/Network.h"

// NOTE
// http://www.jenkinssoftware.com/raknet/manual/systemoverview.html

namespace izanagi {
namespace net {

#define IZ_LIBUV_EXEC(result, func)\
    {\
        int err = func;\
        result = (err == 0);\
        if (!result) { IZ_PRINTF("%s\n", uv_err_name(err)); }\
    }

    ////////////////////////////////////////////////

    TcpClient::TcpClient()
    {
    }

    TcpClient::~TcpClient()
    {
        IZ_ASSERT(!m_isConnected);
    }

    // 起動.
    IZ_BOOL TcpClient::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp)
    {
        IZ_BOOL result = IZ_FALSE;

        m_allocator = allocator;

        IZ_LIBUV_EXEC(result, uv_tcp_init(uv_default_loop(), &m_client));
        VRETURN(result);

        // 通信ポート・アドレスの設定.
        sockaddr_in inAddr;
        {
#if 0
            FILL_ZERO(&inAddr, sizeof(inAddr));

            inAddr.sin_family = AF_INET;
            inAddr.sin_port = htons(hostEp.getPort());

            auto address = hostEp.getAddress();

            if (address.isAny()) {
                setIp(inAddr, htonl(INADDR_ANY));
            }
            else {
                IZ_CHAR ip[64];
                address.toString(ip, COUNTOF(ip));

                setIp(inAddr, inet_addr(ip));
            }
#else
            auto address = hostEp.getAddress();
            auto port = hostEp.getPort();

            if (address.isAny()) {
                uv_ip4_addr("0,0,0,0", port, &inAddr);
            }
            else {
                IZ_CHAR ip[64];
                address.toString(ip, COUNTOF(ip));

                uv_ip4_addr(ip, port, &inAddr);
            }
#endif
        }

        // ソケットにアドレスを結びつける.
        IZ_LIBUV_EXEC(result, uv_tcp_bind(&m_client, (const sockaddr*)&inAddr, 0));
        VGOTO(result, __EXIT__);

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop(nullptr);
        }

        return result;
    }

    IZ_BOOL TcpClient::connectTo(
        const IPv4Endpoint& remoteEp,
        std::function<void(IZ_BOOL)> onConnected)
    {
        if (IsConnected()) {
            return IZ_FALSE;
        }

        if (IsConnecting()) {
            return IZ_FALSE;
        }

        auto onConnect = std::bind(&TcpClient::OnConnect, this, std::placeholders::_1, std::placeholders::_2);

        IZ_BOOL result = IZ_TRUE;

        // 通信ポート・アドレスの設定.
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

            setIp(serverAddr, inet_addr(ip));
        }

        m_isConnecting = IZ_TRUE;

        m_onConnected = onConnected;

        IZ_LIBUV_EXEC(
            result,
            uv_tcp_connect(&m_reqConnect, &m_client, (const sockaddr*)&serverAddr, onConnect));

        if (result) {
            m_remote = remoteEp;

            Network::start();
        }

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            stop(nullptr);
        }

        return result;
    }

    void TcpClient::OnConnect(uv_connect_t *req, int status)
    {
        m_isConnecting = IZ_FALSE;

        if (status == -1) {
            // TODO
            if (m_onConnected) {
                m_onConnected(IZ_FALSE);
            }
            IZ_ASSERT(IZ_FALSE);
            return;
        }

        m_handle = req->handle;
        m_isConnected = IZ_TRUE;

        if (m_onConnected) {
            m_onConnected(IZ_TRUE);
        }
    }

    void TcpClient::stop(std::function<void(void)> onClosed)
    {
        if (m_isConnecting) {
            uv_cancel((uv_req_t*)&m_reqConnect);
            m_isConnecting = IZ_FALSE;
        }

        if (m_isWriting) {
            uv_cancel((uv_req_t*)&m_reqWrite);
            m_isWriting = IZ_FALSE;
        }

        if (m_isReading) {
            uv_read_stop(m_handle);
            m_isReading = IZ_FALSE;
        }

        FREE(m_allocator, m_recvBuf);
        m_recvBuf = nullptr;

        uv_close(
            (uv_handle_t*)m_handle,
            [&](uv_handle_t* handle) {
            if (onClosed) {
                onClosed();
            }
            m_isConnected = IZ_FALSE;
        });
    }

    IZ_BOOL TcpClient::recieve(
        std::function<void(void*, IZ_UINT)> onRecieved)
    {
        // 接続チェック.
        if (IsConnected()) {
            return IZ_FALSE;
        }

        if (IsActing()) {
            return IZ_FALSE;
        }

        m_isReading = IZ_TRUE;

        auto onAlloc = std::bind(
            &TcpClient::OnAlloc,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3);

        auto onReadEnd = std::bind(
            &TcpClient::OnReadEnd,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3);

        IZ_BOOL result = IZ_FALSE;

        IZ_LIBUV_EXEC(
            result,
            uv_read_start(m_handle, onAlloc, onReadEnd));
        IZ_ASSERT(result);

        return result;
    }

    void TcpClient::OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
    {
        m_recvBuf = ALLOC(m_allocator, suggested_size);

        auto uv_buf = uv_buf_init((char*)m_recvBuf, suggested_size);

        memcpy(buf, &uv_buf, sizeof(uv_buf));
    }

    void TcpClient::OnReadEnd(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
    {
        m_isReading = IZ_FALSE;

        if (nread == -1) {
            // TODO
            IZ_ASSERT(IZ_FALSE);
        }

        if (m_onRecieved) {
            m_onRecieved(buf->base, nread);
        }

        FREE(m_allocator, m_recvBuf);
        m_recvBuf = nullptr;
    }

    IZ_BOOL TcpClient::sendData(
        const void* data,
        IZ_UINT size,
        std::function<void(IZ_BOOL)> onSent)
    {
        // 接続チェック.
        if (IsConnected()) {
            return IZ_FALSE;
        }

        if (IsActing()) {
            return IZ_FALSE;
        }

        uv_buf_t tmp[1];

        uv_buf_t buf = uv_buf_init((char*)tmp, sizeof(tmp));

        m_isWriting = IZ_TRUE;

        IZ_BOOL result = IZ_FALSE;

        auto onWirteEnd = std::bind(&TcpClient::OnWriteEnd, this, std::placeholders::_1, std::placeholders::_2);

        // 書き込み
        IZ_LIBUV_EXEC(
            result,
            uv_write(&m_reqWrite, m_handle, &buf, 1, onWirteEnd));

        if (result) {
            m_onSent = onSent;
        }

        return result;
    }

    void TcpClient::OnWriteEnd(uv_write_t *req, int status)
    {
        m_isWriting = IZ_FALSE;

        if (status == -1) {
            // TODO
            if (m_onSent) {
                m_onSent(IZ_FALSE);
            }
            IZ_ASSERT(IZ_FALSE);
            return;
        }

        if (m_onSent) {
            m_onSent(IZ_TRUE);
        }
    }

    ////////////////////////////////////////////////

    TcpListener::TcpListener()
    {
    }

    TcpListener::~TcpListener()
    {
        IZ_ASSERT(!m_isListening);
    }

    IZ_BOOL TcpListener::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp,
        IZ_UINT maxConnections)
    {
        // 接続済みチェック.
        if (IsListening()) {
            return IZ_TRUE;
        }

        IZ_ASSERT(maxConnections > 0);

        m_allocator = allocator;

        IZ_BOOL result = IZ_FALSE;

        IZ_LIBUV_EXEC(result, uv_tcp_init(uv_default_loop(), &m_server));
        VRETURN(result);

        // 通信ポート・アドレスの設定.
        sockaddr_in inAddr;
        {
#if 0
            FILL_ZERO(&inAddr, sizeof(inAddr));

            inAddr.sin_family = AF_INET;
            inAddr.sin_port = htons(hostEp.getPort());

            auto address = hostEp.getAddress();

            if (address.isAny()) {
                setIp(inAddr, htonl(INADDR_ANY));
            }
            else {
                IZ_CHAR ip[64];
                address.toString(ip, COUNTOF(ip));

                setIp(inAddr, inet_addr(ip));
            }
#else
            auto address = hostEp.getAddress();
            auto port = hostEp.getPort();

            if (address.isAny()) {
                uv_ip4_addr("0,0,0,0", port, &inAddr);
            }
            else {
                IZ_CHAR ip[64];
                address.toString(ip, COUNTOF(ip));

                uv_ip4_addr(ip, port, &inAddr);
            }
#endif
        }

        // ソケットにアドレスを結びつける.
        IZ_LIBUV_EXEC(result, uv_tcp_bind(&m_server, (const sockaddr*)&inAddr, 0));
        VGOTO(result, __EXIT__);

        // コネクト要求をいくつまで待つかを設定.
        if (maxConnections > 0) {
            auto onConnection = std::bind(&TcpListener::OnConnection, this, std::placeholders::_1, std::placeholders::_2);
            IZ_LIBUV_EXEC(
                result,
                uv_listen((uv_stream_t*)&m_server, maxConnections, onConnection));
        }

        if (result) {
            m_isListening = IZ_TRUE;
            m_maxConnections = maxConnections;
            m_remotes.init(allocator, maxConnections);

            Network::start();
        }

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop(nullptr);
        }

        return result;
    }

    void TcpListener::OnConnection(uv_stream_t* server, int status)
    {
        if (status == -1) {
            // TODO
            IZ_ASSERT(IZ_FALSE);
            return;
        }

        TcpRemote* remote = nullptr;

        {
            std::lock_guard<std::mutex> lock(m_remotesLocker);
            for (IZ_UINT i = 0; i < m_maxConnections; i++) {
                auto& r = m_remotes.at(i);

                if (!r.IsConnected()) {
                    remote = &r;
                    break;
                }
            }
        }

        if (remote) {
            IZ_BOOL result = IZ_FALSE;
            IZ_LIBUV_EXEC(
                result,
                uv_tcp_init(uv_default_loop(), &remote->m_client));

            if (result) {
                IZ_LIBUV_EXEC(
                    result,
                    uv_accept(server, (uv_stream_t*)&remote->m_client));

                if (result) {
                    remote->m_handle = (uv_stream_t*)&remote->m_client;
                    remote->m_isConnected = IZ_TRUE;

                    std::lock_guard<std::mutex> lock(m_remotesLocker);
                    m_acceptedList.AddTail(remote->getListItem());
                }
            }
        }
    }

    void TcpListener::stop(std::function<void(void)> onClosed)
    {
        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpClient& remote = m_remotes.at(i);
            remote.stop(nullptr);
        }

        m_remotes.clear();

        m_acceptedList.Clear();

        uv_close(
            (uv_handle_t*)&m_server,
            [&](uv_handle_t* handle) {
            if (onClosed) {
                onClosed();
            }
            m_isListening = IZ_FALSE;
        });
    }

    TcpClient* TcpListener::acceptRemote()
    {
        std::lock_guard<std::mutex> lock(m_remotesLocker);

        if (m_acceptedList.GetItemNum() > 0) {
            auto item = m_acceptedList.GetTop();
            auto ret = item->GetData();
            item->Leave();
            return ret;
        }

        return nullptr;
    }
}    // namespace net
}    // namespace izanagi
