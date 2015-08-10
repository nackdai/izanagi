#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_UTILITY_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_UTILITY_H__

#include "network/NetworkTCP.h"

namespace izanagi {
namespace net {
    /**
     */
    class TcpServer : public Tcp {
    public:
        TcpServer() {}
        virtual ~TcpServer() {}

        NO_COPIABLE(TcpServer);

    public:
        /** サーバーとして起動.
        */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint,
            IZ_UINT maxConnections);

    private:
        virtual void onStop() final;

    private:
        sys::CThread m_thread;
    };

    /**
     */
    class TcpClient : public Tcp {
    public:
        TcpClient() {}
        virtual ~TcpClient() {}

        NO_COPIABLE(TcpClient);

    public:
        /** クライアントとして起動.
        */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint,
            IZ_BOOL isBlock);

    private:
        virtual void onStop() final;

    private:
        sys::CThread m_thread;
        sys::CThread m_threadSub;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_UTILITY_H__)
