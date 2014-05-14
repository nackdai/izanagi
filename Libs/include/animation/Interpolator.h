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
        friend class CStoryBoard;

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

        IZ_FLOAT m_From;
        IZ_FLOAT m_To;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_INTERPOLATOR_H__)
