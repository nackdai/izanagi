#if !defined(_IZANAGI_NETWORK_RUDP_COUNTER_H__)
#define _IZANAGI_NETWORK_RUDP_COUNTER_H__

#include <atomic>
#include "izDefs.h"

namespace izanagi {
namespace network {
    class RUDPCounter
    {
    public:
        RUDPCounter() {}
        ~RUDPCounter() {}

        NO_COPIABLE(RUDPCounter);

    public:

        inline IZ_INT SetSequenceNumber(int n)
        {
            m_SequenceNumber = n;
            return m_SequenceNumber;
        }

        inline IZ_INT NextSequenceNumber()
        {
            return ++m_SequenceNumber;
        }

        inline IZ_INT GetOutStandingSegmentCount()
        {
            return m_OutStandingSegmentCount;
        }

        inline IZ_INT IncrementOutStandingSegmentCount()
        {
            return ++m_OutStandingSegmentCount;
        }

        // 現在のシーケンス番号を設定.
        inline IZ_INT SetLastInSequenceNumber(IZ_INT n)
        {
            m_LastInSequenceNumber = n;
            return m_LastInSequenceNumber;
        }

        // 現在のシーケンス番号を取得.
        inline IZ_INT GetLastInSequenceNumber()
        {
            return m_LastInSequenceNumber;
        }

    private:
        std::atomic<IZ_INT> m_SequenceNumber{ -1 };

        // 送信されたが返事のないセグメントの数.
        std::atomic<IZ_INT> m_OutStandingSegmentCount{ 0 };

        // 現在のシーケンス番号.
        std::atomic<IZ_INT> m_LastInSequenceNumber{ -1 };
    };
}   // namespace network
}   // namespace izanagi

#endif  // #if !defined(_IZANAGI_NETWORK_RUDP_COUNTER_H__)

