#include "animation/EasingInterpolator.h"

namespace izanagi
{
namespace animation
{
    CEasingInterpolator* CEasingInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT from, IZ_FLOAT to,
        IZ_FLOAT duration,
        E_ANM_TWEENER_MODE mode)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CEasingInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CEasingInterpolator* ret = new(buf) CEasingInterpolator;
        ret->AddRef();
        ret->m_Allocator = allocator;
        ret->Init(from, to, duration, mode);

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
        IZ_FLOAT duration,
        E_ANM_TWEENER_MODE mode)
    {
        m_From = from;
        m_To = to;

        m_Timeline.Init(duration, 0.0f);
        m_Tweener.Init(mode, 0.0f, 1.0f);
    }

    IZ_FLOAT CEasingInterpolator::GetValue(IZ_FLOAT time, IZ_FLOAT duration)
    {
        IZ_FLOAT t = m_Tweener.GetValue(time, duration);
        IZ_FLOAT ret = m_From + (m_To - m_From) * t;
        return ret;
    }

    IZ_FLOAT CEasingInterpolator::GetValue()
    {
        IZ_FLOAT t = m_Tweener.GetValue(m_Timeline);
        IZ_FLOAT ret = m_From + (m_To - m_From) * t;
        return ret;
    }
}   // namespace izanagi
}   // namespace animation
