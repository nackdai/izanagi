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
        /** サーバーとして起動.
         */
        IZ_BOOL startAsServer(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp,
            IZ_UINT maxConnections);

        /** クライアントとして起動.
         */
        IZ_BOOL startAsClient(
            IMemoryAllocator* allocator,
            const IPv4Endpoint& hostEp);

        /** サーバーと接続.
         */
        IZ_BOOL connectToServer(const IPv4Endpoint& remoteEp);

        /** 停止.
         */
        void stop();

        /** サーバーとして起動しているときに接続されているリモートの数を取得.
         */
        IZ_UINT getRemoteNum();

        /** サーバーとして起動しているときに指定された接続されているリモートの情報を取得.
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

        /** データを送信.
         */
        IZ_INT sendData(
            const void* data, 
            IZ_UINT size);

        /** 指定した接続先にデータを送信.
         */
        IZ_INT sendData(
            const void* data,
            IZ_UINT size,
            const IPv4Endpoint& remoteEp);

        /** 全ての接続先にデータを送信.
         */
        IZ_UINT sendDataToAllRemote(const void* data, IZ_UINT size);

        /** サーバーと接続されているかどうか.
         */
        IZ_BOOL isConnected();

    protected:
        TcpRemote* findRemote(const IPv4Endpoint ep);

        virtual void onStop() {}

    protected:
        IMemoryAllocator* m_allocator;

        IZ_SOCKET m_socket;

        // 接続元リスト.
        std::mutex m_remotesLocker;
        CArray<TcpRemote> m_remotes;

        std::atomic<IZ_BOOL> m_isRunnning;
        std::atomic<IZ_BOOL> m_isConnected;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_H__)
