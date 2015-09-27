#if !defined(_IZANAGI_NETWORK_RUDP_H__)
#define _IZANAGI_NETWORK_RUDP_H__

#include <atomic>
#include <thread>
#include "izDefs.h"
#include "izStd.h"
#include "izSystem.h"
#include "izThreadModel.h"
#include "network/rudp/RUDPParameter.h"
#include "network/rudp/RUDPCounter.h"

namespace izanagi {
namespace net {
    class Udp;
    class IPv4Endpoint;
    class Segment;

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
#if 0

        private void HandleEAKSegment(ExtendAckSegment segment)
        {
            var ackNumbers = segment.AckNumbers;

            int lastInSequence = segment.GetAcknowledgedNumber();
            int lastOutSequence = ackNumbers[ackNumbers.Length - 1];

            // 送信したけど応答がないセグメントリストに対象となるものがないか調べる.
            lock (this.UnAckedSentSegmentList)
            {
                List<Segment> willRemoveList = new List<Segment>();

                for (int i = 0; i < this.UnAckedSentSegmentList.Count; i++)
                {
                    var s = this.UnAckedSentSegmentList[i];

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
                    this.UnAckedSentSegmentList.Remove(s);
                }

                // 残ったのものは再送する.
                for (int i = 0; i < this.UnAckedSentSegmentList.Count; i++)
                {
                    var s = this.UnAckedSentSegmentList[i];

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
                switch((State)this.CurState)
                {
                    case State::SYN_SENT:
                        // 同期セグメント送信状態の場合は、返事待ちしているところ（connect）を解除して先に進むようにする.
                        this.ConnectEvent.Set();
                        break;
                    case State::CLOSED:
                        // すでに終了しているので何もしない.
                        break;
                    default:
                        this.CurState::Store(State::WILL_CLOSE);
                        break;
                }
            }

            // シーケンス番号が正しいかどうか.
            bool isInSequence = false;

            lock (this.RecieveQueueLocker)
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

                    if (this.InSequenceSegmentList.Count + this.OutSequenceSegmentList.Count < MaxRecvListSize)
                    {
                        // キューに余裕がある.

                        // シーケンス番号を更新.
                        this.Counter.SetLastInSequenceNumber(segment.SequenceNumber);

                        if (segment is DataSegment
                            || segment is ResetSegment
                            || segment is FinishSegment)
                        {
                            // read で読み込むセグメントはリストに登録.
                            this.InSequenceSegmentList.Add(segment);
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

                    if (this.InSequenceSegmentList.Count + this.OutSequenceSegmentList.Count < MaxRecvListSize)
                    {
                        bool added = false;

                        for (int i = 0; i < this.OutSequenceSegmentList.Count; i++)
                        {
                            var s = this.OutSequenceSegmentList.Values[i];

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
                            this.OutSequenceSegmentList.Add(segment.SequenceNumber, segment);
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
                else if (this.OutSequenceSegmentList.Count > this.Parameter.MaxOutOfSequence)
                //else if (this.OutSequenceSegmentList.Count > 0)
                {
                    // out-of-sequenceセグメントの数が許容最大値を超えた.
                    SendExtendAcknowledgement();
                }
                else if (this.UnAckedRecievedSegmentList.Count > this.Parameter.MaxCumulativeAck)
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
            lock(this.RecieveQueueLocker)
            {
                // 削除リスト.
                List<int> willRemoveList = new List<int>();

                for (int i = 0; i < this.OutSequenceSegmentList.Count; i++)
                {
                    var segment = this.OutSequenceSegmentList.Values[i];

                    // 期待するシーケンス番号.
                    var nextSequenceNumber = NextSequenceNumber(this.Counter.GetLastInSequenceNumber());

                    // シーケンス番号が更新されたことで、正しい番号になる可能性があるものを探す.
                    if (CompareSequenceNumbers(segment.SequenceNumber, nextSequenceNumber) == 0)
                    {
                        // シーケンス番号を更新
                        this.Counter.SetLastInSequenceNumber(segment.SequenceNumber);

                        if (segment is DataSegment
                            || segment is ResetSegment
                            || segment is FinishSegment)
                        {
                            // シーケンス処理内セグメントリストに登録.
                            this.InSequenceSegmentList.Add(segment);

                            this.InSequenceSegWait.Set();
                        }

                        // 削除リストに登録.
                        willRemoveList.Add(this.OutSequenceSegmentList.Keys[i]);
                    }
                }

                // out-of-sequenceリストから移動したものを削除.
                foreach(var key in willRemoveList)
                {
                    var segment = this.OutSequenceSegmentList[key];

                    // 返事をする
                    SendSingleAcknowledgement(segment);

                    this.OutSequenceSegmentList.Remove(key);
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
            lock(this.RecieveQueueLocker)
            {
                // 受信したけど out-of-sequence 状態のセグメントがある場合.
                if (this.OutSequenceSegmentList.Count > 0)
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
            lock (this.RecieveQueueLocker)
            {
                // 受信したけど out-of-sequence 状態のセグメントが無いのに呼ばれた.
                if (this.OutSequenceSegmentList.Count == 0)
                {
                    return;
                }

                // out-of-sequenceセグメントのシーケンス番号を集める.
                int[] ackNumbers = new int[this.OutSequenceSegmentList.Count];

                for (int i = 0; i < ackNumbers.Length; i++)
                {
                    var seqNumber = this.OutSequenceSegmentList.Keys[i];
                    var segment = this.OutSequenceSegmentList.Values[i];

                    // シーケンス番号チェック
                    if (segment.SequenceNumber != seqNumber)
                    {
                        throw new ApplicationException();
                    }

                    ackNumbers[i] = segment.SequenceNumber;

                    // 受信したけど返信していないセグメントをリストから削除.
                    this.UnAckedRecievedSegmentList.Remove(segment);
                }

                // out-of-sequenceセグメントリストをリセット.
                this.OutSequenceSegmentList.Clear();

                int lastInSequenceNumber = this.Counter.GetLastInSequenceNumber();

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
            if (this.UnAckedRecievedSegmentList.Count == 0)
            {
                return;
            }

#if false
            var lastInSequence = this.Counter.GetLastInSequenceNumber();

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
                this.UnAckedRecievedSegmentList.Remove(segment);
            }
        }

        /// <summary>
        /// 接続確立.
        /// </summary>
        private void OpenConnection()
        {
            if (this.CurState == State::ESTABLISHED)
            {
                // TODO
                // すでに接続確立されている場合.
            }
            else
            {
                this.CurState::Store(State::ESTABLISHED);

                this.ConnectEvent.Set();

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

            if (this.Parameter.MaxRetransmission != 0
                && segment.RetryCount > this.Parameter.MaxRetransmission)
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
            if (this.UnAckedRecievedSegmentList.IsEmpty())
            {
                this.NotEmptyUnAckedSentSegListWait.WaitOne();
                this.NotEmptyUnAckedSentSegListWait.Reset();
            }
            
            // 返事を受けていないセグメントを再送.
            this.UnAckedSentSegmentList.Foreach(this.RetransmitSegment);
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
            var data = this.Udp.Receive(ref remote);
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
                    this.Udp.Send(bytes, (int)length);
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

            if (!this.IsConnected || segment is AcknowledgementSegment)
            {
                using (var ws = new MemoryStream())
                {
                    Console.WriteLine("Send Segment [{0}]", segment.ToString());
                    segment.WriteBytes(ws);
                    var bytes = ws.GetBuffer();
                    var length = ws.Position;
                    this.Udp.Send(bytes, (int)length);
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
                            this.Udp.Send(bytes, (int)length);
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
                            this.Udp.Send(bytes, (int)length);
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
                this.Udp.Send(bytes, (int)length);
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
        threadmodel::ThreadSafeList<Segment> m_UnAckedSentSegmentList;

        std::mutex m_RecieveQueueLocker;

        // シーケンス内のセグメントのリスト.
        threadmodel::ThreadSafeList<Segment> m_InSequenceSegmentList;

        // シーケンス外のセグメントのリスト.
        threadmodel::ThreadSafeOrderedList<Segment> m_OutSequenceSegmentList;

        // 受信したが確認応答を送っていないセグメントのリスト.
        threadmodel::ThreadSafeList<Segment> m_UnAckedRecievedSegmentList;
    };
}   // namespace net
}   // namespace izanagi

#endif  // #if !defined(_IZANAGI_NETWORK_RUDP_H__)
