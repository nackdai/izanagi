#include "animation/LinearInterpolator.h"

namespace izanagi
{
namespace animation
{
    CLinearInterpolator* CLinearInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT from, IZ_FLOAT to,
        IZ_FLOAT duration)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CLinearInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CLinearInterpolator* ret = new(buf) CLinearInterpolator;
        ret->AddRef();
        ret->m_Allocator = allocator;
        ret->Init(from, to, duration);

        return ret;
    }

    CLinearInterpolator::CLinearInterpolator()
    {
    }

    CLinearInterpolator::~CLinearInterpolator()
    {
    }

    void CLinearInterpolator::Init(
        IZ_FLOAT from, IZ_FLOAT to,
        IZ_FLOAT duration)
    {
        m_From = from;
        m_To = to;
        m_Timeline.Init(duration, 0.0f);
    }

    IZ_FLOAT CLinearInterpolator::GetValue()
    {
        IZ_FLOAT t = m_Timeline.GetNormalized();
        IZ_FLOAT ret = m_From + (m_To - m_From) * t;
        return ret;
    }
}   // namespace izanagi
}   // namespace animation
