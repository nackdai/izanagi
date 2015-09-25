#include "network/rudp/segment/ExtendAckSegment.h"

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
namespace network {
    ExtendAckSegment::ExtendAckSegment(
        IMemoryAllocator* allocator,
        IZ_INT sequenceNumber,
        IZ_INT acknowledgedNumber,
        IZ_INT* ackNumbers,
        IZ_UINT ackNumbersNum)
        : Segment(Type::EAK, sequenceNumber, RUDP_HEADER_LEN + ackNumbersNum)
    {
        m_AckNumbers.init(allocator, ackNumbersNum);

        for (IZ_UINT i = 0; i < ackNumbersNum; i++)
        {
            m_AckNumbers.at(i) = ackNumbers[i];
        }
    }

    void ExtendAckSegment::WriteBytes(IOutputStream* wr)
    {
        Segment::WriteBytes(wr);

        m_AckNumbers.traverse(
            [&](IZ_INT& n) {
            wr->Write((IZ_UINT8)n);
        });

        // TODO
        // padding...

        // For CheckSum
        wr->Write(0);
        wr->Write(0);
    }

    void ExtendAckSegment::ReadBytes(
        IMemoryAllocator* allocator,
        IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length)
    {
        Segment::ReadBytes(allocator, bytes, offset, length);

        IZ_ASSERT(CheckFlag(Type::EAK));

        auto dataLength = length - RUDP_HEADER_LEN;
        m_AckNumbers.init(allocator, dataLength);

        for (IZ_UINT i = 0; i < dataLength; i++) {
            m_AckNumbers.at(i) = bytes[offset + RUDP_HEADER_LEN + i];
        }
    }
}   // namespace network
}   // namespace izanagi
