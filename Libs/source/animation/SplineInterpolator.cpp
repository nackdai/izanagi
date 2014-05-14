#include "animation/SplineInterpolator.h"
#include "izMath.h"

namespace izanagi
{
namespace animation
{
    CSplineInterpolator* CSplineInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT from, IZ_FLOAT to,
        IZ_FLOAT duration,
        IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
        IZ_FLOAT cp2X, IZ_FLOAT cp2Y)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CSplineInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CSplineInterpolator* ret = new(buf) CSplineInterpolator;
        ret->AddRef();
        ret->m_Allocator = allocator;
        ret->Init(
            from, to,
            duration,
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
        IZ_FLOAT from, IZ_FLOAT to,
        IZ_FLOAT duration,
        IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
        IZ_FLOAT cp2X, IZ_FLOAT cp2Y)
    {
        m_From = from;
        m_To = to;

        m_Cp1.x = 0.0f;
        m_Cp1.y = 0.0f;
        
        m_Cp2.x = math::CMath::Clamp(cp1X, 0.0f, 1.0f);
        m_Cp2.y = math::CMath::Clamp(cp1Y, 0.0f, 1.0f);

        m_Cp3.x = math::CMath::Clamp(cp2X, 0.0f, 1.0f);
        m_Cp3.y = math::CMath::Clamp(cp2Y, 0.0f, 1.0f);

        m_Cp4.x = 1.0f;
        m_Cp4.y = 1.0f;

        m_Timeline.Init(duration, 0.0f);
    }

    IZ_FLOAT CSplineInterpolator::GetValueX()
    {
        IZ_FLOAT t = m_Timeline.GetNormalized();

        IZ_FLOAT ret = (1.0f - t) * (1.0f - t) * (1.0f - t) * m_Cp1.x + 3.0f * (1.0f - t) * (1.0f - t) * t * m_Cp2.x + 3.0f * (1.0f - t) * t * t * m_Cp3.x + t * t * t * m_Cp4.x;
        ret = m_From + (m_To - m_From) * ret;

        return ret;
    }

    IZ_FLOAT CSplineInterpolator::GetValueY()
    {
        IZ_FLOAT t = m_Timeline.GetNormalized();

        IZ_FLOAT j_1_0_t = 1.0f - t;
        IZ_FLOAT j_1_1_t = t;

        IZ_FLOAT ret = (1.0f - t) * (1.0f - t) * (1.0f - t) * m_Cp1.y + 3.0f * (1.0f - t) * (1.0f - t) * t * m_Cp2.y + 3.0f * (1.0f - t) * t * t * m_Cp3.y + t * t * t * m_Cp4.y;
        ret = m_From + (m_To - m_From) * ret;

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
