#include "graph/GraphPerf.h"

namespace izanagi
{
namespace graph
{
    /**
    * イベント開始
    */
    IZ_INT CGraphPerf::BeginEvent(
        IZ_COLOR color,
        IZ_PCSTR pszFormat,
        ...)
    {
        UNUSED_ALWAYS(color);
        UNUSED_ALWAYS(pszFormat);
        return 0;
    }

    /**
    * イベント終了
    */
    IZ_INT CGraphPerf::EndEvent()
    {
        return 0;
    }
}   // namespace graph
}   // namespace izanagi
