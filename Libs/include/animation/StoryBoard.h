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

        IZ_BOOL IsRegistered(CInterpolator* animation);

        IZ_FLOAT GetDuration();

        IZ_FLOAT GetValue(const CTimeline& timeline);

    private:
        void CheckTime(const CTimeline& timeline);

    private:
        class Element : public CPlacementNew {
        public:
            Element();
            ~Element();

            CStdList<Element>::Item item;
            CInterpolator* animation;
            IZ_FLOAT begin;
            IZ_FLOAT end;
        };

    private:
        IMemoryAllocator* m_Allocator;

        CStdList<Element> m_List;

        Element* m_CurElement;
        Element* m_NextElement;

        IZ_BOOL m_IsForward;
    };
}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_STORY_BOARD_H__)
