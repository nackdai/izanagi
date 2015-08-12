#include "network/NetworkRemote.h"

namespace izanagi {
namespace net {
    void Remote::deteteRemote(
        IMemoryAllocator* allocator,
        Remote* remote)
    {
        delete remote;
        FREE(allocator, remote);
    }

    Remote::Remote()
    {
        m_allocator = nullptr;
        m_sendPacket.size = 0;
        m_sendPacket.data = nullptr;

        m_isRegistered = IZ_FALSE;
    }

    Remote::~Remote()
    {
        clear();
    }

    // 送信データを登録.
    IZ_BOOL Remote::registerData(
        IMemoryAllocator* allocator,
        IZ_UINT num,
        const void** data, IZ_UINT* size)
    {
        IZ_ASSERT(allocator != IZ_NULL);

        IZ_UINT total = 0;
        for (IZ_UINT i = 0; i < num; i++) {
            total += size[i];
        }

        if (m_sendPacket.data == nullptr) {
            // メモリが確保されていない場合
            m_sendPacket.data = (IZ_CHAR*)ALLOC(allocator, total);
            VRETURN(m_sendPacket.data);

            m_sendPacket.size = total;

            m_allocator = allocator;
        }
        else if (m_sendPacket.size < total) {
            // サイズが小さい場合
            VRETURN(m_allocator == allocator);

            m_sendPacket.data = (IZ_CHAR*)REALLOC(allocator, m_sendPacket.data, total);
            VRETURN(m_sendPacket.data);

            m_sendPacket.size = total;
        }

        auto dst = m_sendPacket.data;

        for (IZ_UINT i = 0; i < num; i++) {
            auto s = size[i];

            memcpy(dst, data[i], s);
            dst += s;
        }

        m_isRegistered = IZ_TRUE;

        return IZ_TRUE;
    }

    IZ_BOOL Remote::isRegistered()
    {
        return m_isRegistered;
    }

    void Remote::clear()
    {
        if (m_allocator != nullptr) {
            FREE(m_allocator, m_sendPacket.data);

            m_sendPacket.size = 0;
            m_sendPacket.data = nullptr;

            m_allocator = nullptr;
        }

        m_isRegistered = IZ_FALSE;
    }

    //////////////////////////////////////////////////

    TcpRemote* TcpRemote::create(IMemoryAllocator* allocator)
    {
        return Remote::create<TcpRemote>(allocator);
    }

    void TcpRemote::deteteRemote(
        IMemoryAllocator* allocator,
        Remote* remote)
    {
        Remote::deteteRemote(allocator, remote);
    }

    TcpRemote::TcpRemote()
    {
        m_socket = IZ_INVALID_SOCKET;
    }

    // クライアントと接続しているソケットを割り当てる.
    void TcpRemote::setSocket(IZ_SOCKET socket)
    {
        m_socket = socket;
    }

    // クライアントと接続しているソケットを取得.
    IZ_SOCKET TcpRemote::getSocekt()
    {
        return m_socket;
    }

    // ソケットが有効かどうかを取得.
    IZ_BOOL TcpRemote::isActive() const
    {
        return (m_socket != IZ_INVALID_SOCKET);
    }

    // データを送信.
    IZ_INT TcpRemote::sendData(const void* data, IZ_UINT size)
    {
        VRETURN_VAL(isValidSocket(m_socket), 0);
        IZ_INT ret = send(m_socket, (const char*)data, size, 0);
        return ret;
    }

    // 登録されているデータを送信.
    IZ_INT TcpRemote::sendData()
    {
        if (m_sendPacket.data == nullptr
            || !m_isRegistered)
        {
            return 0;
        }

        VRETURN_VAL(isValidSocket(m_socket), 0);

        IZ_INT ret = sendData(m_sendPacket.data, m_sendPacket.size);

        if (ret > 0) {
            m_isRegistered = IZ_FALSE;
        }

        return ret;
    }

    // データを受信.
    IZ_INT TcpRemote::recieveData(void* data, IZ_UINT size)
    {
        VRETURN_VAL(isValidSocket(m_socket), 0);
        IZ_INT ret = recv(m_socket, (char*)data, size, 0);
        return ret;
    }

    void TcpRemote::close()
    {
        if (isValidSocket(m_socket)) {
            shutdown(m_socket, SD_BOTH);
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        clear();
    }

    void TcpRemote::reset()
    {
        if (isValidSocket(m_socket)) {
            closesocket(m_socket);
            m_socket = IZ_INVALID_SOCKET;
        }

        m_isRegistered = IZ_FALSE;
    }

    //////////////////////////////////////////////////

    UdpRemote* UdpRemote::create(IMemoryAllocator* allocator)
    {
        return Remote::create<UdpRemote>(allocator);
    }

    void UdpRemote::deteteRemote(
        IMemoryAllocator* allocator,
        Remote* remote)
    {
        Remote::deteteRemote(allocator, remote);
    }

    UdpRemote::UdpRemote()
    {
        m_listItem.Init(this);
    }

    // データを送信.
    IZ_INT UdpRemote::sendData(
        IZ_SOCKET socket,
        const void* data, IZ_UINT size)
    {
        VRETURN_VAL(isValidSocket(socket), 0);
        VRETURN_VAL(!m_endpoint.getAddress().isAny(), 0);

        sockaddr_in addr;
        FILL_ZERO(&addr, sizeof(addr));

        m_endpoint.get(addr);

        IZ_INT ret = sendto(
            socket,
            (const char*)data, size,
            0,
            (const sockaddr*)&addr,
            sizeof(addr));

        // TODO
        IZ_ASSERT(ret == size);

        return ret;
    }

    // 登録されているデータを送信.
    IZ_INT UdpRemote::sendData(IZ_SOCKET socket)
    {
        if (m_sendPacket.data == nullptr
            || !m_isRegistered)
        {
            return 0;
        }

        VRETURN_VAL(isValidSocket(socket), 0);
        VRETURN_VAL(!m_endpoint.getAddress().isAny(), 0);

        IZ_INT ret = sendData(
            socket,
            m_sendPacket.data, m_sendPacket.size);

        if (ret > 0) {
            m_isRegistered = IZ_FALSE;
        }

        return ret;
    }

    // 有効かどうか.
    IZ_BOOL UdpRemote::isActive() const
    {
        return !m_endpoint.getAddress().isAny();
    }

    CStdList<UdpRemote>::Item* UdpRemote::getListItem()
    {
        return &m_listItem;
    }
}    // namespace net
}    // namespace izanagi
