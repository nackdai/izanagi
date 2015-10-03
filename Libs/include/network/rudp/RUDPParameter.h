#if !defined(_IZANAGI_NETWORK_RUDP_PARAMETER_H__)
#define _IZANAGI_NETWORK_RUDP_PARAMETER_H__

#include "izDefs.h"

namespace izanagi {
namespace net {
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
            MaxSendQueueSize = maxSendQueueSize;
            MaxRecieveQueueSize = maxRecvQueueSize;
            MaxSegmentSize = maxSegmentSize;
            MaxNumberOfOutStandingSegs = maxOutstandingSegs;
            MaxRetransmission = maxRetrans;
            MaxCumulativeAck = maxCumulativeAcks;
            MaxOutOfSequence = maxOutOfSequence;
            MaxAutoReset = maxAutoReset;
            NullSegmentTimeout = nullSegmentTimeout;
            RetransmissionTimeout = retransmissionTimeout;
            CumulativeAckTimeout = cumulativeAckTimeout;
        }

        ~RUDPParameter() {}

        RUDPParameter(const RUDPParameter& rhs)
        {
            *this = rhs;
        }

        RUDPParameter& operator=(const RUDPParameter& rhs)
        {
            MaxSendQueueSize = rhs.MaxSendQueueSize;
            MaxRecieveQueueSize = rhs.MaxRecieveQueueSize;
            MaxSegmentSize = rhs.MaxSegmentSize;
            MaxNumberOfOutStandingSegs = rhs.MaxNumberOfOutStandingSegs;
            MaxRetransmission = rhs.MaxRetransmission;
            MaxCumulativeAck = rhs.MaxCumulativeAck;
            MaxOutOfSequence = rhs.MaxOutOfSequence;
            MaxAutoReset = rhs.MaxAutoReset;
            NullSegmentTimeout = rhs.NullSegmentTimeout;
            RetransmissionTimeout = rhs.RetransmissionTimeout;
            CumulativeAckTimeout = rhs.CumulativeAckTimeout;

            return *this;
        }

    public:
        IZ_UINT MaxSendQueueSize{ MAX_SEND_QUEUE_SIZE };

        IZ_UINT MaxRecieveQueueSize{ MAX_RECV_QUEUE_SIZE };

        IZ_UINT MaxNumberOfOutStandingSegs{ MAX_OUTSTANDING_SEGS };

        IZ_UINT OptionFlags{ 0 };

        IZ_UINT MaxSegmentSize{ MAX_SEGMENT_SIZE };

        IZ_UINT RetransmissionTimeout{ RETRANSMISSION_TIMEOUT };

        IZ_UINT CumulativeAckTimeout{ CUMULATIVE_ACK_TIMEOUT };

        IZ_UINT NullSegmentTimeout{ NULL_SEGMENT_TIMEOUT };

        IZ_UINT MaxRetransmission{ MAX_RETRANS };

        IZ_UINT MaxCumulativeAck{ MAX_CUMULATIVE_ACKS };

        IZ_UINT MaxOutOfSequence{ MAX_OUT_OF_SEQUENCE };

        IZ_UINT MaxAutoReset{ MAX_AUTO_RESET };
    };
}   // namespace net
}   // namespace izanagi

#endif  // #if !defined(_IZANAGI_NETWORK_RUDP_PARAMETER_H__)
