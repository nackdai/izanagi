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

    class TcpListener;

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

        virtual void stop() override;

        IZ_INT recieve(
            void* buf,
            IZ_UINT size);

        /** データを送信.
         */
        IZ_INT sendData(
            const void* data,
            IZ_UINT size);

        IZ_INT wait(
            IZ_UINT sec = 0,
            IZ_UINT usec = 0);

        void beginRecieve(
            std::function<void(const Packet*)> onRecieve,
            void* buf,
            IZ_UINT size);

        void endRecieve();

    private:
        IZ_BOOL isActive();

    private:
        TcpListener* m_server{ nullptr };

        IPv4Endpoint m_remote;

        std::atomic<IZ_BOOL> m_isRunning{ IZ_FALSE };
        std::thread m_recvThread;
    };

    /**
     */
    class TcpListener : public Tcp {
        friend class TcpClient;

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

        void beginAccept(std::function<void(TcpClient*)> onAccept);

        void endAccept();

        void beginRecieve(
            std::function<void(const Packet*)> onRecieve,
            void* buf,
            IZ_UINT size);

        void endRecieve();

    private:
        class TcpRemote : public TcpClient, sys::CSpinLock {
            friend class TcpListener;

        public:
            TcpRemote() {}
            virtual ~TcpRemote() {}
        };

        TcpRemote* acceptRemoteAsTcpRemote();

        TcpRemote* find(const IPv4Endpoint& remoteEp);

        TcpRemote* onAcceptRemote(
            IZ_UINT sec,
            IZ_UINT usec);

        void onRecvFrom(
            IZ_UINT sec,
            IZ_UINT usec,
            std::function<void(const Packet*)> onRecieve,
            void* buf,
            IZ_UINT size,
            IZ_SOCKET target = IZ_INVALID_SOCKET);

    private:
        IZ_UINT m_maxConnections{ 0 };

        std::mutex m_remotesLocker;
        CArray<TcpRemote> m_remotes;

        std::atomic<IZ_BOOL> m_isAccepting{ IZ_FALSE };
        std::thread m_acceptThread;

        std::atomic<IZ_BOOL> m_isRecieving{ IZ_FALSE };
        std::thread m_recvThread;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
