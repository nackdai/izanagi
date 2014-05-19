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

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void Advance(IZ_FLOAT delta);

        void EnableLoop(IZ_BOOL enable);

        void AutoReverse(IZ_BOOL enable);

        void Start();
        void Pause();
        void Stop();

        PURE_VIRTUAL(IZ_FLOAT GetValue());

    private:
        CTimeline& GetTimeline() { return m_Timeline; }

        PURE_VIRTUAL(IZ_FLOAT GetValue(IZ_FLOAT time, IZ_FLOAT duration));

    protected:
        IMemoryAllocator* m_Allocator;

        CTimeline m_Timeline;

        IZ_FLOAT m_From;
        IZ_FLOAT m_To;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_INTERPOLATOR_H__)
