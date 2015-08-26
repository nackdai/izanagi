#include "network/NetworkTCPUtility.h"

namespace izanagi {
namespace net {
    // サーバーとして起動.
    IZ_BOOL TcpServer::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint,
        IZ_UINT maxConnections)
    {
        IZ_BOOL result = Tcp::startAsServer(allocator, endpoint, maxConnections);
        IZ_ASSERT(result);

        if (result) {
            result = m_thread.Start(
                [this] (void* data) {
                    loop();
                }, nullptr);

            IZ_ASSERT(result);
        }

        if (!result) {
            stop();
        }

        return result;
    }

    void TcpServer::onStop()
    {
        m_thread.Join();
    }

    // クライアントとして起動.
    IZ_BOOL TcpClient::start(
        IMemoryAllocator* allocator,
        const IPv4Endpoint& endpoint,
        IZ_BOOL isBlock)
    {
        IZ_BOOL result = Tcp::startAsClient(allocator, endpoint);

        if (result) {
            if (isBlock) {
                result = connectServer();
            }
            else {
                result = m_thread.Start(
                    [this] (void* data) {
                        connectServer();
                    }, nullptr);
                VGOTO(result, __EXIT__);

                result = m_threadSub.Start(
                    [this] (void* data) {
                        loop();
                    }, nullptr);
            }
        }

    __EXIT__:
        if (!result) {
            stop();
        }

        return result;
    }

    void TcpClient::onStop()
    {
        m_thread.Join();
        m_threadSub.Join();
    }
}    // namespace net
}    // namespace izanagi
