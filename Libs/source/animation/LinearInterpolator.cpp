#include "animation/LinearInterpolator.h"

namespace izanagi
{
namespace animation
{
    CLinearInterpolator* CLinearInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT from, IZ_FLOAT to)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CLinearInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CLinearInterpolator* ret = new(buf) CLinearInterpolator;
        ret->AddRef();
        ret->m_Allocator = allocator;
        ret->Init(from, to);

        return ret;
    }

    CLinearInterpolator::CLinearInterpolator()
    {
    }

    CLinearInterpolator::~CLinearInterpolator()
    {
    }

    void CLinearInterpolator::Init(IZ_FLOAT from, IZ_FLOAT to)
    {
        m_From = from;
        m_To = to;
    }

    IZ_FLOAT CLinearInterpolator::GetValueEx(IZ_FLOAT time, IZ_FLOAT duration)
    {
        IZ_FLOAT t = (duration > 0.0f ? time / duration : 0.0f);
        IZ_FLOAT ret = m_From + (m_To - m_From) * t;
        return ret;
    }
}   // namespace izanagi
}   // namespace animation
