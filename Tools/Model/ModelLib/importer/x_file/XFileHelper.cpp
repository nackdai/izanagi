#include "XFileHelper.h"

#define _CHECK_SCALE    (0)

namespace {
    inline IZ_BOOL _HasScale(IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
    {
        izanagi::math::SVector4 vec;
        vec.Set(x, y, z);

        IZ_FLOAT len = izanagi::math::SVector4::Length(vec);
#if 0
        IZ_BOOL bHasScale = !izanagi::math::CMath::IsNearyEqual(len, 1.0f);
#else
        //IZ_PRINTF("   len[%f]\n", len);
        static const IZ_FLOAT EPSILON = 0.000001f;
        IZ_BOOL bHasScale = !((1.0f - EPSILON < len) && (len < 1.0f + EPSILON));
#endif

        return bHasScale;
    }
}   // namespace


// Break down matrix to trans, quartanion.
IZ_BOOL CXFileMathUtil::BreakDownMatrix(
    const izanagi::math::SMatrix44& mtx,
    izanagi::S_SKL_JOINT_POSE& sPose)
{
#if _CHECK_SCALE
    // If matrix has scale transform, it is fail.
    if (HasScale(mtx)) {
        return IZ_TRUE;
    }
#endif  // #if _CHECK_SCALE

#if 0
    sPose.scale[0] = 1.0f;
    sPose.scale[1] = 1.0f;
    sPose.scale[2] = 1.0f;
#else
    izanagi::math::SVector3 v0;
    v0.Set(mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);

    izanagi::math::SVector3 v1;
    v1.Set(mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);

    izanagi::math::SVector3 v2;
    v2.Set(mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);

    sPose.scale[0] = izanagi::math::SVector3::Length(v0);
    sPose.scale[1] = izanagi::math::SVector3::Length(v1);
    sPose.scale[2] = izanagi::math::SVector3::Length(v2);
#endif

    sPose.trans[0] = mtx.v[3].x;
    sPose.trans[1] = mtx.v[3].y;
    sPose.trans[2] = mtx.v[3].z;

#if 0
    izanagi::math::SQuat::QuatFromMatrix(
        sPose.quat,
        mtx);
#else
    izanagi::math::SMatrix44 tmp;
    izanagi::math::SMatrix44::Copy(tmp, mtx);

    tmp.m[0][0] /= sPose.scale[0];
    tmp.m[1][0] /= sPose.scale[0];
    tmp.m[2][0] /= sPose.scale[0];

    tmp.m[0][1] /= sPose.scale[1];
    tmp.m[1][1] /= sPose.scale[1];
    tmp.m[2][1] /= sPose.scale[1];

    tmp.m[0][2] /= sPose.scale[2];
    tmp.m[1][2] /= sPose.scale[2];
    tmp.m[2][2] /= sPose.scale[2];

    izanagi::math::SQuat::QuatFromMatrix(
        sPose.quat,
        tmp);
#endif

    return IZ_FALSE;
}

IZ_BOOL CXFileMathUtil::HasScale(const izanagi::math::SMatrix44& mtx)
{
    //IZ_PRINTF("scale:");
    IZ_BOOL b0 = _HasScale(mtx._00, mtx._10, mtx._20);
    IZ_BOOL b1 = _HasScale(mtx._01, mtx._11, mtx._21);
    IZ_BOOL b2 = _HasScale(mtx._02, mtx._12, mtx._22);
    return (b0 || b1 || b2);
}
