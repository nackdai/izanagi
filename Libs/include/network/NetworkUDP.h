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
        /** 起動.
         */
        IZ_BOOL start(const IPv4Endpoint& hostEp);

        /** 起動.
         */
        IZ_BOOL start();

        /** 接続.
         */
        IZ_BOOL connectTo(const IPv4Endpoint& remoteEp);

        /** 停止.
         */
        void stop();

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
        IZ_INT sendData(const void* data, IZ_UINT size);

        /** 指定した接続先にデータを送信.
         */
        IZ_INT sendTo(
            const void* data,
            IZ_UINT size,
            IPv4Endpoint& remoteEp);

        IZ_INT wait(
            IZ_UINT sec = 0,
            IZ_UINT usec = 0);

        void beginRecieve(
            std::function<void(const Packet*)> onRecieve,
            void* buf,
            IZ_UINT size);

        void endRecieve();

    protected:
        IZ_SOCKET m_socket{ IZ_INVALID_SOCKET };

        IZ_BOOL m_isBindAddr{ IZ_FALSE };
        IPv4Endpoint m_host;

        std::atomic<IZ_BOOL> m_isRunning{ IZ_FALSE };
        std::thread m_recvThread;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_UDP_H__)
