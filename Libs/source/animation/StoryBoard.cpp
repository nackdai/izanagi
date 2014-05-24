#include "animation/StoryBoard.h"
#include "animation/Interpolator.h"

namespace izanagi
{
namespace animation
{
    CStoryBoard::Element::Element()
    {
        animation = IZ_NULL;
        item.Init(this);

        begin = 0.0f;
        end = 0.0f;
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

        m_IsForward = IZ_TRUE;
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

    IZ_BOOL CStoryBoard::Add(
        CInterpolator* animation,
        IZ_FLOAT duration)
    {
        IZ_ASSERT(animation != IZ_NULL);

        if (IsRegistered(animation)) {
            return IZ_TRUE;
        }

        void* buf = ALLOC_ZERO(m_Allocator, sizeof(Element));
        VRETURN(buf != IZ_NULL);

        Element* element = new(buf) Element();
        SAFE_REPLACE(element->animation, animation);

        IZ_UINT pos = m_List.GetItemNum();

        if (pos == 0) {
            element->begin = 0;
        }
        else {
            CStdList<Element>::Item* item = m_List.GetAt(pos - 1);
            element->begin = item->GetData()->end;
        }

        element->end = element->begin + duration;
        
        m_List.AddTail(&element->item);

        return IZ_TRUE;
    }

    IZ_BOOL CStoryBoard::Remove(CInterpolator* animation)
    {
        IZ_BOOL result = IZ_FALSE;

        CStdList<Element>::Item* item = m_List.GetTop();

        while (item != IZ_NULL) {
            Element* element = item->GetData();

            if (element->animation == animation) {
                if (m_CurElement == element) {
                    CStdList<Element>::Item* next = IZ_NULL;
                    if (m_IsForward) {
                        next = item->GetNext();
                    }
                    else {
                        next = item->GetPrev();
                    }

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

                result = IZ_TRUE;
                break;
            }

            item = item->GetNext();
        }

        // Re-computing time.
        if (result) {
            item = m_List.GetTop();

            Element* prev = IZ_NULL;
            
            while (item != IZ_NULL) {
                Element* element = item->GetData();

                IZ_FLOAT duration = element->end - element->begin;

                if (prev == IZ_NULL) {
                    element->begin = 0.0f;
                }
                else {
                    element->begin = prev->end;
                }

                element->end = element->begin + duration;

                item = item->GetNext();
            }
        }

        return IZ_FALSE;
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

    IZ_FLOAT CStoryBoard::GetDuration()
    {
        CStdList<Element>::Item* item = m_List.GetTail();

        if (item != IZ_NULL) {
            Element* element = item->GetData();
            return element->end;
        }

        return 0.0f;
    }

    IZ_FLOAT CStoryBoard::GetValue(const CTimeline& timeline)
    {
        CheckTime(timeline);

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
            IZ_FLOAT duration = m_CurElement->end - m_CurElement->begin;

            IZ_FLOAT time = timeline.GetTime();
            time = time - m_CurElement->begin;

            IZ_FLOAT ret = m_CurElement->animation->GetValueEx(time, duration);
            return ret;
        }

        return 0.0f;
    }

    void CStoryBoard::CheckTime(const CTimeline& timeline)
    {
        IZ_ASSERT(timeline.GetDuration() == GetDuration());

        if (m_CurElement == IZ_NULL) {
            return;
        }

        // 進行方向に応じて次のアニメーションを選ぶ
        if (timeline.IsForward()
            && m_CurElement->end < timeline.GetTime())
        {
            CStdList<Element>::Item* item = m_CurElement->item.GetNext();

            if (item == IZ_NULL) {
                // 次がないので逆方向にする
                m_IsForward = !m_IsForward;
                item = m_List.GetTail();

                if (item != IZ_NULL) {
                    m_NextElement = item->GetData();
                }
                else {
                    m_NextElement = IZ_NULL;
                }
            }
            else {
                m_NextElement = item->GetData();
            }
        }
        else if (m_CurElement->begin > timeline.GetTime()) {
            CStdList<Element>::Item* item = m_CurElement->item.GetPrev();

            if (item == IZ_NULL) {
                // 次がないので逆方向にする
                m_IsForward = !m_IsForward;
                item = m_List.GetTop();

                if (item != IZ_NULL) {
                    m_NextElement = item->GetData();
                }
                else {
                    m_NextElement = IZ_NULL;
                }
            }
            else {
                m_NextElement = item->GetData();
            }
        }
    }
}   // namespace animation
}   // namespace izanagi
