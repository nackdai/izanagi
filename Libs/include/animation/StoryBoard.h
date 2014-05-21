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

    private:
        CStoryBoard();
        virtual ~CStoryBoard();

        NO_COPIABLE(CStoryBoard);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_BOOL Add(
            CInterpolator* animation,
            IZ_FLOAT duration);

        IZ_BOOL Remove(CInterpolator* animation);

        void Advance(IZ_FLOAT delta);

        void Start();
        void Pause();
        void Stop();

        IZ_BOOL IsRegistered(CInterpolator* animation);

        IZ_FLOAT GetValue();

    private:
        class Element : public CPlacementNew {
        public:
            Element();
            ~Element();

            CStdList<Element>::Item item;
            CInterpolator* animation;
            CTimeline timeline;
        };

        class CTimeOverHandler : public CTimeline::CTimeOverHandler {
        public:
            CTimeOverHandler();
            virtual ~CTimeOverHandler();

            virtual void Handle(const CTimeline& timeline);

            CStoryBoard* storyboard;
        };

    private:
        IMemoryAllocator* m_Allocator;

        CStdList<Element> m_List;

        Element* m_CurElement;
        Element* m_NextElement;

        CTimeOverHandler m_Handler;

        IZ_BOOL m_IsForward;
        IZ_BOOL m_IsPaused;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_STORY_BOARD_H__)
