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

        /** ��M�����f�[�^���擾.
         */
        IZ_BOOL recieve(std::function<void(const net::Packet&)> func);

        /** �f�[�^�𑗐M.
         */
        IZ_BOOL send(const void* data, IZ_UINT size);

        /** �w�肵���ڑ���Ƀf�[�^�𑗐M.
         */
        IZ_BOOL sendTo(
            IPv4Endpoint& endpoint,
            const void* data, IZ_UINT size);

    private:
        UdpRemote* findRemote(const IPv4Endpoint& ep);
        UdpRemote* findRemote(const sockaddr_in& addr);

    protected:
        virtual void onStop() {}

        class Packet;

        Packet* createPacket(IZ_UINT len);
        void deletePacket(Packet* packet);

        IZ_BOOL isRunning();

        IZ_BOOL onRecieve(Udp::Packet*& packet);

        IZ_INT waitForRecieving();

    protected:
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

        IZ_UINT m_bufSize;
        IZ_CHAR* m_recvBuf;

        std::atomic<IZ_BOOL> m_isRunning;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_UDP_H__)
