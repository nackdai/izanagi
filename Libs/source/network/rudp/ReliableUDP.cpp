#include "network/rudp/ReliableUDP.h"
#include "network/rudp/segment/SynchronousSegment.h"
#include "network/rudp/segment/DataSegment.h"
#include "network/rudp/segment/FinishSegment.h"
#include "network/rudp/segment/ExtendAckSegment.h"
#include "network/rudp/segment/AcknowledgementSegment.h"
#include "network/NetworkUDP_libuv.h"
#include "network/IPv4Endpoint.h"

#include "izMath.h"

namespace izanagi {
namespace net {
    void ReliableUDP::RetransTask::OnRun(IZ_FLOAT time)
    {
        IZ_ASSERT(m_RUDP);
        m_RUDP->ProcRetransmit();
    }

    ////////////////////////////////////////////////////////

    // コンストラクタ.
    void ReliableUDP::Init(
        IMemoryAllocator* allocator,
        Udp* udp,
        const RUDPParameter& parameter)
    {
        m_allocator = allocator;

        m_Udp = udp;
        m_Parameter = parameter;

        IZ_ASSERT(m_Parameter.MaxSegmentSize > Segment::RUDP_HEADER_LEN);

        // セグメントの受信は別スレッドで受け取る.
        m_RecieveThread = std::thread(std::bind(&ReliableUDP::ProcRecieve, this));

#if 0
        // Fot retransmition.
        auto task = threadmodel::CTimerTask::CreateTask<RetransTask>(allocator);
        IZ_ASSERT(task);

        task->m_RUDP = this;

        // TODO
        m_RetransWorker.PostIntervalTask(
            allocator,
            task,
            100.0f,     // interval.
            IZ_TRUE);   // will delete.
#endif
    }

    // 接続.
    void ReliableUDP::Connect(const IPv4Endpoint& ep)
    {
        m_Udp->connectTo(ep);

        m_CurState = State::SYN_SENT;

        math::CMathRand::Init(sys::CEnvironment::GetMilliseconds());
        auto segment = Segment::Create<SynchronousSegment>(
            m_allocator,
            m_Counter.SetSequenceNumber(math::CMathRand::GetNext(MAX_SEQUENCE_NUMBER)),
            m_Parameter);

        // 同期要求を送る.
        SendAndQueueSegment(segment);

        // TODO
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

        IZ_INT totalSize = 0;

        std::lock_guard<std::mutex> locker(m_RecieveQueueLocker);
        {
            if (m_InSequenceSegmentList.GetItemNum() == 0)
            {
                return 0;
            }

            auto item = m_InSequenceSegmentList.GetTop();

            while (item != IZ_NULL)
            {
                auto segment = item->GetData();

                auto next = item->GetNext();

                if (segment->IsResetSegment())
                {
                    item->Leave();
					Segment::Delete(segment);
                    break;
                }
                else if (segment->IsFinishSegment())
                {
                    if (totalSize <= 0)
                    {
                        item->Leave();
						Segment::Delete(segment);
                        totalSize = -1;
                    }
                    break;
                }
                else if (segment->IsDataSegment())
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
					Segment::Delete(segment);
                }

                item = next;
            }
        }

        return totalSize;
    }

    // データ送信.
    IZ_BOOL ReliableUDP::Send(const void* bytes, IZ_UINT offset, IZ_UINT length)
    {
        if (IsClosed())
        {
            return IZ_FALSE;
        }

        IZ_UINT totalBytes = 0;

        while (totalBytes < length)
        {
            IZ_INT sendBytes = IZ_MIN(
                m_Parameter.MaxSegmentSize - Segment::RUDP_HEADER_LEN,
                length - totalBytes);

            IZ_ASSERT(sendBytes > 0);

            // NOTE
            // AcknowledgementSegment は受信したあとにハンドリングしないので
            // AcknowledgementSegment からのシーケンス番号は更新されない.
            // そのため、Listner - Client でシーケンス番号に齟齬が生じる.
            // そこで、DataSegment や FinishSegment などを送るときにシーケンス番号をリセットするように
            // LastInSequenceNumber ではなく、独自に管理しているシーケンス番号を使うようにする.

            auto segment = Segment::Create<DataSegment>(
                m_allocator,
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
#if 0
                auto segment = Segment::Create<FinishSegment>(
                    m_allocator,
                    m_Counter.NextSequenceNumber());

                SendSegment(segment);
#else
				FinishSegment segment;
				segment.Init(
					m_allocator,
					m_Counter.NextSequenceNumber());

				SendSegment(&segment);
#endif

                m_CurState = State::WILL_CLOSE;
                if (m_RecieveThread.joinable()) {
                    m_RecieveThread.join();
                }
            }
                break;
            default:
                break;
        }

		m_CurState = State::WILL_CLOSE;
		if (m_RecieveThread.joinable()) {
			m_RecieveThread.join();
		}

        m_CurState = State::CLOSED;

        m_RetransWorker.Join();

		Clear();
    }

	// 終了されるまで待つ.
	IZ_BOOL ReliableUDP::WaitForFinish()
	{
		if (m_CurState == State::CLOSED) {
			return IZ_TRUE;
		}

		if (m_RecieveThread.joinable()) {
			m_RecieveThread.join();
		}

		m_CurState = State::CLOSED;

        m_RetransWorker.Join();

		Clear();

		return IZ_TRUE;
	}

	void ReliableUDP::Clear()
	{
		if (m_isAllocated) {
			FREE(m_allocator, m_recvData);
			FREE(m_allocator, m_sendData);
			m_isAllocated = IZ_FALSE;
		}

		auto func = [](Segment* s) {
			if (!s->isBelonged()) {
				Segment::Delete(s);
			}
		};

		// リストに残っているものを処分する.
		m_UnAckedSentSegmentList.ForeachRemove(func);
		m_InSequenceSegmentList.ForeachRemove(func);
		m_OutSequenceSegmentList.ForeachRemove(func);
		m_UnAckedRecievedSegmentList.ForeachRemove(func);

		m_VacantUnAckedSentSegListWait.Set();
		m_NotEmptyUnAckedSentSegListWait.Set();
		m_InSequenceSegWait.Set();
	}

    // セグメントの送信と返事待ちセグメントキューへの登録.
    void ReliableUDP::SendAndQueueSegment(Segment* segment)
    {
        IZ_UINT numUnAckedSentSegment = 0;

        {
            //std::lock_guard<std::mutex> locker(m_UnAckedSentSegmentListLocker);
            m_UnAckedSentSegmentListLocker.safeExec([&](){
                numUnAckedSentSegment = m_UnAckedSentSegmentList.GetItemNum();
            });
        }

        while (numUnAckedSentSegment >= SendQueueSize
            || m_Counter.GetOutStandingSegmentCount() >= m_Parameter.MaxNumberOfOutStandingSegs)
        {
            m_VacantUnAckedSentSegListWait.Wait();
        }

        // TODO
        // UnAckedSentSegmentListの数で代用できないか？
        // 返事待ちセグメントの数を増やす.
        m_Counter.IncrementOutStandingSegmentCount();

        // 返事のない送信済み（返事待ち）セグメントリストに登録.
        {
            //std::lock_guard<std::mutex> locker(m_UnAckedSentSegmentListLocker);
            m_UnAckedSentSegmentListLocker.safeExec([&]() {
                m_UnAckedSentSegmentList.AddTail(segment->GetListItem(ListItemType::UnAcked));
            });
        }

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
            auto numUnAckedRecievedSegment = 0;

            {
                std::lock_guard<std::mutex> locker(m_UnAckedRecievedSegmentListLocker);
                numUnAckedRecievedSegment = m_UnAckedRecievedSegmentList.GetItemNum();
            }

            // 受信したけど返事をしていないセグメントの有無を確認.
            if (numUnAckedRecievedSegment > 0)
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
                std::lock_guard<std::mutex> locker(m_UnAckedRecievedSegmentListLocker);
                m_UnAckedRecievedSegmentList.AddTail(segment->GetListItem(ListItemType::UnAcked));
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

				if (segment->IsSynchronousSegment()
					|| segment->IsAcknowledgementSegment())
				{
					Segment::Delete(segment);
				}
            }

            sys::CThread::YieldThread();
        }

		// 後始末
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

        //std::lock_guard<std::mutex> locker(m_UnAckedSentSegmentListLocker);
        m_UnAckedSentSegmentListLocker.lock();
        {
            auto item = m_UnAckedSentSegmentList.GetTop();

            while (item != IZ_NULL) {
                auto s = item->GetData();
                auto next = item->GetNext();

                if ((CompareSequenceNumbers(s->SequenceNumber(), ackNumber) <= 0))
                {
					item->Leave();
					Segment::Delete(s);
                }

                item = next;
            }
        }
        m_UnAckedSentSegmentListLocker.unlock();

        m_VacantUnAckedSentSegListWait.Set();
    }

    // 受信した接続要求(SYNchronous)セグメントを処理する.
    void ReliableUDP::HandleSYNSegment(SynchronousSegment* segment)
    {
        switch (m_CurState)
        {
            case State::NONE:  // 未接続.
            {
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

                m_UnAckedRecievedSegmentList.Remove(segment->GetListItem(ListItemType::UnAcked));
            }
                break;
            case State::SYN_SENT:    // 接続要求送信済み.
            {
                m_Counter.SetLastInSequenceNumber(segment->SequenceNumber());

                m_Parameter.MaxSegmentSize = segment->MaxSegmentSize();
                m_Parameter.MaxNumberOfOutStandingSegs = segment->MaxNumberOfOutStandingSegs();
                m_Parameter.MaxRetransmission = segment->MaxRetransmission();
                m_Parameter.MaxCumulativeAck = segment->MaxCumulativeAck();
                m_Parameter.MaxOutOfSequence = segment->MaxOutOfSequence();
                m_Parameter.MaxAutoReset = segment->MaxAutoReset();
                m_Parameter.NullSegmentTimeout = segment->NullSegmentTimeout();
                m_Parameter.RetransmissionTimeout = segment->RetransmissionTimeout();
                m_Parameter.CumulativeAckTimeout = segment->CumulativeAckTimeout();

                IZ_ASSERT(m_Parameter.MaxSegmentSize > Segment::RUDP_HEADER_LEN);

                // 返事を返す.
                {
                    std::lock_guard<std::mutex> locker(m_RecieveQueueLocker);
                    SendAcknowledgement(segment);
                }

                // 接続確立.
                OpenConnection();
            }
                break;
        }

        if (!m_isAllocated) {
            if (m_recvData == nullptr) {
                m_recvData = ALLOC(m_allocator, m_Parameter.MaxSegmentSize);
            }
            if (m_sendData == nullptr) {
                m_sendData = ALLOC(m_allocator, m_Parameter.MaxSegmentSize);
            }
            m_isAllocated = IZ_TRUE;
        }
    }

    void ReliableUDP::HandleEAKSegment(ExtendAckSegment* segment)
    {
        auto& ackNumbers = segment->AckNumbers();

        int lastInSequence = segment->GetAcknowledgedNumber();
        int lastOutSequence = ackNumbers.at(ackNumbers.getNum() - 1);

        // 送信したけど応答がないセグメントリストに対象となるものがないか調べる.
        //std::lock_guard<std::mutex> locker(m_UnAckedSentSegmentListLocker);
        m_UnAckedSentSegmentListLocker.lock();
        {
            auto item = m_UnAckedSentSegmentList.GetTop();

            while (item != IZ_NULL)
            {
                auto s = item->GetData();
                auto next = item->GetNext();

                if (CompareSequenceNumbers(s->SequenceNumber(), lastInSequence) <= 0)
                {
                    // シーケンス内に入っているので、どこかで応答を受けたはず.
                    item->Leave();
                    item = next;
                    continue;
                }

                for (IZ_UINT n = 0; n < ackNumbers.getNum(); n++)
                {
                    auto ack = ackNumbers.at(n);

                    if (CompareSequenceNumbers(s->SequenceNumber(), ack) == 0)
                    {
                        item->Leave();
                        item = next;
                        break;
                    }
                }
            }

            // 残ったのものは再送する.
            for (IZ_UINT i = 0; i < m_UnAckedSentSegmentList.GetItemNum(); i++)
            {
                auto s = m_UnAckedSentSegmentList.GetAt(i)->GetData();

                if (CompareSequenceNumbers(lastInSequence, s->SequenceNumber()) < 0
                    && CompareSequenceNumbers(lastOutSequence, s->SequenceNumber()) > 0)
                {
                    // 再送.
                    RetransmitSegment(s);
                }
            }
        }
        m_UnAckedSentSegmentListLocker.unlock();
    }

    // 受信したセグメントを処理.
    void ReliableUDP::HandleSegment(Segment* segment)
    {
        // リセットセグメントがきたので通信をリセットする.
        if (segment->IsResetSegment())
        {
            // TODO
        }

        // これ以上なにも送られてこないことが通知されてきた.
        if (segment->IsFinishSegment())
        {
            switch(m_CurState)
            {
                case State::SYN_SENT:
                    // 同期セグメント送信状態の場合は、返事待ちしているところ（connect）を解除して先に進むようにする.
                    m_ConnectEvent.Set();
                    break;
                case State::CLOSED:
                    // すでに終了しているので何もしない.
                    break;
                default:
                    m_CurState = State::WILL_CLOSE;
                    break;
            }
        }

        // シーケンス番号が正しいかどうか.
        IZ_BOOL isInSequence = IZ_FALSE;

        std::lock_guard<std::mutex> locker(m_RecieveQueueLocker);
        {
            // 期待するシーケンス番号.
            auto nextSeqNumber = NextSequenceNumber(m_Counter.GetLastInSequenceNumber());

            if (CompareSequenceNumbers(segment->SequenceNumber(), m_Counter.GetLastInSequenceNumber()) >= 0)
            {
                // 送信したものと同じもの or 番号が小さいものがきた(すでに届いているものがきた)ので無視する.
            }
            else if (CompareSequenceNumbers(segment->SequenceNumber(), nextSeqNumber) == 0)
            {
                // 期待する番号がきた.
                isInSequence = IZ_TRUE;

                if (m_InSequenceSegmentList.GetItemNum() + m_OutSequenceSegmentList.GetItemNum() < MaxRecvListSize)
                {
                    // キューに余裕がある.

                    // シーケンス番号を更新.
                    m_Counter.SetLastInSequenceNumber(segment->SequenceNumber());

                    if (segment->IsDataSegment()
                        || segment->IsResetSegment()
                        || segment->IsFinishSegment())
                    {
                        // read で読み込むセグメントはリストに登録.
                        m_InSequenceSegmentList.AddTail(segment->GetListItem(ListItemType::Sequence));
                        m_InSequenceSegWait.Set();
                    }

#if 0
                    // 順番通りにデータが来たことを通知.
                    if (segment->IsDataSegment)
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

                if (m_InSequenceSegmentList.GetItemNum() + m_OutSequenceSegmentList.GetItemNum() < MaxRecvListSize)
                {
                    IZ_BOOL added = IZ_FALSE;

                    for (IZ_UINT i = 0; i < m_OutSequenceSegmentList.GetItemNum(); i++)
                    {
                        auto s = m_OutSequenceSegmentList.GetAt(i)->GetData();

                        if (CompareSequenceNumbers(segment->SequenceNumber(), s->SequenceNumber()) == 0)
                        {
                            // 登録済み.
                            added = IZ_TRUE;
                            break;
                        }
                    }

                    if (!added)
                    {
                        // 未登録なので、登録する.
                        m_OutSequenceSegmentList.Add(segment->GetListItem(ListItemType::Sequence));
                    }

#if 0
                    // 順序がずれたパケットを受けたことを通知する.
                    if (segment->IsDataSegment)
                    {
                        OnRecivedPacketOutOfOrder(segment as DataSegment);
                    }
#endif
                }
            }
                
            if (isInSequence && (segment->IsDataSegment()
                || segment->IsNullSegment()
                || segment->IsFinishSegment()))
            {
                // 受信した応答を返す.
                SendAcknowledgement(segment);
            }
            else if (m_OutSequenceSegmentList.GetItemNum() > m_Parameter.MaxOutOfSequence)
            //else if (m_OutSequenceSegmentList.Count > 0)
            {
                // out-of-sequenceセグメントの数が許容最大値を超えた.
                SendExtendAcknowledgement();
            }
            else if (m_UnAckedRecievedSegmentList.GetItemNum() > m_Parameter.MaxCumulativeAck)
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

    // シーケンス番号をチェックして、out-of-sequenceリストから正しい順序のものを InSequenceSegmentQueue に移す.
    void ReliableUDP::UpdateOutSequeceQueue()
    {
        // NOTE
        // m_RecieveQueueLockerがロックされている内部でよばないといけない

        auto item = m_OutSequenceSegmentList.GetTop();

        while (item != IZ_NULL)
        {
            auto segment = item->GetData();
            auto next = item->GetNext();

            // 期待するシーケンス番号.
            auto nextSequenceNumber = NextSequenceNumber(m_Counter.GetLastInSequenceNumber());

            // シーケンス番号が更新されたことで、正しい番号になる可能性があるものを探す.
            if (CompareSequenceNumbers(segment->SequenceNumber(), nextSequenceNumber) == 0)
            {
                // シーケンス番号を更新
                m_Counter.SetLastInSequenceNumber(segment->SequenceNumber());

                if (segment->IsDataSegment()
                    || segment->IsResetSegment()
                    || segment->IsFinishSegment())
                {
                    // シーケンス処理内セグメントリストに登録.
                    m_InSequenceSegmentList.AddTail(segment->GetListItem(ListItemType::Sequence));

                    m_InSequenceSegWait.Set();
                }

                // out-of-sequenceリストから移動したものを削除.
                {
                    // 返事をする
                    SendSingleAcknowledgement(segment);

                    item->Leave();
                }
            }

            item = next;
        }
    }

    // シーケンス番号を比較.
    // 等しければ 0, s1<s2 なら 1, それ以外は -1 を返す.
    IZ_INT ReliableUDP::CompareSequenceNumbers(IZ_INT s1, IZ_INT s2)
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
    IZ_INT ReliableUDP::NextSequenceNumber(IZ_INT sequenceNumber)
    {
        return (sequenceNumber + 1) % MAX_SEQUENCE_NUMBER;
    }

    // 受信した返事を返す.
    void ReliableUDP::SendAcknowledgement(Segment* segment)
    {
        //std::lock_guard<std::mutex> locker(m_RecieveQueueLocker);
        {
            // 受信したけど out-of-sequence 状態のセグメントがある場合.
            if (m_OutSequenceSegmentList.GetItemNum() > 0)
            {
                SendExtendAcknowledgement();
                return;
            }

            SendSingleAcknowledgement(segment);
        }
    }

    // out-of-sequenceセグメントがあったことについて返事をする.
    void ReliableUDP::SendExtendAcknowledgement()
    {
        //std::lock_guard<std::mutex> locker(m_RecieveQueueLocker);
        {
            // 受信したけど out-of-sequence 状態のセグメントが無いのに呼ばれた.
            if (m_OutSequenceSegmentList.GetItemNum() == 0)
            {
                return;
            }

            // out-of-sequenceセグメントのシーケンス番号を集める.
            IZ_INT ackNumbers[32];
            IZ_ASSERT(COUNTOF(ackNumbers) >= m_OutSequenceSegmentList.GetItemNum());

            IZ_UINT cntAckNumbers = 0;

            for (IZ_UINT i = 0; i < m_OutSequenceSegmentList.GetItemNum(); i++)
            {
                auto segment = m_OutSequenceSegmentList.GetAt(i)->GetData();

                ackNumbers[i] = segment->SequenceNumber();

                // 受信したけど返信していないセグメントをリストから削除.
                m_UnAckedRecievedSegmentList.Remove(segment->GetListItem(ListItemType::UnAcked));

                cntAckNumbers++;
            }

            // out-of-sequenceセグメントリストをリセット.
            m_OutSequenceSegmentList.Clear();

            auto lastInSequenceNumber = m_Counter.GetLastInSequenceNumber();

            auto sendSegment = Segment::Create<ExtendAckSegment>(
                m_allocator,
                m_allocator,
                NextSequenceNumber(lastInSequenceNumber),
                lastInSequenceNumber,
                ackNumbers,
                cntAckNumbers);

            // 送信.
            SendSegment(sendSegment);
        }
    }

    // 受信した返事を返す.
    void ReliableUDP::SendSingleAcknowledgement(Segment* segment)
    {
        {
            std::lock_guard<std::mutex> locker(m_UnAckedRecievedSegmentListLocker);

            // 受信したけど返信していないセグメントの数が 0 のときは何もしない.
            if (m_UnAckedRecievedSegmentList.GetItemNum() == 0)
            {
                return;
            }
        }

#if 0
        auto lastInSequence = m_Counter.GetLastInSequenceNumber();

        auto sendSegment = new AcknowledgementSegment(
            NextSequenceNumber(lastInSequence),
            lastInSequence);
#elif 0
        auto sendSegment = Segment::Create<AcknowledgementSegment>(
            m_allocator,
            NextSequenceNumber(segment->SequenceNumber()),
            segment->SequenceNumber());

		// 送信.
		SendSegment(sendSegment);
#else
		AcknowledgementSegment sendSegment;
		sendSegment.Init(
			m_allocator,
			NextSequenceNumber(segment->SequenceNumber()),
			segment->SequenceNumber());

		// 送信.
		SendSegment(&sendSegment);
#endif

        if (segment != nullptr)
        {
            // NOTE
            // 順番が入れ替わった場合もあるのでここでシーケンス番号チェックはできない

            // 受信したが確認応答を送っていないセグメントのリストから削除.
            m_UnAckedRecievedSegmentList.Remove(segment->GetListItem(ListItemType::UnAcked));
        }
    }

    // 接続確立.
    void ReliableUDP::OpenConnection()
    {
        if (m_CurState == State::ESTABLISHED)
        {
            // TODO
            // すでに接続確立されている場合.
        }
        else
        {
            m_CurState = State::ESTABLISHED;

            m_ConnectEvent.Set();

            OnOpenConnection();
        }
    }

    // セグメントを再送.
    void ReliableUDP::RetransmitSegment(Segment* segment)
    {
        // 再送回数を増やす.
        segment->IncrementRetryCount();

        if (m_Parameter.MaxRetransmission != 0
            && segment->RetryCount() > m_Parameter.MaxRetransmission)
        {
            // TODO
            // 最大再送回数を超えた場合は接続が失敗していることにする.
            return;
        }

        IZ_PRINTF("RetransmitSegment --- ");
        SendSegment(segment);
    }

    void ReliableUDP::ProcRetransmit()
    {
        auto itemNum = 0;

        {
#if 0
            std::lock_guard<std::mutex> locker(m_UnAckedSentSegmentListLocker);
            itemNum = m_UnAckedRecievedSegmentList.GetItemNum();
#else
            m_UnAckedSentSegmentListLocker.safeExec([&]() {
                itemNum = m_UnAckedSentSegmentList.GetItemNum();
            });
#endif
        }

        if (itemNum == 0)
        {
            //m_NotEmptyUnAckedSentSegListWait.Wait();
            //m_NotEmptyUnAckedSentSegListWait.Reset();
            return;
        }
            
        // 返事を受けていないセグメントを再送.

#if 0
        std::lock_guard<std::mutex> locker(m_UnAckedSentSegmentListLocker);
        {
            auto item = m_UnAckedSentSegmentList.GetTop();

            while (item != IZ_NULL) {
                auto segment = item->GetData();

                RetransmitSegment(segment);

                item = item->GetNext();
            }
        }
#else
        m_UnAckedSentSegmentListLocker.lock();
        {
            auto item = m_UnAckedSentSegmentList.GetTop();

            while (item != IZ_NULL) {
                auto segment = item->GetData();

                RetransmitSegment(segment);

                item = item->GetNext();
            }
        }
        m_UnAckedSentSegmentListLocker.unlock();
#endif
    }

    // セグメント受信実装.
    Segment* ReliableUDP::OnRecieveSegment()
    {
        IPv4Endpoint remote;
        auto ret = OnRecieveSegment(remote);
        return ret;
    }

    Segment* ReliableUDP::OnRecieveSegment(IPv4Endpoint& remote)
    {
        IZ_UINT8 tmp[64];

        void* data = tmp;
        IZ_UINT length = sizeof(tmp);

        if (m_isAllocated) {
            data = m_recvData;
            length = m_Parameter.MaxSegmentSize;
        }

        IZ_INT result = m_Udp->recieveFrom(data, length, remote);

        if (result <= 0) {
            return nullptr;
        }

        auto segment = Segment::Parse(
            m_allocator,
            (IZ_UINT8*)data, 0, result);

        IZ_CHAR str[64];
        segment->ToString(str, sizeof(str));

        IZ_PRINTF("Recieve Segment [%s]\n", str);

        return segment;
    }

#if 1
    // セグメント送信実装.
    void ReliableUDP::OnSendSegment(Segment* segment)
    {
        IZ_CHAR str[64];
        segment->ToString(str, sizeof(str));

        IZ_PRINTF("Send Segment [%s]\n", str);

        IZ_UINT8 tmp[64];

        void* data = tmp;
        IZ_UINT length = sizeof(tmp);

        if (m_isAllocated) {
            data = m_sendData;
            length = m_Parameter.MaxSegmentSize;
        }

        CMemoryOutputStream ws(data, length);

        segment->WriteBytes(&ws);
        auto bytes = ws.GetBuffer();
        auto size = ws.GetCurPos();
        m_Udp->sendData(bytes, size);
    }
#else
    static IZ_BOOL isValid = IZ_FALSE;

    void ReliableUDP::OnSendSegment(Segment* segment)
    {
        // 疑似パケットロス.

        if (!IsConnected() || isValid) {
            IZ_CHAR str[64];
            segment->ToString(str, sizeof(str));

            IZ_PRINTF("Send Segment [%s]\n", str);

            IZ_UINT8 tmp[64];

            void* data = tmp;
            IZ_UINT length = sizeof(tmp);

            if (m_isAllocated) {
                data = m_sendData;
                length = m_Parameter.MaxSegmentSize;
            }

            CMemoryOutputStream ws(data, length);

            segment->WriteBytes(&ws);
            auto bytes = ws.GetBuffer();
            auto size = ws.GetCurPos();
            m_Udp->sendData(bytes, size);
        }
        else {
            isValid = IZ_TRUE;
        }
    }
#endif
}   // namespace net
}   // namespace izanagi
