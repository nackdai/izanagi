#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"

namespace izanagi {
namespace net {
    /** TCP.
     */
    class Tcp {
    protected:
        Tcp();
        virtual ~Tcp();

        NO_COPIABLE(Tcp);

    protected:
        // 起動.
        IZ_BOOL start(
            const IPv4Endpoint& hostEp,
            IZ_UINT maxConnections = 0);

    public:
        /** 停止.
         */
        virtual void stop();

    protected:
        IZ_SOCKET m_socket{ IZ_INVALID_SOCKET };

        IPv4Endpoint m_host;
    };

    /**
    */
    class TcpClient : public Tcp {
        friend class TcpListener;

    public:
        TcpClient();
        virtual ~TcpClient();

    public:
        /** 起動.
         */
        IZ_BOOL start(const IPv4Endpoint& hostEp);

        /** 接続.
         */
        IZ_BOOL connectTo(const IPv4Endpoint& remoteEp);

        IZ_INT recieve(
            void* buf,
            IZ_UINT size);

        /** データを送信.
         */
        IZ_INT sendData(
            const void* data,
            IZ_UINT size);

    private:
        IZ_BOOL isActive();

    private:
        IPv4Endpoint m_remote;
    };

    /**
     */
    class TcpListener : public Tcp {
    public:
        TcpListener();
        virtual ~TcpListener();

    public:
        /** 起動.
         */
        IZ_BOOL start(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp,
            IZ_UINT maxConnections);

        virtual void stop() override;

        TcpClient* acceptRemote();

        IZ_INT recieveFrom(
            void* buf,
            IZ_UINT size,
            const IPv4Endpoint& remoteEp);

        /** 指定した接続先にデータを送信.
         */
        IZ_INT sendTo(
            const void* data,
            IZ_UINT size,
            const IPv4Endpoint& remoteEp);

        IZ_INT sendToAll(
            const void* data,
            IZ_UINT size);

    private:
        TcpClient* find(const IPv4Endpoint& remoteEp);

    private:
        IZ_UINT m_maxConnections{ 0 };

        std::mutex m_remotesLocker;
        CArray<TcpClient> m_remotes;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
