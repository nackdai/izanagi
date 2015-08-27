#if !defined(_IZANAGI_NETWORK_NETWORK_UDP_H__)
#define _IZANAGI_NETWORK_NETWORK_UDP_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"

namespace izanagi {
namespace net {
    /**
     */
    class Udp {
    public:
        Udp();
        virtual ~Udp();

        NO_COPIABLE(Udp);

    public:
        /** サーバーとして起動.
         */
        IZ_BOOL startAsServer(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp);

        /** クライアントとして起動.
         */
        IZ_BOOL startAsClient(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp);

        IZ_BOOL startAsClientAndConnectToServer(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& remoteEp);

        /** 停止.
         */
        void stop();

        IZ_INT wait(
            IZ_UINT sec = 0,
            IZ_UINT usec = 0);

        /** 受信したデータを取得.
         */
        IZ_INT recieve(
            void* buf,
            IZ_UINT size);

        IZ_INT recieveFrom(
            void* buf,
            IZ_UINT size,
            IPv4Endpoint& remoteEp);

        /** データを送信.
         */
        IZ_INT send(const void* data, IZ_UINT size);

        /** 指定した接続先にデータを送信.
         */
        IZ_INT sendTo(
            IPv4Endpoint& endpoint,
            const void* data, IZ_UINT size);

    protected:
        virtual void onStop() {}

        IZ_BOOL isRunning();

    private:
        class UdpRemote;

        UdpRemote* findRemote(const IPv4Endpoint& ep);
        UdpRemote* findRemote(const sockaddr_in& addr);

    protected:
        IMemoryAllocator* m_allocator;

        IZ_SOCKET m_socket;

        IPv4Endpoint m_host;

        std::mutex m_remotesLocker;
        CStdList<UdpRemote> m_remotes;

        std::atomic<IZ_BOOL> m_isRunning;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_UDP_H__)
