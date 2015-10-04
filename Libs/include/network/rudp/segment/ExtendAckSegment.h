#if !defined(_IZANAGI_NETWORK_EXTEND_ACK_SEGMENT_H__)
#define _IZANAGI_NETWORK_EXTEND_ACK_SEGMENT_H__

#include "network/rudp/segment/Segment.h"

/*
*  EACK Segment
*
*   0 1 2 3 4 5 6 7 8            15
*  +-+-+-+-+-+-+-+-+---------------+
*  |0|1|1|0|0|0|0|0|     N + 6     |
*  +-+-+-+-+-+-+-+-+---------------+
*  | Sequence #    |   Ack Number  |
*  +---------------+---------------+
*  |1st out of seq |2nd out of seq |
*  |  ack number   |   ack number  |
*  +---------------+---------------+
*  |  . . .        |Nth out of seq |
*  |               |   ack number  |
*  +---------------+---------------+
*  |            Checksum           |
*  +---------------+---------------+
*
*/
namespace izanagi {
namespace net {
    class ExtendAckSegment : public Segment
    {
        friend class Segment;
        friend class ReliableUDP;

    public:
        virtual Type GetType() const override
        {
            return Type::EAK;
        }

        virtual const IZ_CHAR* GetSegmentType() override
        {
            return "EAK";
        }

    private:
        ExtendAckSegment() {}

        ExtendAckSegment(
            IMemoryAllocator* allocator,
            IZ_INT sequenceNumber,
            IZ_INT acknowledgedNumber,
            IZ_INT* ackNumbers,
            IZ_UINT ackNumbersNum);

        virtual ~ExtendAckSegment() {}

        virtual void WriteBytes(IOutputStream* wr) override;

        virtual void ReadBytes(
            IMemoryAllocator* allocator,
            IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length) override;

        const CArray<IZ_INT>& AckNumbers() const
        {
            return m_AckNumbers;
        }

    private:
        CArray<IZ_INT> m_AckNumbers;
    };
}   // namespace net
}   // namespace izanagi

#endif  // #if !defined(_IZANAGI_NETWORK_EXTEND_ACK_SEGMENT_H__)
