#if !defined(_IZANAGI_NETWORK_RUDP_H__)
#define _IZANAGI_NETWORK_RUDP_H__

#include <atomic>
#include <thread>
#include "izDefs.h"
#include "izStd.h"
#include "izSystem.h"
#include "network/rudp/RUDPParameter.h"
#include "network/rudp/RUDPCounter.h"

namespace izanagi {
namespace net {
    class Udp;
    class IPv4Endpoint;
    class Segment;
    class SynchronousSegment;
    class ExtendAckSegment;

    class ReliableUDP
    {
    public:
        enum State : IZ_UINT
        {
            NONE,           // 未接続.
            CLOSED,         // 終了.
            SYN_SENT,       // 接続要求送信済み.
            SYN_RECV,       // 接続要求受信済み.
            ESTABLISHED,    // 通信確立.
            WILL_CLOSE,     // 終了処理中.
        };

    public:
        ReliableUDP()
        {
        }

        void Init(
            IMemoryAllocator* allocator,
            Udp* udp,
            const RUDPParameter& parameter);

        // 接続.
        void Connect(const IPv4Endpoint& ep);

        // データ受信.
        IZ_INT Recieve(void* bytes, IZ_UINT size);

        // データ送信.
        IZ_BOOL Send(void* bytes, IZ_UINT offset, IZ_UINT length);

        // 終了.
        void Close();

    private:
        // セグメントの送信と返事待ちセグメントキューへの登録.
        void SendAndQueueSegment(Segment* segment);

        // セグメント送信.
        void SendSegment(Segment* segment);

        // セグメント受信.
        Segment* RecieveSegment();

        // セグメントを受信して、セグメントのタイプに応じて処理を振り分ける.
        void ProcRecieve();

        void CheckAndGetAck(Segment* segment);

        // 受信した接続要求(SYNchronous)セグメントを処理する.
        void HandleSYNSegment(SynchronousSegment* segment);

        void HandleEAKSegment(ExtendAckSegment* segment);

        // 受信したセグメントを処理.
        void HandleSegment(Segment* segment);

        // シーケンス番号をチェックして、out-of-sequenceリストから正しい順序のものを InSequenceSegmentQueue に移す.
        void UpdateOutSequeceQueue();

        // シーケンス番号を比較.
        // 等しければ 0, s1<s2 なら 1, それ以外は -1 を返す.
        static IZ_INT CompareSequenceNumbers(IZ_INT s1, IZ_INT s2);

        // 次のシーケンス番号を取得する.
        static IZ_INT NextSequenceNumber(IZ_INT sequenceNumber);

        // 受信した返事を返す.
        void SendAcknowledgement(Segment* segment);

        // out-of-sequenceセグメントがあったことについて返事をする.
        void SendExtendAcknowledgement();

        // 受信した返事を返す.
        void SendSingleAcknowledgement(Segment* segment);

        // 接続確立.
        void OpenConnection();

        // セグメントを再送.
        void RetransmitSegment(Segment* segment);

        void ProcRetransmit();

    protected:
        // セグメント受信実装.
        virtual Segment* OnRecieveSegment();

        // セグメント送信実装.
        virtual void OnSendSegment(Segment* segment);

        virtual void OnOpenConnection()
        {
            // 継承先で実装.
        }

        virtual void OnSentPacket()
        {
            // 継承先で実装.
        }

    public:
        IZ_BOOL IsClosed() const
        {
            return m_CurState == State::CLOSED;
        }

        IZ_BOOL WillClose() const
        {
            return m_CurState == State::CLOSED || m_CurState == State::WILL_CLOSE;
        }

        IZ_BOOL IsConnected() const
        {
            return m_CurState == State::ESTABLISHED;
        }

    private:
        // 最大シーケンス番号.
        // セグメントヘッダ内のシーケンス番号は 1byte(=8bit) なので、最大は 255(= 2^8 - 1) になる.
        static const IZ_INT MAX_SEQUENCE_NUMBER = 255;

        // TODO
        // Maximum number of sent segments.
        static const IZ_UINT SendQueueSize = 32;

        // TODO
        // Maximum number of received segments
        static const IZ_UINT MaxRecvListSize = 32;

        IMemoryAllocator* m_allocator{ nullptr };

        // Maximum number of sent segments.
        IZ_UINT m_SendQueueSize{ 32 };

        // Maximum number of received segments
        IZ_UINT m_MaxRecvListSize{ 32 };

        Udp* m_Udp{ nullptr };

        // 現在のステート.
        std::atomic<State> m_CurState{ State::NONE };

        // RUDPパラメータ.
        RUDPParameter m_Parameter;

        RUDPCounter m_Counter;

        sys::CEvent m_ConnectEvent;

        // セグメント受信スレッド.
        std::thread m_RecieveThread;

        // TODO
        // タイマースレッド.
        //TimerWorker RetransWorker;

        // UnAckedSentSegmentListに余裕ができるのを待つ.
        sys::CEvent m_VacantUnAckedSentSegListWait;

        // InSequenceSegmentListにAddされるのを待つ.
        sys::CEvent m_InSequenceSegWait;

        // UnAckedSentSegmentListが空でなくなるのを待つ.
        sys::CEvent m_NotEmptyUnAckedSentSegListWait;

        // 送信したが応答が来ていないセグメントのリスト.
        std::mutex m_UnAckedSentSegmentListLocker;
        CStdList<Segment> m_UnAckedSentSegmentList;

        std::mutex m_RecieveQueueLocker;

        // シーケンス内のセグメントのリスト.
        CStdList<Segment> m_InSequenceSegmentList;

        // シーケンス外のセグメントのリスト.
        CStdOrderedList<Segment> m_OutSequenceSegmentList;

        // 受信したが確認応答を送っていないセグメントのリスト.
        std::mutex m_UnAckedRecievedSegmentListLocker;
        CStdList<Segment> m_UnAckedRecievedSegmentList;

        std::atomic<IZ_BOOL> m_isAllocated{ IZ_FALSE };
        void* m_recvData{ nullptr };
        void* m_sendData{ nullptr };
    };
}   // namespace net
}   // namespace izanagi

#endif  // #if !defined(_IZANAGI_NETWORK_RUDP_H__)
