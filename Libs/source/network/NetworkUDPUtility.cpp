#include "network/NetworkUDPUtility.h"

namespace izanagi {
namespace net {
    UdpProxy::UdpProxy()
    {
        m_funcRecv = nullptr;
    }

    UdpProxy::~UdpProxy()
    {
    }

    void UdpProxy::setFuncRecv(std::function<void(net::Packet&)> func)
    {
        sys::Lock lock(m_locker);
        m_funcRecv = func;
    }

    IZ_BOOL UdpProxy::start(IMemoryAllocator* allocator)
    {
        IZ_BOOL result = m_thread.Start(
            [this](void* data) {
            loop();
        }, nullptr);

        IZ_ASSERT(result);

        return result;
    }

    void UdpProxy::onStop()
    {
        m_thread.Join();
    }

    void UdpProxy::loop()
    {
        for (;;) {
            if (!isRunning()) {
                break;
            }

            IZ_INT result = waitForRecieving();

            if (result > 0) {
                Udp::Packet* packet = nullptr;

                IZ_BOOL resRecv = onRecieve(packet);

                if (resRecv) {
                    {
                        sys::Lock lock(m_locker);
                        if (m_funcRecv) {
                            m_funcRecv(*packet);
                        }
                    }

                    Udp::deletePacket(packet);
                }
            }
            else if (result < 0) {
                // TODO
                // exception
            }
        }
    }

    ////////////////////////////////////////////////////

    // サーバーとして起動.
    IZ_BOOL UdpServer::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint)
    {
        IZ_BOOL result = Udp::startAsServer(allocator, endpoint);
        IZ_ASSERT(result);

        if (result) {
            result = UdpProxy::start(allocator);
        }

        if (!result) {
            stop();
        }

        return result;
    }

    

    // クライアントとして起動.
    IZ_BOOL UdpClient::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint)
    {
        IZ_BOOL result = Udp::startAsClient(allocator, endpoint);

        if (result) {
            result = UdpProxy::start(allocator);
        }

        if (!result) {
            stop();
        }

        return result;
    }
}    // namespace net
}    // namespace izanagi
