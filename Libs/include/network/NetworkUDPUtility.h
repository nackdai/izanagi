#if !defined(_IZANAGI_NETWORK_NETWORK_UDP_UTILITY_H__)
#define _IZANAGI_NETWORK_NETWORK_UDP_UTILITY_H__

#include "network/NetworkUDP.h"

namespace izanagi {
namespace net {
    /**
     */
    class UdpServer : public Udp {
    public:
        UdpServer() {}
        virtual ~UdpServer() {}

        NO_COPIABLE(UdpServer);

    public:
        /** サーバーとして起動.
        */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint);

    private:
        virtual void onStop() final;

    private:
        sys::CThread m_thread;
    };

    /**
     */
    class UdpClient : public Udp {
    public:
        UdpClient() {}
        virtual ~UdpClient() {}

        NO_COPIABLE(UdpClient);

    public:
        /** クライアントとして起動.
        */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint);

    private:
        virtual void onStop() final;

    private:
        sys::CThread m_thread;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_UDP_UTILITY_H__)
