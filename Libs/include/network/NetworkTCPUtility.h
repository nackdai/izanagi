#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_UTILITY_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_UTILITY_H__

#include "network/NetworkTCP.h"

namespace izanagi {
namespace net {
    class TcpProxy : public Tcp {
    protected:
        TcpProxy() {}
        virtual ~TcpProxy() {}

    protected:
        void run(
            std::function<void(const Packet&)> onRecv,
            IZ_UINT sec = 0,
            IZ_UINT usec = 0);

        void loop(std::function<void(const Packet&)> onRecv);

    protected:
        IZ_UINT8* m_recvBuf{ nullptr };
        IZ_UINT m_recvBufSize{ 0 };
    };

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
            const IPv4Endpoint& hostEp,
            IZ_UINT maxConnections,
            std::function<void(const Packet&)> onRecv);

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
