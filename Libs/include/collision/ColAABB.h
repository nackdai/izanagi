#if !defined(__IZANAGI_COLLISION_AABB_H__)
#define __IZANAGI_COLLISION_AABB_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    /**
     */
    struct SAABB {
    };

    /**
     */
    class CAABB : public SAABB {
    public:
        CAABB();
        ~CAABB() {}
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_AABB_H__)
