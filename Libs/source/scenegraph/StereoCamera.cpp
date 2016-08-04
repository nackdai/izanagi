#include "scenegraph/StereoCamera.h"

namespace izanagi {
    void StereoCamera::setFov(
        Eye type,
        IZ_FLOAT degreeInwardsHFov,
        IZ_FLOAT degreeOutwardsHFov,
        IZ_FLOAT degfeeVFov,
        FovPort& fov)
    {
        IZ_FLOAT in = IZ_DEG2RAD(degreeInwardsHFov);
        IZ_FLOAT out = IZ_DEG2RAD(degreeOutwardsHFov);
        IZ_FLOAT v = IZ_DEG2RAD(degfeeVFov);

        fov.tanTop = tanf(v * 0.5f);
        fov.tanBottom = -fov.tanTop;

        //  -------+-------
        //  l  | r   l | r
        //     |       |
        // out *  in   * out
        //     L       R

        if (type == Eye::Left) {
            // Left.
            fov.tanRight = tanf(in);    // inwards.
            fov.tanLeft = -tanf(out);   // outwards.
        }
        else {
            // Right.
            fov.tanRight = tanf(out);   // outwards.
            fov.tanLeft = -tanf(in);    // inwards.
        }
    }

    void StereoCamera::getCamera(
        Eye type,
        const HmdInfo& info,
        const FovPort& fov,
        CVectorCamera& dstCamera,
        const CVectorCamera& srcCamera)
    {
        math::CVector4 dir(srcCamera.m_X);

        IZ_FLOAT ipd = info.InterpupillaryDistance;

        // 投影スクリーンサイズに対する比率.
        //IZ_FLOAT ratio = info.HResolution / info.HScreenSize;
        IZ_FLOAT ratio = 1.0f;

        // 物理的な瞳孔間距離から3D空間内の距離に変換する.
        auto virtualIpd = ratio * ipd;

        auto t = virtualIpd * 0.5f;

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

        // W2Vのみ計算.
        dstCamera.m_IsDirtyW2V = IZ_TRUE;
        dstCamera.m_IsDirtyV2C = IZ_FALSE;
        dstCamera.m_NeedUpdateByTransform = IZ_TRUE;
        dstCamera.Update();

        // NOTE
        // projection matrix.
        // l : left at near plane.
        // r : right at near plane.
        // t : top at near plane.
        // b : bottom at near plane.
        // n : z near.
        // f : z far.
        //
        // D3DXMatrixPerspectiveOffCenterLH
        // https://msdn.microsoft.com/ja-jp/library/cc372889.aspx
        //    2n/(r-l)        0          0      0
        //       0         2n/(t-b)      0      0
        //  (l+r)/(l-r)  (t+b)/(b-t)  f/(f-n)   1
        //       0            0       nf/(n-f)  0
        //
        // D3DXMatrixPerspectiveOffCenterRH
        // https://msdn.microsoft.com/ja-jp/library/cc372890.aspx
        //    2n/(r-l)        0          0      0
        //       0         2n/(t-b)      0      0
        //  (l+r)/(r-l)  (t+b)/(t-b)  f/(n-f)   -1
        //       0            0       nf/(n-f)  0

        // left_tangent   = l/n
        // right_tangent  = r/n
        // top_tangent    = t/n
        // bottom_tangent = b/n
        //
        // right_tangent - left_tangent = r/n - l/n = (r-l)/n
        //  => (r-l)/2n = (right_tangent - left_tangent)/2
        //  => 2n/(r-l) = 2/(right_tangent - left_tangent)
        // 
        // 2n/(t-b) = 2/(top_tangent - bottom_tangent)
        //
        // left_tangent + right_tangent = l/n + r/n = (l+r)/n
        //
        // (l+r)/(l-r)
        //   = (l+r)/n * 1/2 * -2n/(r-l)
        //   = - (left_tangent + right_tangent) * 1/2 * 2/(right_tangent - left_tangent)
        //   = - (left_tangent + right_tangent) / (right_tangent - left_tangent)
        //
        // top_tangent + bottom_tangent = t/n + b/n = (t+b)/n
        // 
        // (t+b)/(b-t)
        //   = (t+b)/n * 1/2 * -2n(t-b)
        //   = - (top_tangent + bottom_tangent) * 1/2 * 2/(top_tangent - bottom_tangent)
        //   = - (top_tangent + bottom_tangent) / (top_tangent - bottom_tangent)

        // V2Cを計算.
        auto& mtxV2C = const_cast<math::SMatrix44&>(dstCamera.GetParam().mtxV2C);
        {
            IZ_FLOAT f = srcCamera.GetParam().cameraFar;
            IZ_FLOAT n = srcCamera.GetParam().cameraNear;

            math::SMatrix44::SetUnit(mtxV2C);

            mtxV2C.m[0][0] = 2.0f / (fov.tanRight - fov.tanLeft);   // 2n/(r-l)
            mtxV2C.m[1][1] = 2.0f / (fov.tanTop - fov.tanBottom);   // 2n/(t-b)

            mtxV2C.m[2][0] = (fov.tanLeft + fov.tanRight) / (fov.tanLeft - fov.tanRight);
            mtxV2C.m[2][1] = (fov.tanTop + fov.tanBottom) / (fov.tanBottom - fov.tanTop);

            mtxV2C.m[2][2] = f / (f - n);
            mtxV2C.m[2][3] = 1.0f;

            mtxV2C.m[3][2] = n * f / (n - f);

            mtxV2C.m[3][3] = 0.0f;
        }

        auto& mtxW2C = const_cast<math::SMatrix44&>(dstCamera.GetParam().mtxW2C);
        math::SMatrix44::Mul(
            mtxW2C,
            dstCamera.GetParam().mtxW2V,
            mtxV2C);
    }
}