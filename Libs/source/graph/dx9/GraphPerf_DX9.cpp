#include "graph/dx9/izDX9Defs.h"
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
#ifdef __IZ_DEBUG__
        static const IZ_UINT BUF_SIZE = 256;
        static wchar_t buf[BUF_SIZE];

        va_list argp;
        izChar tmp[256];
        va_start(argp, pszFormat);
        IZ_VSPRINTF(tmp, sizeof(tmp), pszFormat, argp);
        va_end(argp);

        size_t len = strlen(pszFormat);
        IZ_ASSERT(len <= BUF_SIZE);

        mbstowcs_s(&len, buf, tmp, BUF_SIZE);
        IZ_ASSERT(len >= 0);

        IZ_INT ret = D3DPERF_BeginEvent(color, buf);
        //IZ_ASSERT(ret >= 0);

        return ret;
#else   // #ifdef __IZ_DEBUG__
        UNUSED_ALWAYS(color);
        UNUSED_ALWAYS(pszFormat);
        return 0;
#endif  // #ifdef __IZ_DEBUG__
    }

    /**
    * イベント終了
    */
    IZ_INT CGraphPerf::EndEvent()
    {
#ifdef __IZ_DEBUG__
        IZ_INT ret = D3DPERF_EndEvent();
        //IZ_ASSERT(ret >= 0);
        return ret;
#else   // #ifdef __IZ_DEBUG__
        return 0;
#endif  // #ifdef __IZ_DEBUG__
    }
}   // namespace graph
}   // namespace izanagi
