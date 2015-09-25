#if !defined(_IZANAGI_NETWORK_SEGMENT_H__)
#define _IZANAGI_NETWORK_SEGMENT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSystem.h"

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
namespace network {
    class Segment : public CPlacementNew {
    public:
        static const IZ_UINT RUDP_VERSION = 1;
        static const IZ_UINT RUDP_HEADER_LEN = 6;

        enum Type : IZ_UINT
        {
            SYN = 0x80, // SYNchronous
            ACK = 0x40, // ACKnowledged
            EAK = 0x20, // Extend AcKnowledged
            RST = 0x10, // ReSeT
            NUL = 0x08, // NULl
            CHK = 0x04, // CHecK
            FIN = 0x02, // FINish

            DAT = 0x01, // DATa
        };

    protected:
        Segment() {}

        Segment(
            Type type,
            IZ_INT sequenceNumber,
            IZ_UINT hederSize);

        virtual ~Segment();

    protected:
        virtual Type GetType() = 0;

        virtual const char* GetSegmentType() = 0;

        // 応答番号をセット.
        // この番号と一致する番号を持つセグメントに対する応答となる.
        void SetAcknowledgedNumber(IZ_INT ackNumber);

        // 応答番号を取得.
        IZ_INT GetAcknowledgedNumber();

        // データをパースして、対応するセグメントを作成する.
        static Segment* Parse(
            IMemoryAllocator* allocator,
            IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length);

        // データ書き込み.
        virtual void WriteBytes(IOutputStream* wr);

        // データ読み込み.
        virtual void ReadBytes(
            IMemoryAllocator* allocator,
            IZ_UINT8* bytes, IZ_UINT offset, IZ_UINT length);

        void ToString(IZ_CHAR* buf, IZ_UINT length);

    protected:
        // フラグチェック.
        static IZ_BOOL CheckFlag(IZ_UINT flag, Type type);

        // フラグチェック.
        IZ_BOOL CheckFlag(Type type);

    protected:
        IMemoryAllocator* m_allocator;

        IZ_UINT m_Flags{ 0 };

        IZ_UINT m_HeaderLength{ 0 };

        IZ_INT m_SequenceNumber{ -1 };

        IZ_INT m_AckNumber{ -1 };

        IZ_UINT m_RetryCount{ 0 };
    };
}   // namespace network
}   // namespace izanagi

#endif  // _IZANAGI_NETWORK_SEGMENT_H__
