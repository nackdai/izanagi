#if !defined(__IZANAGI_ANIMATION_SPLINE_INTERPOLATOR_H__)
#define __IZANAGI_ANIMATION_SPLINE_INTERPOLATOR_H__

#include "izDefs.h"
#include "izStd.h"

#include "Interpolator.h"
#include "Timeline.h"

namespace izanagi
{
namespace animation
{
    class CSplineInterpolator : public CInterpolator {
    public:
        static CSplineInterpolator* Create(
            IMemoryAllocator* allocator,
            IZ_FLOAT from, IZ_FLOAT to,
            IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
            IZ_FLOAT cp2X, IZ_FLOAT cp2Y);

    private:
        CSplineInterpolator();
        virtual ~CSplineInterpolator();

    private:
        void Init(
            IZ_FLOAT from, IZ_FLOAT to,
            IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
            IZ_FLOAT cp2X, IZ_FLOAT cp2Y);

        struct Func {
            Func(CSplineInterpolator* interp, IZ_BOOL isDerivation);
            IZ_FLOAT operator()(IZ_FLOAT t);

            CSplineInterpolator* interpolator;
            IZ_BOOL isDerivation;
        };

        IZ_FLOAT BezierX(IZ_FLOAT t);
        IZ_FLOAT BezierY(IZ_FLOAT t);
        IZ_FLOAT BezierD(IZ_FLOAT t);

        IZ_FLOAT ComputeX(
            Func& func,
            Func& funcd,
            IZ_FLOAT time,
            IZ_FLOAT threshold,
            IZ_UINT loopCnt);

    public:
        virtual IZ_FLOAT GetValue(const CTimeline& timeline);

    private:
        CFloatPoint m_Cp1;
        CFloatPoint m_Cp2;
        CFloatPoint m_Cp3;
        CFloatPoint m_Cp4;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_SPLINE_INTERPOLATOR_H__)
