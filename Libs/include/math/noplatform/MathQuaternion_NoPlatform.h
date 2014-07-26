#if !defined(__IZANAGI__MATH_QUATERNION_NO_PLATFORM_H__)
#define __IZANAGI__MATH_QUATERNION_NO_PLATFORM_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"
#include "math/MathVector4.h"

namespace izanagi
{
namespace math
{
    void SQuat::SetIdentity(SQuat& quat)
    {
        quat.x = 0.0f;
        quat.y = 0.0f;
        quat.z = 0.0f;
        quat.w = 1.0f;
    }

    // クオータニオンを乗算する
    // dst = src1 * src2
    void SQuat::Mul(SQuat& dst, const SQuat& src1, const SQuat& src2)
    {
        dst.x = src1.w * src2.x + src2.w * src1.x + src1.y * src2.z - src1.z * src2.y;
        dst.y = src1.w * src2.y + src2.w * src1.y + src1.z * src2.x - src1.x * src2.z;
        dst.z = src1.w * src2.z + src2.w * src1.z + src1.x * src2.y - src1.y * src2.x;

        dst.w = src1.w * src2.w - src1.x * src2.x - src1.y * src2.y - src1.z * src2.z;
    }

    // クオータニオンの大きさ(絶対値)を計算する
    IZ_FLOAT SQuat::Length(const SQuat& quat)
    {
        IZ_FLOAT q = quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
        IZ_FLOAT ret = sqrtf(q);
        return ret;
    }

    // クオータニオンを正規化する
    void SQuat::Normalize(SQuat& dst, const SQuat& src)
    {
        Div(dst, src, Length(src));
    }

    // 共役クオータニオンを求める
    void SQuat::Conjugate(SQuat& dst, const SQuat& src)
    {
        dst.x = -src.x;
        dst.y = -src.y;
        dst.z = -src.z;
        dst.w = src.w;
    }

    // 逆クオータニオンを求める
    void SQuat::Inverse(SQuat& dst, const SQuat& src)
    {
        // |q|^2
        IZ_FLOAT s = src.x * src.x + src.y * src.y + src.z * src.z + src.w * src.w;

        // conjugate(q)
        Conjugate(dst, src);

        // q^-1 = conjugate(q) / |q|^2
        Div(dst, dst, s);
    }

    // 球面線形補間
    void SQuat::Slerp(SQuat& dst, const SQuat& quat1, const SQuat& quat2, IZ_FLOAT t)
    {
        // 念のため
        t = CLAMP(t, 0.0f, 1.0f);

        // TODO
        IZ_ASSERT(IZ_FALSE);
    }

    void SQuat::SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, const SVector4& vAxis)
    {
        // 念のため
        SVector4 v;
        SVector4::Normalize(v, vAxis);

        IZ_FLOAT c = cosf(rad * 0.5f);
        IZ_FLOAT s = sinf(rad * 0.5f);

        quat.x = s * v.x;
        quat.y = s * v.y;
        quat.z = s * v.z;
        quat.w = c;
    }

    // クオータニオンから行列を計算する
    void SQuat::MatrixFromQuat(SMatrix44& mtx, const SQuat& quat)
    {
        const IZ_FLOAT& x = quat.x;
        const IZ_FLOAT& y = quat.y;
        const IZ_FLOAT& z = quat.z;
        const IZ_FLOAT& w = quat.w;

#if 1
        // ベタに計算するとこうなる
        const IZ_FLOAT xx2 = 2.0f * x * x;
        const IZ_FLOAT yy2 = 2.0f * y * y;
        const IZ_FLOAT zz2 = 2.0f * z * z;
        const IZ_FLOAT xy2 = 2.0f * x * y;
        const IZ_FLOAT xz2 = 2.0f * x * z;
        const IZ_FLOAT wz2 = 2.0f * w * z;
        const IZ_FLOAT wy2 = 2.0f * w * y;
        const IZ_FLOAT yz2 = 2.0f * y * z;
        const IZ_FLOAT wx2 = 2.0f * w * x;

        mtx.m[0][0] = 1.0f - yy2 - zz2;
        mtx.m[0][1] = xy2 + wz2;
        mtx.m[0][2] = xz2 - wy2;
        mtx.m[0][3] = 0.0f;

        mtx.m[1][0] = xy2 - wz2;
        mtx.m[1][1] = 1.0f - xx2 - zz2;
        mtx.m[1][2] = yz2 + wx2;
        mtx.m[1][3] = 0.0f;

        mtx.m[2][0] = xz2 + wy2;
        mtx.m[2][1] = yz2 - wx2;
        mtx.m[2][2] = 1.0f - xx2 - yy2;
        mtx.m[2][3] = 0.0f;
#else
        // マトリクスの乗算に直すとこうなる
        SMatrix44 m1, m2;
        
        m1.m[0][0] =  w; m1.m[0][1] =  x; m1.m[0][2] = -y; m1.m[0][3] =  z;
        m1.m[1][0] = -z; m1.m[1][1] =  y; m1.m[1][2] =  x; m1.m[1][3] =  w;
        m1.m[2][0] =  y; m1.m[2][1] =  z; m1.m[2][2] =  w; m1.m[2][3] = -x;
        m1.m[3][0] =     m1.m[3][1] =     m1.m[3][2] =     m1.m[3][3] = 0.0f;
        
        m2.m[0][0] =  w; m2.m[0][1] =  z; m2.m[0][2] = -y; m2.m[0][3] = 0.0f;
        m2.m[1][0] =  x; m2.m[1][1] =  y; m2.m[1][2] =  z; m2.m[1][3] = 0.0f;
        m2.m[2][0] =  y; m2.m[2][1] = -x; m2.m[2][2] =  w; m2.m[2][3] = 0.0f;
        m2.m[3][0] = -z; m2.m[3][1] =  w; m2.m[3][2] =  x; m2.m[3][3] = 0.0f;

        SMatrix44::Mul(mtx, m1, m2);
        mtx.m[3][3] = 1.0f;
#endif
    }

    // 行列からクオータニオンを計算する
    void SQuat::QuatFromMatrix(SQuat& quat, const SMatrix44& mtx)
    {
        // 最大値を探す
        IZ_FLOAT value[4] = {
             mtx._00 - mtx._11 - mtx._22 + 1.0f,
            -mtx._00 + mtx._11 - mtx._22 + 1.0f,
            -mtx._00 - mtx._11 + mtx._22 + 1.0f,
             mtx._00 + mtx._11 + mtx._22 + 1.0f,
        };

        IZ_UINT nMaxValIdx = 0;
        for (IZ_UINT i = 0; i < 4; ++i) {
            if (value[i] > value[nMaxValIdx]) {
                nMaxValIdx = i;
            }
        }

        IZ_ASSERT(value[nMaxValIdx] > 0.0f);

        IZ_FLOAT v = sqrtf(value[nMaxValIdx]) * 0.5f;

        // NOTE
        // 1 / (4 * v)
        IZ_FLOAT div = 0.25f / v;

        switch (nMaxValIdx) {
        case 0:    // x
            quat.x = v;
            quat.y = (mtx._01 + mtx._10) * div;
            quat.z = (mtx._20 + mtx._02) * div;
            quat.w = (mtx._12 - mtx._21) * div;
            break;
        case 1:    // y
            quat.x = (mtx._01 + mtx._10) * div;
            quat.y = v;
            quat.z = (mtx._12 + mtx._21) * div;
            quat.w = (mtx._20 - mtx._02) * div;
            break;
        case 2:    // z
            quat.x = (mtx._20 + mtx._02) * div;
            quat.y = (mtx._12 + mtx._21) * div;
            quat.z = v;
            quat.w = (mtx._01 - mtx._10) * div;
            break;
        case 3:    // w
            quat.x = (mtx._12 - mtx._21) * div;
            quat.y = (mtx._20 - mtx._02) * div;
            quat.z = (mtx._01 - mtx._10) * div;
            quat.w = v;
            break;
        }
    }

    // オイラー角からクオータニオンを計算する
    void SQuat::QuatFromEuler(SQuat& quat, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
    {
        // Q1 = (x1, y1, z1, w1) = x1i + y1j + z1k + w1
        // Q2 = (x2, y2, z2, w2) = x2i + y2j + z2k + w2
        //
        // ii = jj = kk = -1
        // ij = -ji = k
        // jk = -kj = i
        // ki = -ik = j
        //
        // Q1Q2 = (w1w2 - x1x2 - y1y2 - z1-z2)
        //        + (w1x2 + x1w2 + y1z2 - z1y2)i
        //        + (w1y2 - x1z2 + y1w2 + z1x2)j
        //        + (w1z2 + x1y2 - y1x2 + z1w2)k

        // Qyaw   = (0,            sin(yaw/2), 0,           cos(yaw/2))
        // Qpitch = (sin(pitch/2), 0,          0,           cos(pitch/2))
        // Qroll  = (0,            0,          sin(roll/2), cos(roll/2))
        //
        // Q = QyawQpitchQroll -> Roll Pitch Yaw の順番
        //  (qPq^-1 = QyawQpitchQrollPq^-1 からRoll Pitch Yaw の順番でかけることになる）
        //
        // Cy = cos(yaw/2), Cp = cos(pitch/2), Cr = cos(roll/2)
        // Sy = sin(yaw/2), Sp = sin(pitch/2), Sr = sin(roll/2)
        //
        // QpitchQroll = (CpCr) + (SpCr)i + (-SpSR)j + (CpSr)k
        // QyawQpirchQroll = (CyCpCr + SySpSr) 
        //                      + (CySpCr + SyCpCr)i
        //                      + (-CySpSr + SyCpCr)j
        //                      + (CyCpSr - SySpCr)k

        // Yaw
        IZ_FLOAT cosY = cosf(y * 0.5f);
        IZ_FLOAT sinY = sinf(y * 0.5f);

        // Pitch
        IZ_FLOAT cosX = cosf(x * 0.5f);
        IZ_FLOAT sinX = sinf(x * 0.5f);

        // Roll
        IZ_FLOAT cosZ = cosf(z * 0.5f);
        IZ_FLOAT sinZ = sinf(z * 0.5f);

        quat.Set(
            cosZ * sinX * cosY + sinZ * cosX * sinY,
            cosZ * cosX * sinY - sinZ * sinX * cosY,
            sinZ * cosX * cosY - cosZ * sinX * sinY,
            cosZ * cosX * cosY + sinZ * sinX * sinY);
    }
}   // namespace math
}   // namespace izanagi

#endif    // #if !defined(__IZANAGI__MATH_QUATERNION_NO_PLATFORM_H__)
