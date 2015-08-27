#include "network/NetworkUDPUtility.h"

namespace izanagi {
namespace net {
    UdpProxy::UdpProxy()
    {
    }

    UdpProxy::~UdpProxy()
    {
    }

    IZ_BOOL UdpProxy::start(
        IMemoryAllocator* allocator,
        std::function<void(const Packet&)> onRecv)
    {
        // TODO
        m_sizeRecvBuf = 1024;
        m_recvBuf = ALLOC(allocator, m_sizeRecvBuf);

        IZ_BOOL result = m_thread.Start(
            [&](void* data) {
            loop(onRecv);
        }, nullptr);

        IZ_ASSERT(result);

        return result;
    }

    void UdpProxy::onStop()
    {
        m_thread.Join();

        FREE(m_allocator, m_recvBuf);
    }

    void UdpProxy::loop(std::function<void(const Packet&)> onRecv)
    {
        for (;;) {
            if (!isRunning()) {
                break;
            }

            IZ_INT result = wait();

            if (result > 0) {
                IPv4Endpoint remoteEp;
                IZ_INT len = recieveFrom(m_recvBuf, m_sizeRecvBuf, remoteEp);

                if (len > 0) {
                    if (onRecv) {
                        Packet packet;

                        packet.endpoint = remoteEp;
                        packet.data = (IZ_UINT8*)m_recvBuf;
                        packet.size = len;

                        onRecv(packet);
                    }
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
        const IPv4Endpoint& hostEp,
        std::function<void(const Packet&)> onRecv/*= nullptr*/)
    {
        IZ_BOOL result = Udp::startAsServer(allocator, hostEp);
        IZ_ASSERT(result);

        if (result) {
            result = UdpProxy::start(allocator, onRecv);
        }

        return result;
    }

    // クライアントとして起動.
    IZ_BOOL UdpClient::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& hostEp,
        std::function<void(const Packet&)> onRecv/*= nullptr*/)
    {
        IZ_BOOL result = Udp::startAsClient(allocator, hostEp);

        if (result) {
            result = UdpProxy::start(allocator, onRecv);
        }

        return result;
    }

    IZ_BOOL UdpClient::connect(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& remoteEp,
        std::function<void(const Packet&)> onRecv/*= nullptr*/)
    {
        IZ_BOOL result = Udp::startAsClient(allocator, remoteEp);

        if (result) {
            result = UdpProxy::start(allocator, onRecv);
        }

        return result;
    }
}    // namespace net
}    // namespace izanagi
