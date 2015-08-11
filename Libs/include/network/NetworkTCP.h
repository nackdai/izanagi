#if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
#define _IZANAGI_NETWORK_NETWORK_TCP_H__

#include "izDefs.h"
#include "izSystem.h"
#include "network/NetworkDefs.h"
#include "network/NetworkPacket.h"

namespace izanagi {
namespace net {
    class Remote;

    /**
     */
    class Tcp {
    public:
        Tcp();
        virtual ~Tcp();

        NO_COPIABLE(Tcp);

    public:
        /** サーバーとして起動.
         */
        IZ_BOOL startAsServer(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint,
            IZ_UINT maxConnections);

        /** クライアントとして起動.
         */
        IZ_BOOL startAsClient(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& endpoint);

        /** 停止.
         */
        void stop();

        /** サーバーとして起動しているときに接続されているリモートの数を取得.
         */
        IZ_UINT getRemoteNum() const;

        /** サーバーとして起動しているときに指定された接続されているリモートの情報を取得.
         */
        const IPv4Endpoint* getRemote(IZ_UINT idx) const;

        /** 受信したデータを取得.
         */
        IZ_BOOL recieve(std::function<void(const net::Packet&)> func);

        IZ_BOOL sendData(const void* data, IZ_UINT size);

        IZ_BOOL sendData(
            const IPv4Endpoint& endpoint,
            const void* data, IZ_UINT size);

        IZ_UINT sendDataToAllRemote(const void* data, IZ_UINT size);

        IZ_BOOL connectServer();

        IZ_BOOL run(IZ_CHAR* recvBuf, IZ_UINT size);

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

        CArray<Remote> m_clients;

        std::mutex m_recvDataLocker;
        CStdList<Packet> m_recvData;

        std::atomic<IZ_BOOL> m_isRunnning;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
