#include "network/NetworkUDPUtility.h"

namespace izanagi {
namespace net {
    // サーバーとして起動.
    IZ_BOOL UdpServer::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint)
    {
        IZ_BOOL result = Udp::startAsServer(allocator, endpoint);
        IZ_ASSERT(result);

        if (result) {
            m_thread.Init(
                [this] (void* data) {
                    loop();
                }, nullptr);

            result = m_thread.Start(allocator);
            IZ_ASSERT(result);
        }

        if (!result) {
            stop();
        }

        return result;
    }

    void UdpServer::onStop()
    {
        m_thread.Join();
    }

    // クライアントとして起動.
    IZ_BOOL UdpClient::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint)
    {
        IZ_BOOL result = Udp::startAsClient(allocator, endpoint);

        if (result) {
            m_thread.Init(
                [this](void* data) {
                loop();
            }, nullptr);

            result = m_thread.Start(allocator);
            IZ_ASSERT(result);
        }

        if (!result) {
            stop();
        }

        return result;
    }

    void UdpClient::onStop()
    {
        m_thread.Join();
    }
}    // namespace net
}    // namespace izanagi
