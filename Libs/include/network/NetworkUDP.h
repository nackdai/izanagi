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
        /** �T�[�o�[�Ƃ��ċN��.
         */
        IZ_BOOL startAsServer(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint);

        /** �N���C�A���g�Ƃ��ċN��.
         */
        IZ_BOOL startAsClient(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint);

        /** ��~.
         */
        void stop();

        IZ_BOOL run(IZ_CHAR* recvBuf, IZ_UINT size);

    private:
        UdpRemote* findRemote(const sockaddr_in& addr);

    private:
        class Packet : public net::Packet, CPlacementNew {
        public:
            static Packet* create(IMemoryAllocator* allocator, IZ_UINT len);

            Packet()
            {
                listItem.Init(this);
            }
            ~Packet() {}

            CStdList<Packet>::Item listItem;
        };

    private:
        IMemoryAllocator* m_allocator;

        IZ_SOCKET m_socket;

        std::mutex m_remotesLocker;
        CStdList<UdpRemote> m_remotes;

        std::mutex m_recvDataLocker;
        CStdList<Packet> m_recvData;

        std::atomic<IZ_BOOL> m_isRunnning;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_UDP_H__)
