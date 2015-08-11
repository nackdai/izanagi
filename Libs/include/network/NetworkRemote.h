#if !defined(_IZANAGI_NETWORK_NETWORK_REMOTE_H__)
#define _IZANAGI_NETWORK_NETWORK_REMOTE_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"
#include "network/IPv4Endpoint.h"

namespace izanagi {
namespace net {
    // リモート
    class Remote : public CPlacementNew {
    protected:
        template <typename _T>
        static _T* create(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            _T* remote = new(buf)_T;
            return remote;
        }

        static void deteteRemote(
            IMemoryAllocator* allocator,
            Remote* remote);

    protected:
        Remote();
        virtual ~Remote();

    public:
        // 送信データを登録.
        IZ_BOOL registerData(
            IMemoryAllocator* allocator,
            IZ_UINT num,
            const void** data, IZ_UINT* size);

        void clear();

    protected:
        IPv4Endpoint m_endpoint;

        IMemoryAllocator* m_allocator;
        Packet m_sendPacket;

        IZ_BOOL m_isRegistered;
    };

    // TCPリモート
    class TcpRemote : public Remote, sys::CSpinLock {
        friend class Remote;
        friend class Tcp;
        friend class CArray < TcpRemote >;

    private:
        static TcpRemote* create(IMemoryAllocator* allocator);

        static void deteteRemote(
            IMemoryAllocator* allocator,
            Remote* client);

    private:
        TcpRemote();
        virtual ~TcpRemote() {}

    private:
        // クライアントと接続しているソケットを割り当てる.
        void setSocket(IZ_SOCKET socket);

        // クライアントと接続しているソケットを取得.
        IZ_SOCKET getSocekt();

        // ソケットが有効かどうかを取得.
        IZ_BOOL isActive() const;

        // データを送信.
        IZ_INT sendData(const void* data, IZ_UINT size);

        // 登録されているデータを送信.
        IZ_INT sendData();

        // データを受信.
        IZ_INT recieveData(void* data, IZ_UINT size);

        void close();

        void reset();

    private:
        IZ_SOCKET m_socket;
    };

    // UDPリモート
    class UdpRemote : public Remote, sys::CSpinLock {
        friend class Remote;
        friend class Udp;

    private:
        static UdpRemote* create(IMemoryAllocator* allocator);

        static void deteteRemote(
            IMemoryAllocator* allocator,
            Remote* client);

    private:
        UdpRemote();
        virtual ~UdpRemote() {}

    private:
        // データを送信.
        IZ_INT sendData(
            IZ_SOCKET socket,
            const void* data, IZ_UINT size);

        // 登録されているデータを送信.
        IZ_INT sendData(IZ_SOCKET socket);

        CStdList<UdpRemote>::Item* getListItem();

    private:
        CStdList<UdpRemote>::Item m_listItem;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_REMOTE_H__)
