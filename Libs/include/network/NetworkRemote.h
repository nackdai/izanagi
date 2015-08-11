#if !defined(_IZANAGI_NETWORK_NETWORK_REMOTE_H__)
#define _IZANAGI_NETWORK_NETWORK_REMOTE_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"
#include "network/IPv4Endpoint.h"

namespace izanagi {
namespace net {
    class Remote : public CPlacementNew, sys::CSpinLock {
        friend class Tcp;
        friend class CArray < Remote >;

    private:
        static Remote* create(IMemoryAllocator* allocator);

        static void deteteRemote(
            IMemoryAllocator* allocator,
            Remote* client);

    private:
        Remote();
        ~Remote();

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

        // 送信データを登録.
        IZ_BOOL registerData(
            IMemoryAllocator* allocator,
            IZ_UINT num,
            const void** data, IZ_UINT* size);

        void close();

        void reset();

    private:
        IZ_SOCKET m_socket;
        IPv4Endpoint m_endpoint;

        IMemoryAllocator* m_allocator;
        Packet m_sendPacket;

        IZ_BOOL m_isRegistered;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_REMOTE_H__)
