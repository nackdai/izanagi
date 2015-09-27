#include "network/rudp/ReliableUDP.h"
#include "network/rudp/segment/SynchronousSegment.h"
#include "network/rudp/segment/DataSegment.h"
#include "network/rudp/segment/FinishSegment.h"
#include "network/rudp/segment/ExtendAckSegment.h"
#include "network/NetworkUDP_libuv.h"
#include "network/IPv4Endpoint.h"

#include "izMath.h"

namespace izanagi {
namespace net {
    // コンストラクタ.
    void ReliableUDP::Init(
        IMemoryAllocator* allocator,
        Udp* udp,
        const RUDPParameter& parameter)
    {
        m_allocator = allocator;

        m_Udp = udp;
        m_Parameter = parameter;

        // セグメントの受信は別スレッドで受け取る.
        m_RecieveThread = std::thread(std::bind(&ReliableUDP::ProcRecieve, this));

        // TODO
        //m_RetransWorker = new TimerWorker(ProcRetransmit);
    }

    // 接続.
    void ReliableUDP::Connect(const IPv4Endpoint& ep)
    {
        m_Udp->connectTo(ep);

        m_CurState = State::SYN_SENT;

        math::CMathRand::Init(sys::CEnvironment::GetMilliseconds());
        auto segment = Segment::Create<SynchronousSegment>(
            m_allocator,
            math::CMathRand::GetNext(MAX_SEQUENCE_NUMBER),
            m_Parameter);

        // 同期要求を送る.
        SendAndQueueSegment(segment);

        // 接続が確立されるまで待つ
        m_ConnectEvent.Wait();

        if (m_CurState != State::ESTABLISHED)
        {
            // TODO
            // ここから下は失敗したときの処理
        }
    }

    // データ受信.
    IZ_INT ReliableUDP::Recieve(void* bytes, IZ_UINT size)
    {
        if (IsClosed())
        {
            return -1;
        }

        if (m_InSequenceSegmentList.GetItemNum() == 0)
        {
            return 0;
        }

        IZ_INT totalSize = 0;

        sys::Lock locker(m_InSequenceSegmentList.GetLocker());
        {
            CStdList<Segment>* list = &m_InSequenceSegmentList;

            auto item = list->GetTop();

            while (item != IZ_NULL)
            {
                auto segment = item->GetData();

                auto next = item->GetNext();

                if (segment->IsResetSegment())
                {
                    item->Leave();
                    break;
                }
                else if (segment->IsFinishSegment())
                {
                    if (totalSize <= 0)
                    {
                        item->Leave();
                        totalSize = -1;
                    }
                    break;
                }
                else if (segment->IsDataSegment());
                {
                    auto s = (DataSegment*)segment;

                    if (s->Size() + totalSize > size)
                    {
                        IZ_ASSERT(totalSize > 0);
                        break;
                    }

                    IZ_UINT8* dst = (IZ_UINT8*)bytes;

                    memcpy(
                        dst + totalSize,
                        s->Data(),
                        s->Size());

                    totalSize += s->Size();

                    item->Leave();
                }

                item = next;
            }
        }

        return totalSize;
    }

    // データ送信.
    IZ_BOOL ReliableUDP::Send(void* bytes, IZ_UINT offset, IZ_UINT length)
    {
        if (IsClosed())
        {
            return IZ_FALSE;
        }

        IZ_INT totalBytes = 0;

        while (totalBytes < length)
        {
            IZ_INT sendBytes = IZ_MIN(
                m_Parameter.MaxSegmentSize() - Segment::RUDP_HEADER_LEN,
                length - totalBytes);

            IZ_ASSERT(sendBytes > 0);

            // NOTE
            // AcknowledgementSegment は受信したあとにハンドリングしないので
            // AcknowledgementSegment からのシーケンス番号は更新されない.
            // そのため、Listner - Client でシーケンス番号に齟齬が生じる.
            // そこで、DataSegment や FinishSegment などを送るときにシーケンス番号をリセットするように
            // LastInSequenceNumber ではなく、独自に管理しているシーケンス番号を使うようにする.

            auto segment = DataSegment::Create(
                m_allocator,
                m_Counter.NextSequenceNumber(),
                m_Counter.GetLastInSequenceNumber(),
                (IZ_UINT8*)bytes,
                offset,
                length);

            SendAndQueueSegment(segment);

            totalBytes += length;
        }

        return IZ_TRUE;
    }

    // 終了.
    void ReliableUDP::Close()
    {
        if (IsClosed())
        {
            return;
        }

        switch (m_CurState)
        {
            case State::SYN_SENT:
                // 接続待ちを起こす.
                m_ConnectEvent.Set();
                break;
            case State::SYN_RECV:
            case State::ESTABLISHED:
                // NOTE
                // AcknowledgementSegment は受信したあとにハンドリングしないので
                // AcknowledgementSegment からのシーケンス番号は更新されない.
                // そのため、Listner - Client でシーケンス番号に齟齬が生じる.
                // そこで、DataSegment や FinishSegment などを送るときにシーケンス番号をリセットするように
                // LastInSequenceNumber ではなく、独自に管理しているシーケンス番号を使うようにする.
            {
                auto segment = Segment::Create<FinishSegment>(
                    m_allocator,
                    m_Counter.NextSequenceNumber());

                SendSegment(segment);
            }
                // TODO
                // スレッドなどを止める.
                // 現状は recv がブロッキングされるので、スレッドを止められない.
                break;
            default:
                break;
        }

        m_CurState = State::CLOSED;

        m_VacantUnAckedSentSegListWait.Set();
        m_NotEmptyUnAckedSentSegListWait.Set();
        m_InSequenceSegWait.Set();
    }

    // セグメントの送信と返事待ちセグメントキューへの登録.
    void ReliableUDP::SendAndQueueSegment(Segment* segment)
    {
        while (m_UnAckedSentSegmentList.GetItemNum() >= SendQueueSize
            || m_Counter.GetOutStandingSegmentCount() >= m_Parameter.MaxNumberOfOutStandingSegs)
        {
            m_VacantUnAckedSentSegListWait.Wait();
        }

        // TODO
        // UnAckedSentSegmentListの数で代用できないか？
        // 返事待ちセグメントの数を増やす.
        m_Counter.IncrementOutStandingSegmentCount();

        // 返事のない送信済み（返事待ち）セグメントリストに登録.
        m_UnAckedSentSegmentList.Add(segment);

        // UnAckedSentSegmentListが空でなくなったので待機イベントに通知.
        m_NotEmptyUnAckedSentSegListWait.Set();

        // TODO
        // 返事のない送信済み（返事待ち）セグメントについて再送するためのスレッドを起こす.

        // セグメント送信.
        SendSegment(segment);

        // データセグメントの場合はリスナーにパケットを送信したことを通知する.
        if (segment->IsDataSegment())
        {
            OnSentPacket();
        }
    }

    // セグメント送信.
    void ReliableUDP::SendSegment(Segment* segment)
    {
        // ACK(返事)の抱き合わせ設定
        if (segment->IsDataSegment()
            || segment->IsResetSegment()
            || segment->IsFinishSegment()
            || segment->IsNullSegment())
        {
            // 受信したけど返事をしていないセグメントの有無を確認.
            if (m_UnAckedRecievedSegmentList.GetItemNum() > 0)
            {
                // 返事待ちセグメントが存在すれば、ACK（返事）フラグをセグメントに設定する.
                // どの番号に対応するACK（返事）なのかをセット.
                segment->SetAcknowledgedNumber(m_Counter.GetLastInSequenceNumber());
            }
        }

        if (segment->IsDataSegment()
            || segment->IsResetSegment()
            || segment->IsFinishSegment())
        {
            // TODO
            // Reset null segment timer
        }

        // 送信.
        OnSendSegment(segment);
    }

    // セグメント受信.
    Segment* ReliableUDP::RecieveSegment()
    {
        Segment* segment = nullptr;

        if ((segment = OnRecieveSegment()) != nullptr)
        {
            if (segment->IsDataSegment()
                || segment->IsNullSegment()
                || segment->IsResetSegment()
                || segment->IsFinishSegment()
                || segment->IsSynchronousSegment())
            {
                // 確認応答を送っていないセグメントリストに登録.
                m_UnAckedRecievedSegmentList.AddTail(segment->GetListItem());
            }

            // TODO
            // 受信したということは接続がまだ生きているので殺されないようにリセットする.
        }

        return segment;
    }

    // セグメントを受信して、セグメントのタイプに応じて処理を振り分ける.
    void ReliableUDP::ProcRecieve()
    {
        while (!WillClose())
        {
            auto segment = RecieveSegment();

            if (segment)
            {
                if (segment->IsSynchronousSegment())
                {
                    HandleSYNSegment((SynchronousSegment*)segment);
                }
                else if (segment->IsExtendAckSegment())
                {
                    HandleEAKSegment((ExtendAckSegment*)segment);
                }
                else if (segment->IsAcknowledgementSegment())
                {
                    // なにもしない.
                }
                else
                {
                    HandleSegment(segment);
                }

                CheckAndGetAck(segment);
            }

            sys::CThread::YieldThread();
        }
    }

    void ReliableUDP::CheckAndGetAck(Segment* segment)
    {
        auto ackNumber = segment->GetAcknowledgedNumber();

        if (ackNumber < 0)
        {
            return;
        }

        if (m_CurState == State::SYN_RECV)
        {
            OpenConnection();

            // 接続要求を受信した返事に対する返事なので、接続を開く.
            m_CurState = State::ESTABLISHED;
        }

        m_UnAckedSentSegmentList.ForeachWithRemoving(
            (Segment s) =>
            {
                return (CompareSequenceNumbers(s.SequenceNumber, ackNumber) <= 0);
            });

        m_VacantUnAckedSentSegListWait.Set();
    }

    // 受信した接続要求(SYNchronous)セグメントを処理する.
    void ReliableUDP::HandleSYNSegment(SynchronousSegment* segment)
    {
        switch (m_CurState)
        {
            case State::NONE:  // 未接続.
                m_Counter.SetLastInSequenceNumber(segment->SequenceNumber());

                m_CurState = State::SYN_RECV;

                // NOTE
                // State::NONE 状態で SynchronousSegment を受信するのは Listener なので
                // SynchronousSegment を送信し返すタイミングでシーケンス番号を新規設定する？
                // でも、もらった番号をそのまま使えばいい気もするが・・・

                math::CMathRand::Init(sys::CEnvironment::GetMilliseconds());
                auto syncSegment = Segment::Create<SynchronousSegment>(
                    m_allocator,
                    m_Counter.SetSequenceNumber(math::CMathRand::GetNext(MAX_SEQUENCE_NUMBER)),
                    m_Parameter);

                syncSegment->SetAcknowledgedNumber(segment->SequenceNumber());

                SendAndQueueSegment(syncSegment);

                m_UnAckedRecievedSegmentList.Remove(segment);

                break;
            case State::SYN_SENT:    // 接続要求送信済み.
                m_Counter.SetLastInSequenceNumber(segment->SequenceNumber());

                // 返事を返す.
                SendAcknowledgement(segment);

                // 接続確立.
                OpenConnection();

                break;
        }
    }
#if 0

    private void HandleEAKSegment(ExtendAckSegment segment)
    {
        var ackNumbers = segment.AckNumbers;

        int lastInSequence = segment.GetAcknowledgedNumber();
        int lastOutSequence = ackNumbers[ackNumbers.Length - 1];

        // 送信したけど応答がないセグメントリストに対象となるものがないか調べる.
        lock (m_UnAckedSentSegmentList)
        {
            List<Segment> willRemoveList = new List<Segment>();

            for (int i = 0; i < m_UnAckedSentSegmentList.Count; i++)
            {
                var s = m_UnAckedSentSegmentList[i];

                if (CompareSequenceNumbers(s.SequenceNumber, lastInSequence) <= 0)
                {
                    // シーケンス内に入っているので、どこかで応答を受けたはず.
                    willRemoveList.Add(s);
                    continue;
                }

                for (int n = 0; n < ackNumbers.Length; n++)
                {
                    if (CompareSequenceNumbers(s.SequenceNumber, ackNumbers[n]) == 0)
                    {
                        willRemoveList.Add(s);
                        break;
                    }
                }
            }

            foreach (var s in willRemoveList)
            {
                m_UnAckedSentSegmentList.Remove(s);
            }

            // 残ったのものは再送する.
            for (int i = 0; i < m_UnAckedSentSegmentList.Count; i++)
            {
                var s = m_UnAckedSentSegmentList[i];

                if (CompareSequenceNumbers(lastInSequence, s.SequenceNumber) < 0
                    && CompareSequenceNumbers(lastOutSequence, s.SequenceNumber) > 0)
                {
                    // 再送.
                    RetransmitSegment(s);
                }
            }
        }
    }

    /// <summary>
    /// 受信したセグメントを処理.
    /// </summary>
    /// <param name="segment"></param>
    private void HandleSegment(Segment segment)
    {
        // リセットセグメントがきたので通信をリセットする.
        if (segment is ResetSegment)
        {
            // TODO
        }

        // これ以上なにも送られてこないことが通知されてきた.
        if (segment is FinishSegment)
        {
            switch((State)m_CurState)
            {
                case State::SYN_SENT:
                    // 同期セグメント送信状態の場合は、返事待ちしているところ（connect）を解除して先に進むようにする.
                    m_ConnectEvent.Set();
                    break;
                case State::CLOSED:
                    // すでに終了しているので何もしない.
                    break;
                default:
                    m_CurState::Store(State::WILL_CLOSE);
                    break;
            }
        }

        // シーケンス番号が正しいかどうか.
        bool isInSequence = false;

        lock (m_RecieveQueueLocker)
        {
            // 期待するシーケンス番号.
            var nextSeqNumber = NextSequenceNumber(Counter.GetLastInSequenceNumber());

            if (CompareSequenceNumbers(segment.SequenceNumber, Counter.GetLastInSequenceNumber()) >= 0)
            {
                // 送信したものと同じもの or 番号が小さいものがきた(すでに届いているものがきた)ので無視する.
            }
            else if (CompareSequenceNumbers(segment.SequenceNumber, nextSeqNumber) == 0)
            {
                // 期待する番号がきた.
                isInSequence = true;

                if (m_InSequenceSegmentList.Count + m_OutSequenceSegmentList.Count < MaxRecvListSize)
                {
                    // キューに余裕がある.

                    // シーケンス番号を更新.
                    m_Counter.SetLastInSequenceNumber(segment.SequenceNumber);

                    if (segment is DataSegment
                        || segment is ResetSegment
                        || segment is FinishSegment)
                    {
                        // read で読み込むセグメントはリストに登録.
                        m_InSequenceSegmentList.Add(segment);
                        InSequenceSegWait.Set();
                    }

#if false
                    // 順番通りにデータが来たことを通知.
                    if (segment is DataSegment)
                    {
                        OnRecivedPacketInOrder(segment as DataSegment);
                    }
#endif

                    // シーケンス番号が変わったので、out-of-sequenceリストから正しい順序のものを InSequenceSegmentQueue に移す.
                    UpdateOutSequeceQueue();
                }
                else
                {
                    // リストがいっぱいなので、何もしない.
                }
            }
            else
            {
                // シーケンス番号が順番がずれた（パケット順序がおかしくなった）ので、out-of-sequenceリストに登録する.

                if (m_InSequenceSegmentList.Count + m_OutSequenceSegmentList.Count < MaxRecvListSize)
                {
                    bool added = false;

                    for (int i = 0; i < m_OutSequenceSegmentList.Count; i++)
                    {
                        var s = m_OutSequenceSegmentList.Values[i];

                        if (CompareSequenceNumbers(segment.SequenceNumber, s.SequenceNumber) == 0)
                        {
                            // 登録済み.
                            added = true;
                            break;
                        }
                    }

                    if (!added)
                    {
                        // 未登録なので、登録する.
                        m_OutSequenceSegmentList.Add(segment.SequenceNumber, segment);
                    }

#if false
                    // 順序がずれたパケットを受けたことを通知する.
                    if (segment is DataSegment)
                    {
                        OnRecivedPacketOutOfOrder(segment as DataSegment);
                    }
#endif
                }
            }
                
            if (isInSequence && (segment is DataSegment
                                || segment is NullSegment
                                || segment is FinishSegment)
                )
            {
                // 受信した応答を返す.
                SendAcknowledgement(segment);
            }
            else if (m_OutSequenceSegmentList.Count > m_Parameter.MaxOutOfSequence)
            //else if (m_OutSequenceSegmentList.Count > 0)
            {
                // out-of-sequenceセグメントの数が許容最大値を超えた.
                SendExtendAcknowledgement();
            }
            else if (m_UnAckedRecievedSegmentList.Count > m_Parameter.MaxCumulativeAck)
            {
                // 受信したけど確認応答を送っていないセグメントの数が許容最大値を超えた.
                SendSingleAcknowledgement(segment);
            }
            else
            {
                // TODO
                // 上記以外の場合は、定期的にACKセグメント送る.
            }
        }
    }

    /// <summary>
    /// シーケンス番号をチェックして、out-of-sequenceリストから正しい順序のものを InSequenceSegmentQueue に移す.
    /// </summary>
    private void UpdateOutSequeceQueue()
    {
        lock(m_RecieveQueueLocker)
        {
            // 削除リスト.
            List<int> willRemoveList = new List<int>();

            for (int i = 0; i < m_OutSequenceSegmentList.Count; i++)
            {
                var segment = m_OutSequenceSegmentList.Values[i];

                // 期待するシーケンス番号.
                var nextSequenceNumber = NextSequenceNumber(m_Counter.GetLastInSequenceNumber());

                // シーケンス番号が更新されたことで、正しい番号になる可能性があるものを探す.
                if (CompareSequenceNumbers(segment.SequenceNumber, nextSequenceNumber) == 0)
                {
                    // シーケンス番号を更新
                    m_Counter.SetLastInSequenceNumber(segment.SequenceNumber);

                    if (segment is DataSegment
                        || segment is ResetSegment
                        || segment is FinishSegment)
                    {
                        // シーケンス処理内セグメントリストに登録.
                        m_InSequenceSegmentList.Add(segment);

                        m_InSequenceSegWait.Set();
                    }

                    // 削除リストに登録.
                    willRemoveList.Add(m_OutSequenceSegmentList.Keys[i]);
                }
            }

            // out-of-sequenceリストから移動したものを削除.
            foreach(var key in willRemoveList)
            {
                var segment = m_OutSequenceSegmentList[key];

                // 返事をする
                SendSingleAcknowledgement(segment);

                m_OutSequenceSegmentList.Remove(key);
            }
        }
    }

    /// <summary>
    /// シーケンス番号を比較.
    /// </summary>
    /// <param name="s1"></param>
    /// <param name="s2"></param>
    /// <returns>等しければ 0, s1<s2 なら 1, それ以外は -1 を返す. </returns>
    private static int CompareSequenceNumbers(int s1, int s2)
    {
        // NOTE
        // RFC1982
        // http://www5d.biglobe.ne.jp/stssk/rfc/rfc1982j.html

        // シーケンス番号に加算できる数(n)は,
        //  0 <= n <= 2^(BITS - 1) - 1
        // でないといけない.
        // たとえば、BITS = 8 なら、
        //  0 <= n <= 2^7 - 1 (= 127)
        // つまり、n は最大シーケンス番号(2^BITS)の半分から 1 マイナスした値になる.

        //  s1 + n = s2 <-> s2 - s1 = n
        //  0 <= s2 - s1 <= 2^(BITS - 1) - 1
        //  <-> 0 <= s2 - s1 < 2^(BITS - 1)
        // たとえば、BITS = 8 なら,
        //  0 <= s2 - s1 <= 127 <-> 0 <= s2 - s1 < 128
        // つまり、シーケンス番号の差分は最大シーケンス番号(2^BITS)の半分未満でないといけない.

        // たとえば,
        //  s1 + n = s2 -> 200 + 127 = 327
        // ということはありえる.
        // しかし、BITS=8 とすると、s' の値は最大値(255)を超えているため 327 ではなく、71(=327 - 256)となる.
        // この場合だと、
        //  s2 - s1 = 71 - 200 = -129
        // となり、マイナスになってしまう.
        // そのため,
        //  s1 > s2
        // のチェックが必要.また、値は 255 を超えて一周しているので、その差分は,
        //  s1 - s2 > 2^(BITS - 1)
        // となる.
        // たとえば,
        //   s1 +  n  =  s2
        //  128 + 127 = 255 -> s1 < s2
        //  129 + 127 = 256 = 0 -> s1 > s2 -> s1 - s2 = 129 - 0 = 129 > 128 (= 2^7)

        // s１がs２より小さいとは、ｓ１とｓ２が異なりかつ以下が満たされる場合とします.
        // (i1 < i2 and i2 - i1 < 2^(SERIAL_BITS - 1)) or (i1 > i2 and i1 - i2 > 2^(SERIAL_BITS - 1))

        // s１がs２より大きいとは、ｓ１とｓ２が異なりかつ以下が満たされる場合とします.
        // (i1 < i2 and i2 - i1 > 2^(SERIAL_BITS - 1)) or (i1 > i2 and i1 - i2 < 2^(SERIAL_BITS - 1))

        if (s1 == s2)
        {
            return 0;
        }
        else if ((s1 < s2 && (s2 - s1) < MAX_SEQUENCE_NUMBER / 2)
            || (s1 > s2 && (s1 - s2) > MAX_SEQUENCE_NUMBER / 2))
        {
            // less than
            return 1;
        }
        else
        {
            // "greater than" or "invalid".
            return -1;
        }
    }

    /// <summary>
    /// 次のシーケンス番号を取得する.
    /// </summary>
    /// <param name="sequenceNumber"></param>
    /// <returns></returns>
    private static int NextSequenceNumber(int sequenceNumber)
    {
        return (sequenceNumber + 1) % MAX_SEQUENCE_NUMBER;
    }

    /// <summary>
    /// 受信した返事を返す.
    /// </summary>
    /// <param name="segment">返事をする対象のセグメント</param>
    private void SendAcknowledgement(Segment segment)
    {
        lock(m_RecieveQueueLocker)
        {
            // 受信したけど out-of-sequence 状態のセグメントがある場合.
            if (m_OutSequenceSegmentList.Count > 0)
            {
                SendExtendAcknowledgement();
                return;
            }

            SendSingleAcknowledgement(segment);
        }
    }

    /// <summary>
    /// out-of-sequenceセグメントがあったことについて返事をする.
    /// </summary>
    private void SendExtendAcknowledgement()
    {
        lock (m_RecieveQueueLocker)
        {
            // 受信したけど out-of-sequence 状態のセグメントが無いのに呼ばれた.
            if (m_OutSequenceSegmentList.Count == 0)
            {
                return;
            }

            // out-of-sequenceセグメントのシーケンス番号を集める.
            int[] ackNumbers = new int[m_OutSequenceSegmentList.Count];

            for (int i = 0; i < ackNumbers.Length; i++)
            {
                var seqNumber = m_OutSequenceSegmentList.Keys[i];
                var segment = m_OutSequenceSegmentList.Values[i];

                // シーケンス番号チェック
                if (segment.SequenceNumber != seqNumber)
                {
                    throw new ApplicationException();
                }

                ackNumbers[i] = segment.SequenceNumber;

                // 受信したけど返信していないセグメントをリストから削除.
                m_UnAckedRecievedSegmentList.Remove(segment);
            }

            // out-of-sequenceセグメントリストをリセット.
            m_OutSequenceSegmentList.Clear();

            int lastInSequenceNumber = m_Counter.GetLastInSequenceNumber();

            var sendSegment = new ExtendAckSegment(
                NextSequenceNumber(lastInSequenceNumber),
                lastInSequenceNumber,
                ackNumbers);

            // 送信.
            SendSegment(sendSegment);
        }
    }

    /// <summary>
    /// 受信した返事を返す.
    /// </summary>
    /// <param name="segment">返事をする対象のセグメント</param>
    private void SendSingleAcknowledgement(Segment segment)
    {
        // 受信したけど返信していないセグメントの数が 0 のときは何もしない.
        if (m_UnAckedRecievedSegmentList.Count == 0)
        {
            return;
        }

#if false
        var lastInSequence = m_Counter.GetLastInSequenceNumber();

        var sendSegment = new AcknowledgementSegment(
            NextSequenceNumber(lastInSequence),
            lastInSequence);
#else
        var sendSegment = new AcknowledgementSegment(
            NextSequenceNumber(segment.SequenceNumber),
            segment.SequenceNumber);
#endif

        // 送信.
        SendSegment(sendSegment);

        if (segment != null)
        {
            // NOTE
            // 順番が入れ替わった場合もあるのでここでシーケンス番号チェックはできない

            // 受信したが確認応答を送っていないセグメントのリストから削除.
            m_UnAckedRecievedSegmentList.Remove(segment);
        }
    }

    /// <summary>
    /// 接続確立.
    /// </summary>
    private void OpenConnection()
    {
        if (m_CurState == State::ESTABLISHED)
        {
            // TODO
            // すでに接続確立されている場合.
        }
        else
        {
            m_CurState::Store(State::ESTABLISHED);

            m_ConnectEvent.Set();

            OnOpenConnection();
        }
    }

    /// <summary>
    /// セグメントを再送.
    /// </summary>
    /// <param name="segment"></param>
    private void RetransmitSegment(Segment segment)
    {
        // 再送回数を増やす.
        segment.RetryCount++;

        if (m_Parameter.MaxRetransmission != 0
            && segment.RetryCount > m_Parameter.MaxRetransmission)
        {
            // TODO
            // 最大再送回数を超えた場合は接続が失敗していることにする.
            return;
        }

        Console.Write("RetransmitSegment --- ");
        SendSegment(segment);
    }

    private void ProcRetransmit()
    {
        if (m_UnAckedRecievedSegmentList.IsEmpty())
        {
            m_NotEmptyUnAckedSentSegListWait.WaitOne();
            m_NotEmptyUnAckedSentSegListWait.Reset();
        }
            
        // 返事を受けていないセグメントを再送.
        m_UnAckedSentSegmentList.Foreach(m_RetransmitSegment);
    }

    /// <summary>
    /// セグメント受信実装.
    /// </summary>
    /// <returns></returns>
    protected virtual Segment OnRecieveSegment()
    {
        IPEndPoint remoteEp = new IPEndPoint(IPAddress.Any, 0);

        var ret = OnRecieveSegment(ref remoteEp);
        return ret;
    }

    /// <summary>
    /// セグメント受信実装.
    /// </summary>
    /// <param name="remote"></param>
    /// <returns></returns>
    protected virtual Segment OnRecieveSegment(ref IPEndPoint remote)
    {
        var data = m_Udp.Receive(ref remote);
        var segment = Segment.Parse(data, 0, data.Length);

        Console.WriteLine("Recieve Segment [{0}]", segment.ToString());

        return segment;
    }

#if false
    static bool isValid = false;

    /// <summary>
    /// セグメント送信実装.
    /// </summary>
    /// <param name="segment"></param>
    protected virtual void OnSendSegment(Segment segment)
    {
        // 疑似パケットロス発生

        if (isValid)
        {
            using (var ws = new MemoryStream())
            {
                segment.WriteBytes(ws);
                var bytes = ws.GetBuffer();
                var length = ws.Position;
                m_Udp.Send(bytes, (int)length);
            }
        }
        else
        {
            // TODO
            isValid = true;
        }
    }
#elif false
    Queue<Segment> TempQueue = new Queue<Segment>();
    static bool isValid = false;

    /// <summary>
    /// セグメント送信実装.
    /// </summary>
    /// <param name="segment"></param>
    protected virtual void OnSendSegment(Segment segment)
    {
        // 疑似パケット順番入れ替え

        if (!m_IsConnected || segment is AcknowledgementSegment)
        {
            using (var ws = new MemoryStream())
            {
                Console.WriteLine("Send Segment [{0}]", segment.ToString());
                segment.WriteBytes(ws);
                var bytes = ws.GetBuffer();
                var length = ws.Position;
                m_Udp.Send(bytes, (int)length);
            }
        }
        else
        {
            if (isValid)
            {
                using (var ws = new MemoryStream())
                {
                    {
                        Console.WriteLine("Send Segment [{0}]", segment.ToString());
                        segment.WriteBytes(ws);
                        var bytes = ws.GetBuffer();
                        var length = ws.Position;
                        m_Udp.Send(bytes, (int)length);
                    }
                }

                using (var ws = new MemoryStream())
                {
                    if (TempQueue.Count > 0)
                    {
                        var s = TempQueue.Dequeue();
                        Console.WriteLine("Send Segment [{0}]", s.ToString());
                        s.WriteBytes(ws);
                        var bytes = ws.GetBuffer();
                        var length = ws.Position;
                        m_Udp.Send(bytes, (int)length);
                    }
                }
            }
            else
            {
                isValid = true;
                TempQueue.Enqueue(segment);
            }
        }
    }
#else
    /// <summary>
    /// セグメント送信実装.
    /// </summary>
    /// <param name="segment"></param>
    protected virtual void OnSendSegment(Segment segment)
    {
        Console.WriteLine("Send Segment [{0}]", segment.ToString());

        using (var ws = new MemoryStream())
        {
            segment.WriteBytes(ws);
            var bytes = ws.GetBuffer();
            var length = ws.Position;
            m_Udp.Send(bytes, (int)length);
        }
    }
#endif

    protected virtual void OnOpenConnection()
    {
        // 継承先で実装.
    }

    protected virtual void OnSentPacket()
    {
        // 継承先で実装.
    }
#endif
}   // namespace net
}   // namespace izanagi
