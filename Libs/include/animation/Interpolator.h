#if !defined(__IZANAGI_ANIMATION_INTERPOLATOR_H__)
#define __IZANAGI_ANIMATION_INTERPOLATOR_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
namespace animation
{
    class CInterpolator : public CObject {
    protected:
        CInterpolator();
        virtual ~CInterpolator() {}

    public:
        PURE_VIRTUAL(void Advance(IZ_FLOAT delta));

    private:
        CStdList<CInterpolator>::Item* GetListItem() { return &m_ListItem; }

    protected:
        IMemoryAllocator* m_Allocator;
        CStdList<CInterpolator>::Item m_ListItem;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_INTERPOLATOR_H__)
