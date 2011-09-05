#include "scenegraph/skl/SkeletonUtil.h"
#include "scenegraph/anm/ANMFormat.h"

using namespace izanagi;

namespace {
	inline void _UpdatePose(
		IZ_UINT paramType,
		IZ_FLOAT* dst,
		const SVector& src)
	{
		IZ_UINT paramNum = 0;
		IZ_UINT paramPos = 0;

		switch (paramType) {
		case E_ANM_TRANSFORM_TYPE_X:	// Xのみ
			paramNum = 1;
			paramPos = 0;
			break;
		case E_ANM_TRANSFORM_TYPE_Y:	// Yのみ
			paramNum = 1;
			paramPos = 1;
			break;
		case E_ANM_TRANSFORM_TYPE_Z:	// Zのみ
			paramNum = 1;
			paramPos = 2;
			break;
		case E_ANM_TRANSFORM_TYPE_W:	// Wのみ
			paramNum = 1;
			paramPos = 3;
			break;
		case E_ANM_TRANSFORM_TYPE_XYZ:	// XWZのみ
			SVector::CopyXYZ(
				*(reinterpret_cast<SVector*>(dst)),
				src);
			break;
		case E_ANM_TRANSFORM_TYPE_XYZW:	// XYZWすべて
			SVector::Copy(
				*(reinterpret_cast<SVector*>(dst)),
				src);
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}

		if (paramNum == 1) {
			dst[paramPos] = src.v[paramPos];
		}
	}
}	// namespace

void CSkeletonUtil::UpdatePose(
	S_SKL_JOINT_POSE& pose,
	IZ_UINT transformType,
	IZ_UINT paramType,
	const SVector& param)
{
	switch (transformType) {
	case E_ANM_TRANSFORM_TYPE_TRANSLATE:
		_UpdatePose(paramType, pose.trans, param);
		break;
	case E_ANM_TRANSFORM_TYPE_QUATERNION:
		_UpdatePose(paramType, pose.quat.v, param);
		break;
	case E_ANM_TRANSFORM_TYPE_SCALE:
		_UpdatePose(paramType, pose.scale, param);
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}
}
