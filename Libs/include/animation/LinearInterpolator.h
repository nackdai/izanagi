#if !defined(__IZANAGI_ANIMATION_LINEAR_INTERPOLATOR_H__)
#define __IZANAGI_ANIMATION_LINEAR_INTERPOLATOR_H__

#include "izDefs.h"
#include "izStd.h"

#include "Interpolator.h"

namespace izanagi
{
namespace animation
{
    class CLinearInterpolator : public CInterpolator {
    public:
        static CLinearInterpolator* Create(
            IMemoryAllocator* allocator,
            IZ_FLOAT from, IZ_FLOAT to);

    private:
        CLinearInterpolator();
        virtual ~CLinearInterpolator();

    private:
        void Init(IZ_FLOAT from, IZ_FLOAT to);

    public:
        virtual IZ_FLOAT GetValue(const CTimeline& timeline);
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_LINEAR_INTERPOLATOR_H__)
