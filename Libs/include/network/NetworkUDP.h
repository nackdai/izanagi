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

        /** クライアントとして起動.
         */
        IZ_BOOL startAsClient(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint);

        /** 停止.
         */
        void stop();

        /** 受信したデータを取得.
        */
        IZ_BOOL recieve(std::function<void(const net::Packet&)> func);

        /** 受信した全データを取得.
        */
        IZ_BOOL recieveAll(std::function<void(const net::Packet&)> func);

        /** データを送信.
        */
        IZ_BOOL sendData(const void* data, IZ_UINT size);

        /** 指定した接続先にデータを送信.
        */
        IZ_BOOL sendData(
            const IPv4Endpoint& endpoint,
            const void* data, IZ_UINT size);

        IZ_BOOL run(IZ_CHAR* recvBuf, IZ_UINT size);

    private:
        UdpRemote* findRemote(const IPv4Endpoint& ep);
        UdpRemote* findRemote(const sockaddr_in& addr);

        void traverseRemotes(std::function<IZ_BOOL(UdpRemote*)> func);

    protected:
        void loop();

        virtual void onStop() {}

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
