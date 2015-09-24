#include "network/NetworkTCP_libuv.h"
#include "network/IPv4Endpoint.h"
#include "network/Network.h"

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
    IZ_BOOL TcpClient::start(IMemoryAllocator* allocator)
    {
        IZ_BOOL result = IZ_FALSE;

        m_allocator = allocator;

        IZ_LIBUV_EXEC(result, uv_tcp_init(uv_default_loop(), &m_client));

        // NOTE
        // libuvでは、クライアント側は bind すると、connect で失敗する.

        if (!result) {
            IZ_ASSERT(IZ_FALSE);

            stop();
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
            auto address = remoteEp.getAddress();
            result = !address.isAny();

            VGOTO(result, __EXIT__);

            auto port = remoteEp.getPort();

            IZ_CHAR ip[64];
            address.toString(ip, COUNTOF(ip));

            uv_ip4_addr(ip, port, &serverAddr);
        }

        m_isConnecting = IZ_TRUE;

        m_onConnected = onConnected;

        m_cbConnected.Set(
            CallbackRegister::Key(CallbackRegister::Type::Connect, &m_reqConnect),
            onConnect);

        CallbackRegister::Regist(m_cbConnected);

        IZ_LIBUV_EXEC(
            result,
            uv_tcp_connect(&m_reqConnect, &m_client, (const sockaddr*)&serverAddr, [](uv_connect_t *req, int status){
            CallbackRegister::Invoke<CallbackConnected>(CallbackRegister::Key(CallbackRegister::Type::Connect, req), req, status);
        }));

        if (result) {
            m_remote = remoteEp;

            Network::start();
        }

    __EXIT__:
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            stop();
        }

        return result;
    }

    void TcpClient::OnConnect(uv_connect_t *req, int status)
    {
        m_isConnecting = IZ_FALSE;

        if (status < 0) {
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

    void TcpClient::stop()
    {
        if (!IsConnected()) {
            return;
        }

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

            std::lock_guard<std::mutex> lock(m_listRecvDataLocker);

            CStdList<RecvData>::Item* item = m_listRecvData.GetTop();
            while (item) {
                auto data = item->GetData();
                item = item->GetNext();

                delete data;
                FREE(m_allocator, data);
            }

            CallbackRegister::Remove(m_cbAllocated);
            CallbackRegister::Remove(m_cbReadEnd);
        }

        // NOTE
        // どうも内部でコールバックを呼ばないぽい・・・

        uv_close(
            (uv_handle_t*)m_handle,
            IZ_NULL);

        m_isConnected = IZ_FALSE;
    }

    IZ_INT TcpClient::recieve(
        void* buf,
        IZ_UINT size)
    {
        CStdList<RecvData>::Item* item = nullptr;

        {
            std::lock_guard<std::mutex> lock(m_listRecvDataLocker);
            item = m_listRecvData.GetTop();
        }

        if (item == nullptr) {
            return -1;
        }

        auto data = item->GetData();

        if (data->length < 0) {
            return -1;
        }

        if (size < data->length) {
            IZ_ASSERT(IZ_FALSE);
            return -1;
        }

        memcpy(buf, data->buf->base, data->length);

        IZ_INT ret = data->length;

        delete data;
        FREE(m_allocator, data);

        return ret;
    }

    void TcpClient::startRecieve()
    {
        // 接続チェック.
        IZ_ASSERT(IsConnected());

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

        m_cbAllocated.Set(
            CallbackRegister::Key(CallbackRegister::Type::Alloc, m_handle),
            onAlloc);

        m_cbReadEnd.Set(
            CallbackRegister::Key(CallbackRegister::Type::Read, m_handle),
            onReadEnd);

        CallbackRegister::RegistPermanently(m_cbAllocated);
        CallbackRegister::RegistPermanently(m_cbReadEnd);

        IZ_LIBUV_EXEC(
            result,
            uv_read_start(
            m_handle, 
            [](uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
            CallbackRegister::Invoke<CallbackOnAlloc>(CallbackRegister::Key(CallbackRegister::Type::Alloc, handle), handle, suggested_size, buf);
        }, 
            [](uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf){
            CallbackRegister::Invoke<CallbackOnReadEnd>(CallbackRegister::Key(CallbackRegister::Type::Read, stream), stream, nread, buf);
        }));
        IZ_ASSERT(result);
    }

    void TcpClient::OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
    {
        IZ_UINT8* tmp = (IZ_UINT8*)ALLOC(m_allocator, suggested_size + sizeof(RecvData));

        RecvData* data = new(tmp)RecvData;
        tmp += sizeof(RecvData);

        data->buf = (uv_buf_t*)tmp;
        tmp += sizeof(uv_buf_t);

        {
            std::lock_guard<std::mutex> lock(m_listRecvDataLocker);
            m_listRecvData.AddTail(&data->listItem);
        }

        *(data->buf) = uv_buf_init((char*)tmp, suggested_size);

        *buf = *(data->buf);
    }

    void TcpClient::OnReadEnd(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
    {
        if (nread == UV_EOF) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(m_listRecvDataLocker);

        auto found = m_listRecvData.Find([&](RecvData* data) {
            return (data->buf->base == buf->base);
        });

        if (found) {
            found->length = nread;
        }

        if (nread < 0) {
            // TODO
            IZ_ASSERT(IZ_FALSE);

            if (found) {
                delete found;
                FREE(m_allocator, found);
            }
        }
    }

    IZ_BOOL TcpClient::sendData(
        const void* data,
        IZ_UINT size,
        std::function<void(IZ_BOOL)> onSent)
    {
        // 接続チェック.
        if (!IsConnected()) {
            return IZ_FALSE;
        }

        if (IsSending()) {
            return IZ_FALSE;
        }

        uv_buf_t buf = uv_buf_init((char*)data, size);

        buf.len = size;
        buf.base = (char*)data;

        m_isWriting = IZ_TRUE;

        IZ_BOOL result = IZ_FALSE;

#if 0
        auto onWriteEnd = std::bind(&TcpClient::OnWriteEnd, this, std::placeholders::_1, std::placeholders::_2);

        m_cbWriteEnd.Set(
            CallbackRegister::Key(CallbackRegister::Type::Write, &m_reqWrite),
            onWriteEnd);

        CallbackRegister::Regist(m_cbWriteEnd);

        // 書き込み
        IZ_LIBUV_EXEC(
            result,
            uv_write(&m_reqWrite, m_handle, &buf, 1, [](uv_write_t *req, int status){
            CallbackRegister::Invoke<CallbackOnWriteEnd>(CallbackRegister::Key(CallbackRegister::Type::Write, req), req, status);
        }));

        if (result) {
            m_onSent = onSent;
        }
#else
        // NOTE
        // どうも内部でコールバックを呼ばないぽい・・・

        IZ_LIBUV_EXEC(
            result,
            uv_write(&m_reqWrite, m_handle, &buf, 1, IZ_NULL));

        m_isWriting = IZ_FALSE;

        if (onSent) {
            onSent(result);
        }
#endif

        return result;
    }

    void TcpClient::OnWriteEnd(uv_write_t *req, int status)
    {
        m_isWriting = IZ_FALSE;

        if (status < 0) {
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
        }

        // ソケットにアドレスを結びつける.
        IZ_LIBUV_EXEC(result, uv_tcp_bind(&m_server, (const sockaddr*)&inAddr, 0));
        VGOTO(result, __EXIT__);

        // コネクト要求をいくつまで待つかを設定.
        if (maxConnections > 0) {
            auto onConnection = std::bind(&TcpListener::OnConnection, this, std::placeholders::_1, std::placeholders::_2);

            m_cbConnection.Set(
                CallbackRegister::Key(CallbackRegister::Type::Listen, &m_server),
                onConnection);

            CallbackRegister::Regist(m_cbConnection);

            IZ_LIBUV_EXEC(
                result,
                uv_listen((uv_stream_t*)&m_server, maxConnections, [](uv_stream_t* server, int status){
                CallbackRegister::Invoke<CallbackOnConnection>(CallbackRegister::Key(CallbackRegister::Type::Listen, server), server, status);
            }));
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

            stop();
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
                    remote->m_allocator = m_allocator;
                    remote->m_handle = (uv_stream_t*)&remote->m_client;
                    remote->m_isConnected = IZ_TRUE;

                    remote->startRecieve();

                    std::lock_guard<std::mutex> lock(m_remotesLocker);
                    m_acceptedList.AddTail(remote->getListItem());
                }
            }
        }
    }

    void TcpListener::stop()
    {
        for (IZ_UINT i = 0; i < m_remotes.getNum(); i++) {
            TcpClient& remote = m_remotes.at(i);
            remote.stop();
        }

        m_remotes.clear();

        m_acceptedList.Clear();

        m_isListening = IZ_FALSE;

        // NOTE
        // どうも内部でコールバックを呼ばないぽい・・・

        uv_close(
            (uv_handle_t*)&m_server,
            IZ_NULL);
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
