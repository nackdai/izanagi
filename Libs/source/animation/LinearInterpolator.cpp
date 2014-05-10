#include "animation/LinearInterpolator.h"

namespace izanagi
{
namespace animation
{
    CLinearInterpolator* CLinearInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT target,
        IZ_FLOAT keytime,
        IZ_BOOL loop)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CLinearInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CLinearInterpolator* ret = new(buf) CLinearInterpolator;
        ret->m_Allocator = allocator;
        ret->Init(target, keytime, loop);

        return ret;
    }

    CLinearInterpolator::CLinearInterpolator()
    {
    }

    CLinearInterpolator::~CLinearInterpolator()
    {
    }

    void CLinearInterpolator::Init(
        IZ_FLOAT target,
        IZ_FLOAT keytime,
        IZ_BOOL loop)
    {
        m_Value = target;
        m_Timeline.Init(keytime, 0.0f);
        m_Timeline.EnableLoop(loop);
    }

    void CLinearInterpolator::Advance(IZ_FLOAT delta)
    {
        m_Timeline.Start();
        m_Timeline.Advance(delta);
    }

    IZ_FLOAT CLinearInterpolator::GetValue()
    {
        IZ_FLOAT t = m_Timeline.GetNormalized();
        IZ_FLOAT ret = m_Value * t;
        return ret;
    }
}   // namespace izanagi
}   // namespace animation
