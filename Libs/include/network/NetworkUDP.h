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
        /** �T�[�o�[�Ƃ��ċN��.
         */
        IZ_BOOL startAsServer(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp);

        /** �N���C�A���g�Ƃ��ċN��.
         */
        IZ_BOOL startAsClient(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp);

        IZ_BOOL startAsClientAndConnectToServer(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& remoteEp);

        /** ��~.
         */
        void stop();

        IZ_INT wait(
            IZ_UINT sec = 0,
            IZ_UINT usec = 0);

        /** ��M�����f�[�^���擾.
         */
        IZ_INT recieve(
            void* buf,
            IZ_UINT size);

        IZ_INT recieveFrom(
            void* buf,
            IZ_UINT size,
            IPv4Endpoint& remoteEp);

        /** �f�[�^�𑗐M.
         */
        IZ_INT send(const void* data, IZ_UINT size);

        /** �w�肵���ڑ���Ƀf�[�^�𑗐M.
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
