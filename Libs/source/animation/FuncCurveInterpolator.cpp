#include "animation/FuncCurveInterpolator.h"
#include "animation/Timeline.h"
#include "math/MathInterpolator.h"

namespace izanagi {
namespace animation {
    IZ_FLOAT CFuncCurveInterpolator::GetValue(const CTimeline& timeline)
    {
        IZ_FLOAT time = timeline.GetTime();
        IZ_FLOAT duration = timeline.GetDuration();

        IZ_FLOAT t = time / duration;

        IZ_FLOAT ret = GetValue(t);

        return ret;
    }

    IZ_FLOAT CFuncCurveInterpolator::GetValue(IZ_FLOAT t)
    {
        typedef IZ_FLOAT (*Func)(IZ_FLOAT, const math::SVector&);

        static Func funcs[] =
        {
            &math::CMathInterpolator::ComputeBezier,
            &math::CMathInterpolator::ComputeHermite,
            &math::CMathInterpolator::ComputeCatmullRom,
        };

        IZ_C_ASSERT(COUNTOF(funcs) == E_ANM_FUNC_CURVE_MODE_NUM);

        IZ_FLOAT ret = (*funcs[m_Mode])(t, m_Param);
        return ret;
    }
}   // namespace animation
}   // namespace izanagi
