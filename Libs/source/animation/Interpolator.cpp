#include "animation/Interpolator.h"

namespace izanagi
{
namespace animation
{
    CInterpolator::CInterpolator()
    {
        m_Allocator = IZ_NULL;
        m_From = 0.0f;
        m_To = 0.0f;
    }
}   // namespace izanagi
}   // namespace animation
