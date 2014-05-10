#include "animation/SplineInterpolator.h"
#include "izMath.h"

namespace izanagi
{
namespace animation
{
    CSplineInterpolator* CSplineInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT target,
        IZ_FLOAT keytime,
        IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
        IZ_FLOAT cp2X, IZ_FLOAT cp2Y)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CSplineInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CSplineInterpolator* ret = new(buf) CSplineInterpolator;
        ret->m_Allocator = allocator;
        ret->Init(
            target, keytime,
            cp1X, cp1Y,
            cp2X, cp2Y);

        return ret;
    }

    CSplineInterpolator::CSplineInterpolator()
    {
    }

    CSplineInterpolator::~CSplineInterpolator()
    {
    }

    void CSplineInterpolator::Init(
        IZ_FLOAT target,
        IZ_FLOAT keytime,
        IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
        IZ_FLOAT cp2X, IZ_FLOAT cp2Y)
    {
        m_Value = target;
        m_Cp1X = math::CMath::Clamp(cp1X, 0.0f, 1.0f);
        m_Cp1Y = math::CMath::Clamp(cp1Y, 0.0f, 1.0f);
        m_Cp2X = math::CMath::Clamp(cp2X, 0.0f, 1.0f);
        m_Cp2Y = math::CMath::Clamp(cp2Y, 0.0f, 1.0f);
        m_Timeline.Init(keytime, 0.0f);
    }

    void CSplineInterpolator::Advance(IZ_FLOAT delta)
    {
        m_Timeline.Advance(delta);
    }

    IZ_FLOAT CSplineInterpolator::GetValueX()
    {
        IZ_FLOAT t = m_Timeline.GetNormalized();

        IZ_FLOAT j_1_0_t = 1.0f - t;
        IZ_FLOAT j_1_1_t = t;

        IZ_FLOAT ret = m_Cp1X * (1.0f - t) + m_Cp2X * t;

        return ret;
    }

    IZ_FLOAT CSplineInterpolator::GetValueY()
    {
        IZ_FLOAT t = m_Timeline.GetNormalized();

        IZ_FLOAT j_1_0_t = 1.0f - t;
        IZ_FLOAT j_1_1_t = t;

        IZ_FLOAT ret = m_Cp1Y * (1.0f - t) + m_Cp2Y * t;

        return ret;
    }

    void CSplineInterpolator::GetValue(IZ_FLOAT* x, IZ_FLOAT* y)
    {
        if (x) {
            *x = GetValueX();
        }

        if (y) {
            *y = GetValueY();
        }
    }
}   // namespace izanagi
}   // namespace animation
