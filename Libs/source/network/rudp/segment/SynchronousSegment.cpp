#include "network/rudp/segment/SynchronousSegment.h"

/*
*  SYNchronous Segment
*
*   0             7 8             15
*  +-+-+-+-+-+-+-+-+---------------+
*  | |A| | | | | | |               |
*  |1|C|0|0|0|0|0|0|       22      |
*  | |K| | | | | | |               |
*  +-+-+-+-+-+-+-+-+---------------+
*  |  Sequence #   +   Ack Number  |
*  +---------------+---------------+
*  | Vers  | Spare | Max # of Out  |
*  |       |       | standing Segs |
*  +---------------+---------------+
*  | Option Flags  |     Spare     |
*  +---------------+---------------+
*  |     Maximum Segment Size      |
*  +---------------+---------------+
*  | Retransmission Timeout Value  |
*  +---------------+---------------+
*  | Cumulative Ack Timeout Value  |
*  +---------------+---------------+
*  |   Null Segment Timeout Value  |
*  +---------------+---------------+
*  |  Max Retrans  | Max Cum Ack   |
*  +---------------+---------------+
*  | Max Out of Seq| Max Auto Reset|
*  +---------------+---------------+
*  |           Checksum            |
*  +---------------+---------------+
*
*/
namespace izanagi {
namespace network {
    SynchronousSegment::SynchronousSegment(
        IZ_INT sequenceNumber,
        IZ_UINT maxNumberOfOutStandingSegs,
        IZ_UINT maxSegmentSize,
        IZ_UINT retransmissionTimeout,
        IZ_UINT cumulativeAckTimeout,
        IZ_UINT nullSegmentTimeout,
        IZ_UINT maxRetransmission,
        IZ_UINT maxCumulativeAck,
        IZ_UINT maxOutOfSequence,
        IZ_UINT maxAutoReset)
        : Segment(Type::SYN, sequenceNumber, SYN_HEADER_LEN)
    {
        m_Version = RUDP_VERSION;

        m_MaxNumberOfOutStandingSegs = maxNumberOfOutStandingSegs;
        m_OptionFlags = 0x01;    // no options.
        m_MaxSegmentSize = maxSegmentSize;
        m_RetransmissionTimeout = retransmissionTimeout;
        m_CumulativeAckTimeout = cumulativeAckTimeout;
        m_NullSegmentTimeout = nullSegmentTimeout;
        m_MaxRetransmission = maxRetransmission;
        m_MaxCumulativeAck = maxCumulativeAck;
        m_MaxOutOfSequence = maxOutOfSequence;
        m_MaxAutoReset = maxAutoReset;
    }

    void SynchronousSegment::WriteBytes(IOutputStream* wr)
    {
        Segment::WriteBytes(wr);

        wr->Write((IZ_UINT8)((m_Version << 4) & 0xff));
        wr->Write((IZ_UINT8)m_MaxNumberOfOutStandingSegs);
        wr->Write((IZ_UINT8)m_OptionFlags);
            
        wr->Write(0);    // spare.

        wr->Write((IZ_UINT8)((m_MaxSegmentSize >> 8) & 0xff));
        wr->Write((IZ_UINT8)((m_MaxSegmentSize >> 0) & 0xff));

        wr->Write((IZ_UINT8)((m_RetransmissionTimeout >> 8) & 0xff));
        wr->Write((IZ_UINT8)((m_RetransmissionTimeout >> 0) & 0xff));

        wr->Write((IZ_UINT8)((m_CumulativeAckTimeout >> 8) & 0xff));
        wr->Write((IZ_UINT8)((m_CumulativeAckTimeout >> 0) & 0xff));

        wr->Write((IZ_UINT8)((m_NullSegmentTimeout >> 8) & 0xff));
        wr->Write((IZ_UINT8)((m_NullSegmentTimeout >> 0) & 0xff));

        wr->Write((IZ_UINT8)m_MaxRetransmission);
        wr->Write((IZ_UINT8)m_MaxCumulativeAck);
        wr->Write((IZ_UINT8)m_MaxOutOfSequence);
        wr->Write((IZ_UINT8)m_MaxAutoReset);

        // For CheckSum
        wr->Write(0);
        wr->Write(0);
    }

    void SynchronousSegment::ReadBytes(
        IMemoryAllocator* allocator,
        IZ_UINT8* IZ_UINT8s, IZ_UINT offset, IZ_UINT length)
    {
        Segment::ReadBytes(allocator, IZ_UINT8s, offset, length);

        IZ_ASSERT(CheckFlag(Type::SYN));

        IZ_ASSERT(m_HeaderLength == SYN_HEADER_LEN);

        m_Version = (IZ_UINT8s[offset + 4] >> 4);
        IZ_ASSERT(m_Version == RUDP_VERSION);

        m_MaxNumberOfOutStandingSegs = IZ_UINT8s[offset + 5];
        m_OptionFlags = IZ_UINT8s[offset + 6];
            
        auto spare = IZ_UINT8s[offset + 7];

        m_MaxSegmentSize = ((IZ_UINT8s[offset + 8] << 8) | IZ_UINT8s[offset + 9]);
        m_RetransmissionTimeout = ((IZ_UINT8s[offset + 10] << 8) | IZ_UINT8s[offset + 11]);
        m_CumulativeAckTimeout = ((IZ_UINT8s[offset + 12] << 8) | IZ_UINT8s[offset + 13]);
        m_NullSegmentTimeout = ((IZ_UINT8s[offset + 14] << 8) | IZ_UINT8s[offset + 15]);

        m_MaxRetransmission = IZ_UINT8s[offset + 16];
        m_MaxCumulativeAck = IZ_UINT8s[offset + 17];
        m_MaxOutOfSequence = IZ_UINT8s[offset + 18];
        m_MaxAutoReset = IZ_UINT8s[offset + 19];
    }
}   // namespace network
}   // namespace izanagi
