#if !defined(_IZANAGI_NETWORK_RESET_SEGMENT_H__)
#define _IZANAGI_NETWORK_RESET_SEGMENT_H__

#include "network/rudp/segment/Segment.h"

/*
*  ReSeT Segment
*
*   0 1 2 3 4 5 6 7 8            15
*  +-+-+-+-+-+-+-+-+---------------+
*  | |A| | | | | | |               |
*  |0|C|0|1|0|0|0|0|        6      |
*  | |K| | | | | | |               |
*  +-+-+-+-+-+-+-+-+---------------+
*  | Sequence #    |   Ack Number  |
*  +---------------+---------------+
*  |         Header Checksum       |
*  +---------------+---------------+
*
*/
namespace izanagi {
namespace net {
    class ResetSegment : Segment
    {
        friend class Segment;

    public:
        virtual Type GetType() const override
        {
            return Type::RST;
        }

        virtual const IZ_CHAR* GetSegmentType() override
        {
            return "RST";
        }

    protected:
        ResetSegment()
        {
        }

        ResetSegment(IZ_INT sequenceNumber)
            : Segment(Type::RST, sequenceNumber, RUDP_HEADER_LEN)
        {
        }

        virtual ~ResetSegment() {}

        virtual void WriteBytes(IOutputStream* wr) override
        {
            Segment::WriteBytes(wr);

            // For CheckSum
            wr->Write(0);
            wr->Write(0);
        }

        virtual void ReadBytes(
            IMemoryAllocator* allocator,
            IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length) override
        {
            Segment::ReadBytes(allocator, bytes, offset, length);

            IZ_ASSERT(CheckFlag(Type::RST));
        }
    };
}   // namespace net
}   // namespace izanagi

#endif  // _IZANAGI_NETWORK_RESET_SEGMENT_H__