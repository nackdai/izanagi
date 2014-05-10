#if !defined(__IZANAGI_ANIMATION_LINEAR_INTERPOLATOR_H__)
#define __IZANAGI_ANIMATION_LINEAR_INTERPOLATOR_H__

#include "izDefs.h"
#include "izStd.h"

#include "Interpolator.h"
#include "Timeline.h"

namespace izanagi
{
namespace animation
{
    class CLinearInterpolator : public CInterpolator {
    public:
        static CLinearInterpolator* Create(
            IMemoryAllocator* allocator,
            IZ_FLOAT target,
            IZ_FLOAT keytime);

    private:
        CLinearInterpolator();
        virtual ~CLinearInterpolator();

        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        void Init(
            IZ_FLOAT target,
            IZ_FLOAT keytime);

    public:
        virtual void Advance(IZ_FLOAT delta);

        IZ_FLOAT GetValue();

    private:
        IZ_FLOAT m_Value;
        CTimeline m_Timeline;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_LINEAR_INTERPOLATOR_H__)
