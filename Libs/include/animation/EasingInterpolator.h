#if !defined(__IZANAGI_ANIMATION_EASING_INTERPOLATOR_H__)
#define __IZANAGI_ANIMATION_EASING_INTERPOLATOR_H__

#include "izDefs.h"
#include "izStd.h"

#include "Interpolator.h"
#include "Timeline.h"
#include "TweenerUtility.h"

namespace izanagi
{
namespace animation
{
    class CEasingInterpolator : public CInterpolator {
    public:
        static CEasingInterpolator* Create(
            IMemoryAllocator* allocator,
            IZ_FLOAT target,
            IZ_FLOAT keytime,
            E_ANM_TWEENER_MODE mode);

    private:
        CEasingInterpolator();
        virtual ~CEasingInterpolator();

        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        void Init(
            IZ_FLOAT target,
            IZ_FLOAT keytime,
            E_ANM_TWEENER_MODE mode);

    public:
        virtual void Advance(IZ_FLOAT delta);

        IZ_FLOAT GetValue();

    private:
        IZ_FLOAT m_Value;
        CTimeline m_Timeline;

        CTweener m_Tweener;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_EASING_INTERPOLATOR_H__)
