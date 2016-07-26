#include "scenegraph/StereoCamera.h"

namespace izanagi {
    void StereoCamera::getCamera(
        Eye type,
        IZ_FLOAT ipd,   // 瞳孔間距離
        CVectorCamera& dstCamera,
        const CVectorCamera& srcCamera)
    {
        math::CVector4 dir(srcCamera.m_X);

        auto t = ipd * 0.5f;

        // NOTE
        // Left Hand Coord.
        switch (type) {
        case Eye::Left:
            dir *= -t;
            break;
        case Eye::Right:
            dir *= t;
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        math::CVector4 pos(srcCamera.m_Pos);
        pos += dir;

        math::SVector4::Copy(dstCamera.m_X, srcCamera.m_X);
        math::SVector4::Copy(dstCamera.m_Y, srcCamera.m_Y);
        math::SVector4::Copy(dstCamera.m_Z, srcCamera.m_Z);
        math::SVector4::Copy(dstCamera.m_Pos, pos);

        dstCamera.m_IsDirtyW2V = IZ_TRUE;
        dstCamera.m_IsDirtyV2C = IZ_TRUE;
        dstCamera.m_NeedUpdateByTransform = IZ_TRUE;

        dstCamera.m_Param.verticalFOV = srcCamera.GetParam().verticalFOV;
        dstCamera.m_Param.aspect = srcCamera.GetParam().aspect;
        dstCamera.m_Param.cameraNear = srcCamera.GetParam().cameraNear;
        dstCamera.m_Param.cameraFar = srcCamera.GetParam().cameraFar;

        dstCamera.Update();
    }
}