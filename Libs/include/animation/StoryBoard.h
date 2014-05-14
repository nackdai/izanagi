#if !defined(__IZANAGI_ANIMATION_STORY_BOARD_H__)
#define __IZANAGI_ANIMATION_STORY_BOARD_H__

#include "izDefs.h"
#include "izStd.h"

#include "Timeline.h"

namespace izanagi
{
namespace animation
{
    class CInterpolator;

    /**
     */
    class CStoryBoard : public CObject {
    public:
        static CStoryBoard* Create(IMemoryAllocator* allocator);

    public:
        void SetTarget(DependencyObjectBase* object);

        void Add(
            CInterpolator* animation,
            const DependencyProperty& prop);

        void Advance(IZ_FLOAT delta);

        void Pause();
        void Resume();

    private:
        class CTimeOverHandler : public CTimeline::CTimeOverHandler {
        public:
            CTimeOverHandler() {}
            virtual ~CTimeOverHandler() {}

            virtual void Handle(const CTimeline& timeline);
        };

    private:
        CStdList<CInterpolator> m_IterpList;

        CInterpolator* m_CurAnimation;

        CTimeOverHandler m_Handler;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_STORY_BOARD_H__)
