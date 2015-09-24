#include "network/NetworkUDP_libuv.h"
#include "network/IPv4Endpoint.h"
#include "network/Network.h"

namespace izanagi {
namespace net {
#define IZ_LIBUV_EXEC(result, func)\
    {\
        int err = func;\
        result = (err == 0);\
        if (!result) { IZ_PRINTF("%s\n", uv_err_name(err)); }\
    }

    Udp::Udp()
    {
    }

    Udp::~Udp()
    {
        stop();
    }

    // 起動.
    IZ_BOOL Udp::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp)
    {
        if (IsStarted()) {
            return IZ_FALSE;
        }

        IZ_BOOL result = IZ_FALSE;

        IZ_LIBUV_EXEC(
            result,
            uv_udp_init(uv_default_loop(), &m_udp));
        VRETURN(result);

        // 通信ポート・アドレスの設定
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

        // ソケットにアドレスを結びつける
        IZ_LIBUV_EXEC(
            result,
            uv_udp_bind(&m_udp, (const sockaddr*)&inAddr, 0));
        
        if (result) {
            m_allocator = allocator;
            m_isBindAddr = IZ_TRUE;
            m_host = hostEp;

            m_isStarted = IZ_TRUE;

            startRecieve();
        }
        else {
            IZ_ASSERT(IZ_FALSE);
            stop();
        }

        return result;
    }

    // 起動.
    IZ_BOOL Udp::start(IMemoryAllocator* allocator)
    {
        if (IsStarted()) {
            return IZ_FALSE;
        }

        IZ_BOOL result = IZ_FALSE;

        IZ_LIBUV_EXEC(
            result,
            uv_udp_init(uv_default_loop(), &m_udp));

        if (result) {
            m_allocator = allocator;

            m_isStarted = IZ_TRUE;

            startRecieve();
        }
        else {
            IZ_ASSERT(IZ_FALSE);

            stop();
        }

        return result;
    }

    IZ_BOOL Udp::connectTo(const IPv4Endpoint& remoteEp)
    {
        if (m_specifiedRemote) {
            return IZ_FALSE;
        }

        m_specifiedRemote = IZ_TRUE;
        m_remote = remoteEp;

        return IZ_TRUE;
    }

    // 停止.
    void Udp::stop()
    {
        if (!IsStarted()) {
            return;
        }

        if (m_isRecieving) {
            IZ_BOOL result = IZ_FALSE;

            IZ_LIBUV_EXEC(
                result,
                uv_udp_recv_stop(&m_udp));
            IZ_ASSERT(result);

            m_isRecieving = IZ_FALSE;

            std::lock_guard<std::mutex> lock(m_listRecvDataLocker);

            CStdList<RecvData>::Item* item = m_listRecvData.GetTop();
            while (item) {
                auto data = item->GetData();
                item = item->GetNext();

                delete data;
                FREE(m_allocator, data);
            }

            CallbackRegister::Remove(m_cbAllocated);
            CallbackRegister::Remove(m_cbRecieved);
        }

        // NOTE
        // uv_close 内部でコールバックが呼ばれていないぽい.

        uv_close(
            (uv_handle_t*)&m_udp,
            IZ_NULL);

        m_isStarted = IZ_FALSE;
    }

    void Udp::startRecieve()
    {
        auto onAlloc = std::bind(
            &Udp::OnAlloc,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3);

        auto onRecieved = std::bind(
            &Udp::OnRecieved,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4,
            std::placeholders::_5);

        m_cbAllocated.Set(
            CallbackRegister::Key(CallbackRegister::Type::Alloc, &m_udp),
            onAlloc);

        m_cbRecieved.Set(
            CallbackRegister::Key(CallbackRegister::Type::Recieve, &m_udp),
            onRecieved);

        CallbackRegister::RegistPermanently(m_cbAllocated);
        CallbackRegister::RegistPermanently(m_cbRecieved);

        m_isRecieving = IZ_TRUE;

        IZ_BOOL result = IZ_FALSE;

        IZ_LIBUV_EXEC(
            result,
            uv_udp_recv_start(
                &m_udp,
                [](uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf){
                CallbackRegister::Invoke<CallbackOnAlloc>(CallbackRegister::Key(CallbackRegister::Type::Alloc, handle), handle, suggested_size, buf);
            },
                [](uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags){
                CallbackRegister::Invoke<CallbackOnRecieved>(CallbackRegister::Key(CallbackRegister::Type::Recieve, handle), handle, nread, buf, addr, flags);
            }));

        IZ_ASSERT(result);

        Network::start();
    }

    void Udp::OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
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

    void Udp::OnRecieved(
        uv_udp_t* handle,
        ssize_t nread,
        const uv_buf_t* buf,
        const struct sockaddr* addr,
        unsigned flags)
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

            // TODO
            //found->remote.set();
        }

        if (nread < 0 || addr == IZ_NULL) {
            // TODO
            IZ_ASSERT(IZ_FALSE);
            return;
        }
    }

    // 受信したデータを取得.
    IZ_INT Udp::recieve(
        void* buf,
        IZ_UINT size)
    {
        IPv4Endpoint remoteEp;

        IZ_INT ret = recieveFrom(buf, size, remoteEp);
        return ret;
    }

    // 受信したデータを取得.
    IZ_INT Udp::recieveFrom(
        void* buf,
        IZ_UINT size,
        IPv4Endpoint& remoteEp)
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

        // TODO
        // remoteEp

        delete data;
        FREE(m_allocator, data);

        return ret;
    }

    IZ_BOOL Udp::sendData(const void* data, IZ_UINT size)
    {
        IZ_ASSERT(m_specifiedRemote);
        return sendTo(data, size, m_remote);
    }

    // 指定した接続先にデータを送信.
    IZ_BOOL Udp::sendTo(
        const void* data,
        IZ_UINT size,
        const IPv4Endpoint& remoteEp)
    {
        sockaddr_in addr;
        remoteEp.get(addr);

        IZ_BOOL result = IZ_FALSE;

        uv_buf_t buf = uv_buf_init((char*)data, size);

        // NOTE
        // uv_udp_send 内部でコールバックが呼ばれていないぽい.

        IZ_LIBUV_EXEC(
            result,
            uv_udp_send(&m_reqSend, &m_udp, &buf, 1, (const sockaddr*)&addr, IZ_NULL));

        return result;
    }
}    // namespace net
}    // namespace izanagi
