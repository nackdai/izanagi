#if !defined(_IZANAGI_NETWORK_RUDP_PARAMETER_H__)
#define _IZANAGI_NETWORK_RUDP_PARAMETER_H__

#include "izDefs.h"

namespace izanagi {
namespace network {
    class RUDPParameter
    {
    public:
        static const IZ_UINT MAX_SEND_QUEUE_SIZE = 32;
        static const IZ_UINT MAX_RECV_QUEUE_SIZE = 32;

        static const IZ_UINT MAX_SEGMENT_SIZE = 128;
        static const IZ_UINT MAX_OUTSTANDING_SEGS = 3;
        static const IZ_UINT MAX_RETRANS = 3;
        static const IZ_UINT MAX_CUMULATIVE_ACKS = 3;
        static const IZ_UINT MAX_OUT_OF_SEQUENCE = 3;
        static const IZ_UINT MAX_AUTO_RESET = 3;
        static const IZ_UINT NULL_SEGMENT_TIMEOUT = 2000;
        static const IZ_UINT RETRANSMISSION_TIMEOUT = 600;
        static const IZ_UINT CUMULATIVE_ACK_TIMEOUT = 300;

    public:
        inline IZ_UINT MaxSendQueueSize() const { return m_MaxSendQueueSize; }

        inline IZ_UINT MaxRecieveQueueSize() const { return m_MaxRecieveQueueSize; }

        inline IZ_UINT MaxNumberOfOutStandingSegs() const { return m_MaxNumberOfOutStandingSegs; }

        inline IZ_UINT OptionFlags() const { return m_OptionFlags; }

        inline IZ_UINT MaxSegmentSize() const { return m_MaxSegmentSize; }

        inline IZ_UINT RetransmissionTimeout() const { return m_RetransmissionTimeout; }

        inline IZ_UINT CumulativeAckTimeout() const { return m_CumulativeAckTimeout; }

        inline IZ_UINT NullSegmentTimeout() const { return m_NullSegmentTimeout; }

        inline IZ_UINT MaxRetransmission() const { return m_MaxRetransmission; }

        inline IZ_UINT MaxCumulativeAck() const { return m_MaxCumulativeAck; }

        inline IZ_UINT MaxOutOfSequence() const { return m_MaxOutOfSequence; }

        inline IZ_UINT MaxAutoReset() const { return m_MaxAutoReset; }

        RUDPParameter()
        {
        }

        RUDPParameter(
            IZ_UINT maxSendQueueSize,
            IZ_UINT maxRecvQueueSize,
            IZ_UINT maxSegmentSize,
            IZ_UINT maxOutstandingSegs,
            IZ_UINT maxRetrans,
            IZ_UINT maxCumulativeAcks,
            IZ_UINT maxOutOfSequence,
            IZ_UINT maxAutoReset,
            IZ_UINT nullSegmentTimeout,
            IZ_UINT retransmissionTimeout,
            IZ_UINT cumulativeAckTimeout)
        {
            m_MaxSendQueueSize = maxSendQueueSize;
            m_MaxRecieveQueueSize = maxRecvQueueSize;
            m_MaxSegmentSize = maxSegmentSize;
            m_MaxNumberOfOutStandingSegs = maxOutstandingSegs;
            m_MaxRetransmission = maxRetrans;
            m_MaxCumulativeAck = maxCumulativeAcks;
            m_MaxOutOfSequence = maxOutOfSequence;
            m_MaxAutoReset = maxAutoReset;
            m_NullSegmentTimeout = nullSegmentTimeout;
            m_RetransmissionTimeout = retransmissionTimeout;
            m_CumulativeAckTimeout = cumulativeAckTimeout;
        }

        ~RUDPParameter() {}

        RUDPParameter(const RUDPParameter& rhs)
        {
            *this = rhs;
        }

        RUDPParameter& operator=(const RUDPParameter& rhs)
        {
            m_MaxSendQueueSize = rhs.MaxSendQueueSize();
            m_MaxRecieveQueueSize = rhs.MaxRecieveQueueSize();
            m_MaxSegmentSize = rhs.MaxSegmentSize();
            m_MaxNumberOfOutStandingSegs = rhs.MaxNumberOfOutStandingSegs();
            m_MaxRetransmission = rhs.MaxRetransmission();
            m_MaxCumulativeAck = rhs.MaxCumulativeAck();
            m_MaxOutOfSequence = rhs.MaxOutOfSequence();
            m_MaxAutoReset = rhs.MaxAutoReset();
            m_NullSegmentTimeout = rhs.NullSegmentTimeout();
            m_RetransmissionTimeout = rhs.RetransmissionTimeout();
            m_CumulativeAckTimeout = rhs.CumulativeAckTimeout();
        }

    private:
        IZ_UINT m_MaxSendQueueSize{ MAX_SEND_QUEUE_SIZE };

        IZ_UINT m_MaxRecieveQueueSize{ MAX_RECV_QUEUE_SIZE };

        IZ_UINT m_MaxNumberOfOutStandingSegs{ MAX_OUTSTANDING_SEGS };

        IZ_UINT m_OptionFlags{ 0 };

        IZ_UINT m_MaxSegmentSize{ MAX_SEGMENT_SIZE };

        IZ_UINT m_RetransmissionTimeout{ RETRANSMISSION_TIMEOUT };

        IZ_UINT m_CumulativeAckTimeout{ CUMULATIVE_ACK_TIMEOUT };

        IZ_UINT m_NullSegmentTimeout{ NULL_SEGMENT_TIMEOUT };

        IZ_UINT m_MaxRetransmission{ MAX_RETRANS };

        IZ_UINT m_MaxCumulativeAck{ MAX_CUMULATIVE_ACKS };

        IZ_UINT m_MaxOutOfSequence{ MAX_OUT_OF_SEQUENCE };

        IZ_UINT m_MaxAutoReset{ MAX_AUTO_RESET };
    };
}   // namespace network
}   // namespace izanagi

#endif  // #if !defined(_IZANAGI_NETWORK_RUDP_PARAMETER_H__)
