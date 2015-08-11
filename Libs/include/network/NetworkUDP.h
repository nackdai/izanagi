#if !defined(_IZANAGI_NETWORK_NETWORK_UDP_H__)
#define _IZANAGI_NETWORK_NETWORK_UDP_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"

namespace izanagi {
namespace net {
    class UdpRemote;

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
            const IPv4Endpoint& endpoint);

        /** 停止.
         */
        void stop();

        IZ_BOOL run(IZ_CHAR* recvBuf, IZ_UINT size);

    private:
        UdpRemote* findRemote(const sockaddr_in& addr);

    private:
        IMemoryAllocator* m_allocator;

        IZ_SOCKET m_socket;

        CStdList<UdpRemote> m_remotes;

        std::atomic<IZ_BOOL> m_isRunnning;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_UDP_H__)
