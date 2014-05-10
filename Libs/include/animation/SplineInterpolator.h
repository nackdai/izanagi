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
            IZ_FLOAT target,
            IZ_FLOAT keytime,
            IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
            IZ_FLOAT cp2X, IZ_FLOAT cp2Y);

    private:
        CSplineInterpolator();
        virtual ~CSplineInterpolator();

        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        void Init(
            IZ_FLOAT target,
            IZ_FLOAT keytime,
            IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
            IZ_FLOAT cp2X, IZ_FLOAT cp2Y);

    public:
        virtual void Advance(IZ_FLOAT delta);

        IZ_FLOAT GetValueX();
        IZ_FLOAT GetValueY();

        void GetValue(IZ_FLOAT* x, IZ_FLOAT* y);

    private:
        IZ_FLOAT m_Value;
        CTimeline m_Timeline;

        IZ_FLOAT m_Cp1X;
        IZ_FLOAT m_Cp1Y;
        IZ_FLOAT m_Cp2X;
        IZ_FLOAT m_Cp2Y;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_SPLINE_INTERPOLATOR_H__)
