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
                m_thread.Init(
                    [this] (void* data) {
                        connectServer();
                    }, nullptr);

                result = m_thread.Start(allocator);
                VGOTO(result, __EXIT__);

                m_threadSub.Init(
                    [this] (void* data) {
                        loop();
                    }, nullptr);

                result = m_threadSub.Start(allocator);
                VGOTO(result, __EXIT__);
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
