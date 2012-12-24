#if !defined(__CUSTOM_INTERP_H__)
#define __CUSTOM_INTERP_H__

#include "izSceneGraph.h"

class CCustomInterp
{
public:
    static IZ_FLOAT InterpScalar(
        IZ_FLOAT time,
        IZ_UINT keyNum,
        IZ_UINT paramPos,
        const izanagi::S_ANM_KEY** keys);

    static void InterpVector(
        izanagi::math::SVector& ret,
        IZ_FLOAT time,
        IZ_UINT keyNum,
        IZ_UINT paramPos,
        const izanagi::S_ANM_KEY** keys);
};

#endif    // #if !defined(__CUSTOM_INTERP_H__)
