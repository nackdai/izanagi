#if !defined(_IZANAGI_NETWORK_NULL_SEGMENT_H__)
#define _IZANAGI_NETWORK_NULL_SEGMENT_H__

#include "network/rudp/segment/Segment.h"

/*
 *  NUL Segment
 *
 *   0 1 2 3 4 5 6 7 8            15
 *  +-+-+-+-+-+-+-+-+---------------+
 *  |0|1|0|0|1|0|0|0|       6       |
 *  +-+-+-+-+-+-+-+-+---------------+
 *  | Sequence #    |  Ack Number   |
 *  +---------------+---------------+
 *  |            Checksum           |
 *  +---------------+---------------+
 *
 */
namespace izanagi {
namespace net {
    class NullSegment : public Segment
    {
        friend class Segment;

    public:
        virtual Type GetType() const override
        {
            return Type::NUL;
        }

        virtual const IZ_CHAR* GetSegmentType() override
        {
            return "NUL";
        }

    protected:
        NullSegment()
        {
        }

        NullSegment(IZ_INT sequenceNumber)
            : Segment(Type::NUL, sequenceNumber, RUDP_HEADER_LEN)
        {
        }

        virtual ~NullSegment() {}

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

            IZ_ASSERT(CheckFlag(Type::NUL));
        }
    };
}   // namespace net
}   // namespace izanagi

#endif  // _IZANAGI_NETWORK_NULL_SEGMENT_H__