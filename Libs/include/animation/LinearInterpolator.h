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
            IZ_FLOAT from, IZ_FLOAT to,
            IZ_FLOAT duration);

    private:
        CLinearInterpolator();
        virtual ~CLinearInterpolator();

        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        void Init(
            IZ_FLOAT from, IZ_FLOAT to,
            IZ_FLOAT duration);

    public:
        IZ_FLOAT GetValue();        
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_LINEAR_INTERPOLATOR_H__)
