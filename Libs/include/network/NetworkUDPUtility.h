#if !defined(_IZANAGI_NETWORK_NETWORK_UDP_UTILITY_H__)
#define _IZANAGI_NETWORK_NETWORK_UDP_UTILITY_H__

#include "network/NetworkUDP.h"

namespace izanagi {
namespace net {
    /**
     */
    class UdpProxy : public Udp {
    protected:
        UdpProxy();
        virtual ~UdpProxy();

    protected:
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            std::function<void(const Packet&)> onRecv);

        void loop(std::function<void(const Packet&)> onRecv);

    private:
        virtual void onStop() final;

    private:        
        sys::CThread m_thread;

        void* m_recvBuf{ nullptr };
        IZ_UINT m_sizeRecvBuf{ 0 };
    };

    /**
     */
    class UdpServer : public UdpProxy {
    public:
        UdpServer() {}
        virtual ~UdpServer() {}

        NO_COPIABLE(UdpServer);

    public:
        /** サーバーとして起動.
        */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp,
            std::function<void(const Packet&)> onRecv = nullptr);
    };

    /**
     */
    class UdpClient : public UdpProxy {
    public:
        UdpClient() {}
        virtual ~UdpClient() {}

        NO_COPIABLE(UdpClient);

    public:
        /** クライアントとして起動.
        */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp,
            std::function<void(const Packet&)> onRecv = nullptr);

        IZ_BOOL connect(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& remoteEp,
            std::function<void(const Packet&)> onRecv = nullptr);
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_UDP_UTILITY_H__)
