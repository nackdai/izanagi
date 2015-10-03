#if !defined(_IZANAGI_NETWORK_RUDP_LISTENER_H__)
#define _IZANAGI_NETWORK_RUDP_LISTENER_H__

#include "network/IPv4Endpoint.h"
#include "network/rudp/ReliableUDP.h"

namespace izanagi {
namespace net {
    class Udp;

    class ReliableUDPListener : public ReliableUDP
    {
    public:
        ReliableUDPListener() {}

        virtual ~ReliableUDPListener() {}

        NO_COPIABLE(ReliableUDPListener);

    public:
        void Init(
            IMemoryAllocator* allocator,
            Udp* udp);

        ReliableUDP* Accept();

    private:
        void ProcRecieve();

        Udp* GetUdp();

    private:
        static const IZ_UINT RecievedClientMapNum = 4;

        class ReliableUDPClient;

        using AcceptedClientList = CStdList < ReliableUDPClient >;
        using RecievedClientMap = CStdHash < IPv4Endpoint, ReliableUDPClient, RecievedClientMapNum > ;

        class ReliableUDPClient : public ReliableUDP, sys::CSpinLock
        {
            IZ_DECL_PLACEMENT_NEW();

        public:
            static ReliableUDPClient* Create(
                IMemoryAllocator* allocator,
                ReliableUDPListener* server,
                const IPv4Endpoint& ep);

        private:
            ReliableUDPClient(
                ReliableUDPListener* server, 
                const IPv4Endpoint& ep);

        public:
            void EnqueueSegment(Segment* segment);

            AcceptedClientList::Item* GetListItem()
            {
                return &m_listItem;
            }

            RecievedClientMap::Item* GetHashItem()
            {
                return &m_hashItem;
            }

        private:
            virtual void OnOpenConnection() override;

            virtual Segment* OnRecieveSegment() override;

            // セグメント送信実装.
            virtual void OnSendSegment(Segment* segment) override;

        private:
            IPv4Endpoint m_RemoteEp;

            ReliableUDPListener* m_Parent;

            CStdList<Segment> m_RecievedSegmentQueue;

            AcceptedClientList::Item m_listItem;
            RecievedClientMap::Item m_hashItem;
        };

    private:
        sys::CEvent m_AcceptWaitEvent;

        std::mutex m_AcceptedClientListWaiter;

        // コネクションされたクライアントのリスト.
        AcceptedClientList m_AcceptedClientList;

        // 受信したクライアントのリスト.
        RecievedClientMap m_RecievedClientMap;

        std::thread m_RecieveThread;
    };
}   // namespace net
}   // namespace izanagi

#endif  // #if !defined(_IZANAGI_NETWORK_RUDP_LISTENER_H__)

