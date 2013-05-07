#if !defined(__IZANAGI_GRAPH_PERF_H__)
#define __IZANAGI_GRAPH_PERF_H__

#include "izDefs.h"

namespace izanagi
{
namespace graph
{
    class CGraphPerf {
    private:
        CGraphPerf();
        ~CGraphPerf();

        CGraphPerf(const CGraphPerf& rhs);
        const CGraphPerf& operator=(const CGraphPerf& rhs);

    public:
        static IZ_INT BeginEvent(
            IZ_COLOR color,
            IZ_PCSTR pszFormat,
            ...);

        static IZ_INT EndEvent();
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_PERF_H__)
