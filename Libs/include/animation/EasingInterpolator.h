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
            IZ_FLOAT from, IZ_FLOAT to,
            IZ_FLOAT duration,
            E_ANM_TWEENER_MODE mode);

    private:
        CEasingInterpolator();
        virtual ~CEasingInterpolator();

    private:
        void Init(
            IZ_FLOAT from, IZ_FLOAT to,
            IZ_FLOAT duration,
            E_ANM_TWEENER_MODE mode);

        virtual IZ_FLOAT GetValue(IZ_FLOAT time, IZ_FLOAT duration);

    public:
        virtual IZ_FLOAT GetValue();

    private:
        CTweener m_Tweener;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_EASING_INTERPOLATOR_H__)
