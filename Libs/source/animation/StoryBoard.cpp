#include "animation/StoryBoard.h"
#include "animation/Interpolator.h"

namespace izanagi
{
namespace animation
{
    CStoryBoard::CTimeOverHandler::CTimeOverHandler()
    {
        storyboard = IZ_NULL;
    }

    CStoryBoard::CTimeOverHandler::~CTimeOverHandler()
    {
        storyboard = IZ_NULL;
    }

    void CStoryBoard::CTimeOverHandler::Handle(const CTimeline& timeline)
    {
        if (storyboard != IZ_NULL) {
            // 進行方向に応じて次のアニメーションを選ぶ
            if (storyboard->m_IsForward) {
                CStdList<Element>::Item* item = storyboard->m_CurElement->item.GetNext();

                if (item == IZ_NULL) {
                    // 次がないので逆方向にする
                    storyboard->m_IsForward = !storyboard->m_IsForward;
                    item = storyboard->m_List.GetTail();

                    if (item != IZ_NULL) {
                        storyboard->m_NextElement = item->GetData();
                    }
                    else {
                        storyboard->m_NextElement = IZ_NULL;
                    }
                }
                else {
                    storyboard->m_NextElement = item->GetData();
                }
            }
            else {
                CStdList<Element>::Item* item = storyboard->m_CurElement->item.GetPrev();

                if (item == IZ_NULL) {
                    // 次がないので逆方向にする
                    storyboard->m_IsForward = !storyboard->m_IsForward;
                    item = storyboard->m_List.GetTop();

                    if (item != IZ_NULL) {
                        storyboard->m_NextElement = item->GetData();
                    }
                    else {
                        storyboard->m_NextElement = IZ_NULL;
                    }
                }
                else {
                    storyboard->m_NextElement = item->GetData();
                }
            }

            // 超過分を次に持ち越す
            IZ_FLOAT over = storyboard->m_CurElement->timeline.GetOverTime();
            storyboard->m_CurElement->timeline.SetOverTime(0.0f);

            if (storyboard->m_NextElement != IZ_NULL) {
                storyboard->m_NextElement->timeline.SetOverTime(over);

                // 方向が違うので合わせる
                if (storyboard->m_NextElement->timeline.IsForward() != storyboard->m_IsForward) {
                    storyboard->m_NextElement->timeline.ToggleDirection();
                }
            }
        }
    }

    ///////////////////////////////////

    CStoryBoard::Element::Element()
    {
        animation = IZ_NULL;
        item.Init(this);
    }

    CStoryBoard::Element::~Element()
    {
        SAFE_RELEASE(animation);
    }

    ///////////////////////////////////

    CStoryBoard* CStoryBoard::Create(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CStoryBoard));
        VRETURN_NULL(buf != IZ_NULL);

        CStoryBoard* ret = new(buf) CStoryBoard;
        {
            ret->AddRef();
            ret->m_Allocator = allocator;
        }

        return ret;
    }

    CStoryBoard::CStoryBoard()
    {
        m_Allocator = IZ_NULL;

        m_CurElement = IZ_NULL;
        m_NextElement = IZ_NULL;

        m_Handler.storyboard = this;

        m_IsForward = IZ_TRUE;
        m_IsPaused = IZ_TRUE;
    }

    CStoryBoard::~CStoryBoard()
    {
        CStdList<Element>::Item* item = m_List.GetTop();

        while (item != IZ_NULL) {
            Element* element = item->GetData();
            item = item->GetNext();

            delete element;
            FREE(m_Allocator, element);
        }

        FREE(m_Allocator, this);
    }

    IZ_BOOL CStoryBoard::Add(CInterpolator* animation)
    {
        IZ_ASSERT(animation != IZ_NULL);

        if (IsRegistered(animation)) {
            return IZ_TRUE;
        }

        void* buf = ALLOC_ZERO(m_Allocator, sizeof(Element));
        VRETURN(buf != IZ_NULL);

        Element* element = new(buf) Element();
        SAFE_REPLACE(element->animation, animation);

        element->timeline.SetDuration(animation->GetTimeline().GetDuration());
        element->timeline.SetTimeOverHandler(&m_Handler);
        
        m_List.AddTail(&element->item);

        return IZ_TRUE;
    }

    IZ_BOOL CStoryBoard::Remove(CInterpolator* animation)
    {
        CStdList<Element>::Item* item = m_List.GetTop();

        while (item != IZ_NULL) {
            Element* element = item->GetData();

            if (element->animation == animation) {
                if (m_CurElement == element) {
                    CStdList<Element>::Item* next = item->GetNext();
                    if (next != IZ_NULL) {
                        m_CurElement = next->GetData();
                    }
                    else {
                        m_CurElement = IZ_NULL;
                    }
                }

                item->Leave();

                delete element;
                FREE(m_Allocator, element);

                return IZ_TRUE;
            }

            item = item->GetNext();
        }

        return IZ_FALSE;
    }

    void CStoryBoard::Advance(IZ_FLOAT delta)
    {
        if (m_NextElement != IZ_NULL) {
            m_CurElement = m_NextElement;
            m_NextElement = IZ_NULL;
        }

        if (m_CurElement == IZ_NULL) {
            CStdList<Element>::Item* item = m_List.GetTop();
            if (item != IZ_NULL) {
                Element* element = item->GetData();
                m_CurElement = element;
            }
        }

        if (m_CurElement != IZ_NULL) {
            if (!m_IsPaused) {
                m_CurElement->timeline.Start();
                m_CurElement->timeline.Advance(delta);
            }
        }
    }

    void CStoryBoard::Start()
    {
        m_IsPaused = IZ_FALSE;
    }

    void CStoryBoard::Pause()
    {
        m_IsPaused = IZ_TRUE;
    }

    void CStoryBoard::Stop()
    {
        CStdList<Element>::Item* item = m_List.GetTop();

        while (item != IZ_NULL) {
            Element* element = item->GetData();
            
            IZ_FLOAT time = (m_IsForward
                ? 0.0f
                : element->animation->GetTimeline().GetDuration());
            
            element->timeline.SetTimeForcibly(time);

            item = item->GetNext();
        }

        m_IsPaused = IZ_TRUE;
    }

    IZ_BOOL CStoryBoard::IsRegistered(CInterpolator* animation)
    {
        IZ_ASSERT(animation != IZ_NULL);

        CStdList<Element>::Item* item = m_List.GetTop();

        while (item != IZ_NULL) {
            Element* element = item->GetData();

            if (element->animation == animation) {
                return IZ_TRUE;
            }

            item = item->GetNext();
        }

        return IZ_FALSE;
    }

    IZ_FLOAT CStoryBoard::GetValue()
    {
        if (m_CurElement != IZ_NULL) {
            IZ_FLOAT time = m_CurElement->timeline.GetTime();
            IZ_FLOAT duration = m_CurElement->timeline.GetDuration();

            IZ_FLOAT ret = m_CurElement->animation->GetValue(time, duration);
            return ret;
        }

        return 0.0f;
    }
}   // namespace animation
}   // namespace izanagi
