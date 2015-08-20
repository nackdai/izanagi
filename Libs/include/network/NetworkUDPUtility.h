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

    public:
        void setFuncRecv(std::function<void(net::Packet&)> func);

    protected:
        IZ_BOOL start(IMemoryAllocator* allocator);

        void loop();

    private:
        virtual void onStop() final;

    private:
        sys::CSpinLock m_locker;
        std::function<void(net::Packet&)> m_funcRecv;
        sys::CThread m_thread;
    };

    /**
     */
    class UdpServer : public UdpProxy {
    public:
        UdpServer() {}
        virtual ~UdpServer() {}

        NO_COPIABLE(UdpServer);

    public:
        /** �T�[�o�[�Ƃ��ċN��.
        */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint);
    };

    /**
     */
    class UdpClient : public UdpProxy {
    public:
        UdpClient() {}
        virtual ~UdpClient() {}

        NO_COPIABLE(UdpClient);

    public:
        /** �N���C�A���g�Ƃ��ċN��.
        */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint);
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_UDP_UTILITY_H__)
