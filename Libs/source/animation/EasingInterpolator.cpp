#include "animation/EasingInterpolator.h"

namespace izanagi
{
namespace animation
{
    CEasingInterpolator* CEasingInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT from, IZ_FLOAT to,
        E_ANM_TWEENER_MODE mode)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CEasingInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CEasingInterpolator* ret = new(buf) CEasingInterpolator;
        ret->AddRef();
        ret->m_Allocator = allocator;
        ret->Init(from, to, mode);

        return ret;
    }

    CEasingInterpolator::CEasingInterpolator()
    {
    }

    CEasingInterpolator::~CEasingInterpolator()
    {
    }

    void CEasingInterpolator::Init(
        IZ_FLOAT from, IZ_FLOAT to,
        E_ANM_TWEENER_MODE mode)
    {
        m_From = from;
        m_To = to;

        m_Tweener.Init(mode, 0.0f, 1.0f);
    }

    IZ_FLOAT CEasingInterpolator::GetValue(const CTimeline& timeline)
    {
        IZ_FLOAT t = m_Tweener.GetValue(timeline);
        IZ_FLOAT ret = m_From + (m_To - m_From) * t;
        return ret;
    }
}   // namespace izanagi
}   // namespace animation
