#include "XFileHelper.h"

#define	_CHECK_SCALE	(0)

namespace {
	inline IZ_BOOL _HasScale(IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
	{
		izanagi::SVector vec;
		vec.Set(x, y, z);

		IZ_FLOAT len = izanagi::LengthVector(vec);
#if 0
		IZ_BOOL bHasScale = !izanagi::CMath::IsNearyEqual(len, 1.0f);
#else
		//IZ_PRINTF("   len[%f]\n", len);
		static const IZ_FLOAT EPSILON = 0.000001f;
		IZ_BOOL bHasScale = !((1.0f - EPSILON < len) && (len < 1.0f + EPSILON));
#endif

		return bHasScale;
	}
}	// namespace


// Break down matrix to trans, quartanion.
IZ_BOOL CXFileMathUtil::BreakDownMatrix(
	const izanagi::SMatrix& mtx,
	izanagi::S_SKL_JOINT_POSE& sPose)
{
#if _CHECK_SCALE
	// If matrix has scale transform, it is fail.
	if (HasScale(mtx)) {
		return IZ_TRUE;
	}
#endif	// #if _CHECK_SCALE

	sPose.scale[0] = 1.0f;
	sPose.scale[1] = 1.0f;
	sPose.scale[2] = 1.0f;

	sPose.trans[0] = mtx.v[3].x;
	sPose.trans[1] = mtx.v[3].y;
	sPose.trans[2] = mtx.v[3].z;

	izanagi::QuatFromMatrix(
		sPose.quat,
		mtx);

	return IZ_FALSE;
}

IZ_BOOL CXFileMathUtil::HasScale(const izanagi::SMatrix& mtx)
{
	//IZ_PRINTF("scale:");
	IZ_BOOL b0 = _HasScale(mtx._00, mtx._10, mtx._20);
	IZ_BOOL b1 = _HasScale(mtx._01, mtx._11, mtx._21);
	IZ_BOOL b2 = _HasScale(mtx._02, mtx._12, mtx._22);
	return (b0 || b1 || b2);
}
