#include "animation/EasingInterpolator.h"

namespace izanagi
{
namespace animation
{
    CEasingInterpolator* CEasingInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT target,
        IZ_FLOAT keytime,
        E_ANM_TWEENER_MODE mode)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CEasingInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CEasingInterpolator* ret = new(buf) CEasingInterpolator;
        ret->AddRef();
        ret->m_Allocator = allocator;
        ret->Init(target, keytime, mode);

        return ret;
    }

    CEasingInterpolator::CEasingInterpolator()
    {
    }

    CEasingInterpolator::~CEasingInterpolator()
    {
    }

    void CEasingInterpolator::Init(
        IZ_FLOAT target,
        IZ_FLOAT keytime,
        E_ANM_TWEENER_MODE mode)
    {
        m_Value = target;
        m_Timeline.Init(keytime, 0.0f);
        m_Tweener.Init(mode, 0.0f, 1.0f);
    }

    IZ_FLOAT CEasingInterpolator::GetValue()
    {
        IZ_FLOAT t = m_Tweener.GetValue(m_Timeline);
        IZ_FLOAT ret = m_Value * t;
        return ret;
    }
}   // namespace izanagi
}   // namespace animation
