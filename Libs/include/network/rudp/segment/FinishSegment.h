#if !defined(_IZANAGI_NETWORK_FINISH_SEGMENT_H__)
#define _IZANAGI_NETWORK_FINISH_SEGMENT_H__

#include "network/rudp/segment/Segment.h"

/*
*  FIN Segment
*
*   0 1 2 3 4 5 6 7 8            15
*  +-+-+-+-+-+-+-+-+---------------+
*  | |A| | | | | | |               |
*  |0|C|0|0|0|0|1|0|        6      |
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
    class FinishSegment : Segment
    {
        friend class Segment;
		friend class ReliableUDP;

    public:
        virtual Type GetType() const override
        {
            return Type::FIN;
        }

        virtual const IZ_CHAR* GetSegmentType() override
        {
            return "FIN";
        }

    protected:
        FinishSegment()
        {
        }

        FinishSegment(IZ_INT sequenceNumber)
            : Segment(Type::FIN, sequenceNumber, RUDP_HEADER_LEN)
        {
        }

        virtual ~FinishSegment() {}

		void Init(
			IMemoryAllocator* allocator,
			IZ_INT sequenceNumber)
		{
			m_allocator = allocator;

			m_Flags = (int)Type::FIN;
			m_SequenceNumber = sequenceNumber;
			m_HeaderLength = RUDP_HEADER_LEN;

			m_AckNumber = -1;
		}

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

            IZ_ASSERT(CheckFlag(Type::FIN));
        }
    };
}   // namespace net
}   // namespace izanagi

#endif  // _IZANAGI_NETWORK_FINISH_SEGMENT_H__