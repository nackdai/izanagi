#include "network/rudp/segment/Segment.h"
#include "network/rudp/segment/AcknowledgementSegment.h"
#include "network/rudp/segment/DataSegment.h"
#include "network/rudp/segment/ExtendAckSegment.h"
#include "network/rudp/segment/FinishSegment.h"
#include "network/rudp/segment/NullSegment.h"
#include "network/rudp/segment/ResetSegment.h"
#include "network/rudp/segment/SynchronousSegment.h"

/*
*  RUDP Header
*
*   0 1 2 3 4 5 6 7 8            15
*  +-+-+-+-+-+-+-+-+---------------+
*  |S|A|E|R|N|C| | |    Header     |
*  |Y|C|A|S|U|H|0|0|    Length     |
*  |N|K|K|T|L|K| | |               |
*  +-+-+-+-+-+-+-+-+---------------+
*  |  Sequence #   +   Ack Number  |
*  +---------------+---------------+
*  |            Checksum           |
*  +---------------+---------------+
*
*/
namespace izanagi {
namespace net {
    void Segment::Delete(Segment* segment)
    {
        auto allocator = segment->m_allocator;

        delete segment;
        FREE(allocator, segment);
    }

    Segment::Segment(
        Segment::Type type,
        IZ_INT sequenceNumber,
        IZ_UINT hederSize)
        : Segment()
    {
        m_Flags = (int)type;
        m_SequenceNumber = sequenceNumber;
        m_HeaderLength = hederSize;

        m_AckNumber = -1;
    }

    Segment::~Segment()
    {
    }

    // 応答番号をセット.
    // この番号と一致する番号を持つセグメントに対する応答となる.
    void Segment::SetAcknowledgedNumber(IZ_INT ackNumber)
    {
        m_Flags |= Type::ACK;
        m_AckNumber = ackNumber;
    }

    // フラグチェック.
    IZ_BOOL Segment::CheckFlag(IZ_UINT flag, Type type)
    {
        return ((flag & type) != 0);
    }

    // フラグチェック.
    IZ_BOOL Segment::CheckFlag(Type type)
    {
        return CheckFlag(m_Flags, type);
    }

    // 応答番号を取得.
    IZ_INT Segment::GetAcknowledgedNumber()
    {
        if (CheckFlag(Type::ACK))
        {
            return m_AckNumber;
        }

        return -1;
    }

#define NEW_SEGMENT(ret, a, type) \
    void* p = ALLOC(a, sizeof(type));\
    ret = new(p) type##()

    // データをパースして、対応するセグメントを作成する.
    Segment* Segment::Parse(
        IMemoryAllocator* allocator,
        IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length)
    {
        Segment* ret = nullptr;

        auto flags = bytes[0];

        if (CheckFlag(flags, Type::SYN))
        {
            NEW_SEGMENT(ret, allocator, SynchronousSegment);
        }
        else if (CheckFlag(flags, Type::NUL))
        {
            NEW_SEGMENT(ret, allocator, NullSegment);
        }
        else if (CheckFlag(flags, Type::EAK))
        {
            NEW_SEGMENT(ret, allocator, ExtendAckSegment);
        }
        else if (CheckFlag(flags, Type::RST))
        {
            NEW_SEGMENT(ret, allocator, ResetSegment);
        }
        else if (CheckFlag(flags, Type::FIN))
        {
            NEW_SEGMENT(ret, allocator, FinishSegment);
        }
        else if (CheckFlag(flags, Type::ACK))
        {
            if (length == RUDP_HEADER_LEN)
            {
                // ACK
                NEW_SEGMENT(ret, allocator, AcknowledgementSegment);
            }
            else
            {
                // DAT
                NEW_SEGMENT(ret, allocator, DataSegment);
            }
        }

        IZ_ASSERT(ret != nullptr);

        if (ret)
        {
            ret->m_allocator = allocator;
            ret->ReadBytes(allocator, bytes, offset, length);
        }

        return ret;
    }

    // データ書き込み.
    void Segment::WriteBytes(IOutputStream* wr)
    {
        wr->Write((IZ_UINT8)m_Flags);
        wr->Write((IZ_UINT8)m_HeaderLength);
        wr->Write((IZ_UINT8)m_SequenceNumber);
        wr->Write((IZ_UINT8)m_AckNumber);
    }

    // データ読み込み.
    void Segment::ReadBytes(
        IMemoryAllocator* allocator,
        IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length)
    {
        m_Flags = bytes[offset + 0];
        m_HeaderLength = bytes[offset + 1];
        m_SequenceNumber = bytes[offset + 2];
        m_AckNumber = bytes[offset + 3];
    }

    void Segment::ToString(IZ_CHAR* buf, IZ_UINT length)
    {
        IZ_SPRINTF(
            buf,
            length,
            "%d SEQ:%d ACK:%d",
            GetSegmentType(),
            m_SequenceNumber,
            (GetAcknowledgedNumber() >= 0 ? "" + GetAcknowledgedNumber() : "N/A"));
    }
}   // namespace net
}   // namespace izanagi
