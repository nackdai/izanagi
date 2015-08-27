#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"

namespace izanagi {
namespace net {
    class TcpRemote;

    /** TCP.
     */
    class Tcp {
    public:
        Tcp();
        virtual ~Tcp();

        NO_COPIABLE(Tcp);

    public:
        /** �T�[�o�[�Ƃ��ċN��.
         */
        IZ_BOOL startAsServer(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp,
            IZ_UINT maxConnections);

        /** �N���C�A���g�Ƃ��ċN��.
         */
        IZ_BOOL startAsClient(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp);

        /** �T�[�o�[�Ɛڑ�.
         */
        IZ_BOOL connectToServer(const IPv4Endpoint& remoteEp);

        /** ��~.
         */
        void stop();

        /** �T�[�o�[�Ƃ��ċN�����Ă���Ƃ��ɐڑ�����Ă��郊���[�g�̐����擾.
         */
        IZ_UINT getRemoteNum();

        /** �T�[�o�[�Ƃ��ċN�����Ă���Ƃ��Ɏw�肳�ꂽ�ڑ�����Ă��郊���[�g�̏����擾.
         */
        const IPv4Endpoint* getRemote(IZ_UINT idx);

        const IPv4Endpoint* acceptRemote();

        IZ_BOOL isEnabledRemote(const IPv4Endpoint& remoteEp);

        IZ_INT wait(
            IZ_UINT sec = 0,
            IZ_UINT usec = 0);

        IZ_INT recieveFrom(
            IZ_UINT8* buf,
            IZ_UINT size,
            const IPv4Endpoint& remoteEp);

        /** �f�[�^�𑗐M.
         */
        IZ_INT sendData(
            const void* data, 
            IZ_UINT size);

        /** �w�肵���ڑ���Ƀf�[�^�𑗐M.
         */
        IZ_INT sendData(
            const void* data,
            IZ_UINT size,
            const IPv4Endpoint& remoteEp);

        /** �S�Ă̐ڑ���Ƀf�[�^�𑗐M.
         */
        IZ_UINT sendDataToAllRemote(const void* data, IZ_UINT size);

        /** �T�[�o�[�Ɛڑ�����Ă��邩�ǂ���.
         */
        IZ_BOOL isConnected();

    protected:
        TcpRemote* findRemote(const IPv4Endpoint ep);

        virtual void onStop() {}

    protected:
        IMemoryAllocator* m_allocator;

        IZ_SOCKET m_socket;

        // �ڑ������X�g.
        std::mutex m_remotesLocker;
        CArray<TcpRemote> m_remotes;

        std::atomic<IZ_BOOL> m_isRunnning;
        std::atomic<IZ_BOOL> m_isConnected;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
