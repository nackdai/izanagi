#include "network/rudp/segment/DataSegment.h"

/*
*  DATa Segment
*
*   0 1 2 3 4 5 6 7 8            15
*  +-+-+-+-+-+-+-+-+---------------+
*  |0|1|0|0|0|0|0|0|       6       |
*  +-+-+-+-+-+-+-+-+---------------+
*  | Sequence #    |   Ack Number  |
*  +---------------+---------------+
*  |           Checksum            |
*  +---------------+---------------+
*  | ...                           |
*  +-------------------------------+
*
*/
namespace izanagi {
namespace net {
    Segment* DataSegment::Create(
        IMemoryAllocator* allocator,
        IZ_INT sequenceNumber,
        IZ_INT acknowledgedNumber,
        IZ_UINT8* bytes,
        IZ_UINT offset,
        IZ_UINT length)
    {
        void* p = ALLOC(allocator, sizeof(DataSegment));
        IZ_ASSERT(p);

        auto ret = new(p)DataSegment(
            allocator,
            sequenceNumber,
            acknowledgedNumber,
            bytes,
            offset, length);

        return ret;
    }

    DataSegment::DataSegment(
        IMemoryAllocator* allocator,
        IZ_INT sequenceNumber,
        IZ_INT acknowledgedNumber,
        IZ_UINT8* bytes,
        IZ_UINT offset,
        IZ_UINT length)
        : Segment(Type::ACK, sequenceNumber, RUDP_HEADER_LEN)
    {
        m_allocator = allocator;

        SetAcknowledgedNumber(acknowledgedNumber);

        m_Data = ALLOC(m_allocator, length);
        memcpy(m_Data, bytes + offset, length);

        m_Size = length;
    }

    DataSegment::~DataSegment()
    {
        if (m_Data) {
            FREE(m_allocator, m_Data);
        }
    }

    void DataSegment::WriteBytes(IOutputStream* wr)
    {
        Segment::WriteBytes(wr);

        // For CheckSum
        wr->Write(0);
        wr->Write(0);

        wr->Write(m_Data, 0, m_Size);
    }

    void DataSegment::ReadBytes(
        IMemoryAllocator* allocator,
        IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length)
    {
        IZ_ASSERT(m_allocator == nullptr || m_allocator == allocator);

        Segment::ReadBytes(allocator, bytes, offset, length);

        IZ_ASSERT(CheckFlag(Type::ACK));

        auto dataLength = length - RUDP_HEADER_LEN;
        m_Data = ALLOC(allocator, dataLength);

        if (m_allocator == nullptr) {
            m_allocator = allocator;
        }

        memcpy(
            m_Data,
            bytes + offset + RUDP_HEADER_LEN,
            dataLength);

        m_Size = dataLength;
    }
}   // namespace net
}   // namespace izanagi
