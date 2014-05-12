#if !defined(__IZANAGI_ANIMATION_INTERPOLATOR_H__)
#define __IZANAGI_ANIMATION_INTERPOLATOR_H__

#include "izDefs.h"
#include "izStd.h"

#include "Timeline.h"

namespace izanagi
{
namespace animation
{
    class CInterpolator : public CObject {
    protected:
        CInterpolator();
        virtual ~CInterpolator() {}

    public:
        void Advance(IZ_FLOAT delta);

        void EnableLoop(IZ_BOOL enable);

        void AutoReverse(IZ_BOOL enable);

    private:
        CStdList<CInterpolator>::Item* GetListItem() { return &m_ListItem; }

    protected:
        CTimeline& TimeLine() { return m_Timeline; }

    protected:
        IMemoryAllocator* m_Allocator;
        CStdList<CInterpolator>::Item m_ListItem;

        CTimeline m_Timeline;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_INTERPOLATOR_H__)
