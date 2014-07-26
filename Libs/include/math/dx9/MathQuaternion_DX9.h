#if !defined(__IZANAGI__MATH_QUATERNION_DX9_H__)
#define __IZANAGI__MATH_QUATERNION_DX9_H__

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
        D3DXQuaternionIdentity(reinterpret_cast<D3DXQUATERNION*>(&quat));
    }

    // クオータニオンを乗算する
    // dst = src1 * src2
    void SQuat::Mul(SQuat& dst, const SQuat& src1, const SQuat& src2)
    {
        D3DXQuaternionMultiply(
            reinterpret_cast<D3DXQUATERNION*>(&dst),
            reinterpret_cast<const D3DXQUATERNION*>(&src1),
            reinterpret_cast<const D3DXQUATERNION*>(&src2));
    }

    // クオータニオンの大きさ(絶対値)を計算する
    IZ_FLOAT SQuat::Length(const SQuat& quat)
    {
        IZ_FLOAT ret = D3DXQuaternionLength(reinterpret_cast<const D3DXQUATERNION*>(&quat));
        return ret;
    }

    // クオータニオンを正規化する
    void SQuat::Normalize(SQuat& dst, const SQuat& src)
    {
        D3DXQuaternionNormalize(
            reinterpret_cast<D3DXQUATERNION*>(&dst),
            reinterpret_cast<const D3DXQUATERNION*>(&src));
    }

    // 共役クオータニオンを求める
    void SQuat::Conjugate(SQuat& dst, const SQuat& src)
    {
        D3DXQuaternionConjugate(
            reinterpret_cast<D3DXQUATERNION*>(&dst),
            reinterpret_cast<const D3DXQUATERNION*>(&src));
    }

    // 逆クオータニオンを求める
    void SQuat::Inverse(SQuat& dst, const SQuat& src)
    {
        D3DXQuaternionInverse(
            reinterpret_cast<D3DXQUATERNION*>(&dst),
            reinterpret_cast<const D3DXQUATERNION*>(&src));
    }

    // 球面線形補間
    void SQuat::Slerp(SQuat& dst, const SQuat& quat1, const SQuat& quat2, IZ_FLOAT t)
    {
        // 念のため
        t = CLAMP(t, 0.0f, 1.0f);

        D3DXQuaternionSlerp(
            reinterpret_cast<D3DXQUATERNION*>(&dst),
            reinterpret_cast<const D3DXQUATERNION*>(&quat1),
            reinterpret_cast<const D3DXQUATERNION*>(&quat2),
            t);
    }

    void SQuat::SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, const SVector4& vAxis)
    {
        D3DXQuaternionRotationAxis(
            reinterpret_cast<D3DXQUATERNION*>(&quat),
            reinterpret_cast<const D3DXVECTOR3*>(&vAxis),
            rad);
    }

    // クオータニオンから行列を計算する
    void SQuat::MatrixFromQuat(SMatrix44& mtx, const SQuat& quat)
    {
        D3DXMatrixRotationQuaternion(
            reinterpret_cast<D3DXMATRIX*>(&mtx),
            reinterpret_cast<const D3DXQUATERNION*>(&quat));
    }

    // 行列からクオータニオンを計算する
    void SQuat::QuatFromMatrix(SQuat& quat, const SMatrix44& mtx)
    {
        D3DXQuaternionRotationMatrix(
            reinterpret_cast<D3DXQUATERNION*>(&quat),
            reinterpret_cast<const D3DXMATRIX*>(&mtx));
    }

    // オイラー角からクオータニオンを計算する
    void SQuat::QuatFromEuler(SQuat& quat, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
    {
        D3DXQuaternionRotationYawPitchRoll(
            reinterpret_cast<D3DXQUATERNION*>(&quat),
            y,    // Yaw
            x,    // Pitch
            z);    // Roll
    }
}   // namespace math
}    // namespace izanagi

#endif    // #if !defined(__IZANAGI__MATH_QUATERNION_DX9_H__)
