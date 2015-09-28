#if !defined(_IZANAGI_NETWORK_SYN_SEGMENT_H__)
#define _IZANAGI_NETWORK_SYN_SEGMENT_H__

#include "network/rudp/segment/Segment.h"
#include "network/rudp/RUDPParameter.h"

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
namespace net {
    class SynchronousSegment : public Segment
    {
        friend class Segment;
        friend class ReliableUDP;

    public:
        virtual Type GetType() const override
        {
            return Type::SYN;
        }

        virtual const IZ_CHAR* GetSegmentType() override
        {
            return "SYN";
        }

    protected:
        static const IZ_UINT SYN_HEADER_LEN = RUDP_HEADER_LEN + 16;

    public:
        inline IZ_UINT Version() const { return m_Version; }

        inline IZ_UINT MaxNumberOfOutStandingSegs() const { return m_MaxNumberOfOutStandingSegs; }

        inline IZ_UINT OptionFlags() const { return m_OptionFlags; }

        inline IZ_UINT MaxSegmentSize() const { return m_MaxSegmentSize; }

        inline IZ_UINT RetransmissionTimeout() const { return m_RetransmissionTimeout; }

        inline IZ_UINT CumulativeAckTimeout() const { return m_CumulativeAckTimeout; }

        inline IZ_UINT NullSegmentTimeout() const { return m_NullSegmentTimeout; }

        inline IZ_UINT MaxRetransmission() const { return m_MaxRetransmission; }

        inline IZ_UINT MaxCumulativeAck() const { return m_MaxCumulativeAck; }

        inline IZ_UINT MaxOutOfSequence() const { return m_MaxOutOfSequence; }

        inline IZ_UINT MaxAutoReset () const { return m_MaxAutoReset; }

    protected:
        SynchronousSegment()
        {
        }

        SynchronousSegment(
            IZ_INT sequenceNumber,
            IZ_UINT maxNumberOfOutStandingSegs,
            IZ_UINT maxSegmentSize,
            IZ_UINT retransmissionTimeout,
            IZ_UINT cumulativeAckTimeout,
            IZ_UINT nullSegmentTimeout,
            IZ_UINT maxRetransmission,
            IZ_UINT maxCumulativeAck,
            IZ_UINT maxOutOfSequence,
            IZ_UINT maxAutoReset);

        SynchronousSegment(IZ_INT sequenceNumber, const RUDPParameter& parameter)
            : SynchronousSegment(sequenceNumber,
            parameter.MaxNumberOfOutStandingSegs,
            parameter.MaxSegmentSize,
            parameter.RetransmissionTimeout,
            parameter.CumulativeAckTimeout,
            parameter.NullSegmentTimeout,
            parameter.MaxRetransmission,
            parameter.MaxCumulativeAck,
            parameter.MaxOutOfSequence,
            parameter.MaxAutoReset)
        {
        }

        virtual ~SynchronousSegment() {}

        virtual void WriteBytes(IOutputStream* wr) override;

        virtual void ReadBytes(
            IMemoryAllocator* allocator,
            IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length) override;

    private:
        IZ_UINT m_Version;

        IZ_UINT m_MaxNumberOfOutStandingSegs;

        IZ_UINT m_OptionFlags;

        IZ_UINT m_MaxSegmentSize;

        IZ_UINT m_RetransmissionTimeout;

        IZ_UINT m_CumulativeAckTimeout;

        IZ_UINT m_NullSegmentTimeout;

        IZ_UINT m_MaxRetransmission;

        IZ_UINT m_MaxCumulativeAck;

        IZ_UINT m_MaxOutOfSequence;

        IZ_UINT m_MaxAutoReset;
    };
}   // namespace net
}   // namespace izanagi

#endif  // _IZANAGI_NETWORK_SYN_SEGMENT_H__