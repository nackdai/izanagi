#if !defined(__IZANAGI__MATH_MATRIX44_DX9_H__)
#define __IZANAGI__MATH_MATRIX44_DX9_H__

#include <math.h>
#include "izDefs.h"
#include "math/MathDefs.h"
#include "math/MathVector4.h"

namespace izanagi
{
namespace math
{
    // 単位マトリクスを設定する
    void SMatrix44::SetUnit(SMatrix44& mtx)
    {
        D3DXMatrixIdentity(reinterpret_cast<D3DXMATRIX*>(&mtx));
    }

    // マトリクスを転置する
    void SMatrix44::Transpose(SMatrix44& dst, const SMatrix44& src)
    {
        D3DXMatrixTranspose(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            reinterpret_cast<const D3DXMATRIX*>(&src));
    }

    // マトリクスを乗算する
    // dst = src1 x src2
    void SMatrix44::Mul(SMatrix44& dst, const SMatrix44& src1, const SMatrix44& src2)
    {
        D3DXMatrixMultiply(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            reinterpret_cast<const D3DXMATRIX*>(&src1),
            reinterpret_cast<const D3DXMATRIX*>(&src2));
    }

    // ベクトルとマトリクスを乗算する
    // dst = vec x mtx
    void SMatrix44::Apply(SVector4& dst, const SVector4& vec, const SMatrix44& mtx)
    {
        D3DXVec4Transform(
            reinterpret_cast<D3DXVECTOR4*>(&dst),
            reinterpret_cast<const D3DXVECTOR4*>(&vec),
            reinterpret_cast<const D3DXMATRIX*>(&mtx));
    }

    // ベクトルとマトリクスのＸＹＺ成分のみを乗算する
    // ベクトルのＷ成分、およびマトリクスの第４行は無視される
    void SMatrix44::ApplyXYZ(SVector4& dst, const SVector4& vec, const SMatrix44& mtx)
    {
        SVector4 v;
        SVector4::Copy(v, vec);
        v.w = 0.0f;

        D3DXVec4Transform(
            reinterpret_cast<D3DXVECTOR4*>(&dst),
            reinterpret_cast<const D3DXVECTOR4*>(&v),
            reinterpret_cast<const D3DXMATRIX*>(&mtx));

        v.w = vec.w;
    }

        // X軸を回転軸にして回転するマトリクスを取得
    void SMatrix44::GetRotByX(SMatrix44& dst, IZ_FLOAT fTheta)
    {
        D3DXMatrixRotationX(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            fTheta);
    }

    // Y軸を回転軸にして回転するマトリクスを取得
    void SMatrix44::GetRotByY(SMatrix44& dst, IZ_FLOAT fTheta)
    {
        D3DXMatrixRotationY(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            fTheta);
    }

    // Z軸を回転軸にして回転するマトリクスを取得
    void SMatrix44::GetRotByZ(SMatrix44& dst, IZ_FLOAT fTheta)
    {
        D3DXMatrixRotationZ(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            fTheta);
    }

    // 任意軸を回転軸にして回転するマトリクスを取得
    void SMatrix44::GetRot(
        SMatrix44& dst, IZ_FLOAT fTheta,
        IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
    {
        D3DXVECTOR3 v(x, y, z);

        D3DXMatrixRotationAxis(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            &v,
            fTheta);
    }

    void SMatrix44::GetTrans(SMatrix44& dst, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
    {
        D3DXMatrixTranslation(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            x, y, z);
    }

    // スケーリングマトリクスを得る
    void SMatrix44::SetScale(SMatrix44& dst, IZ_FLOAT fScaleX, IZ_FLOAT fScaleY, IZ_FLOAT fScaleZ)
    {
        D3DXMatrixScaling(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            fScaleX,
            fScaleY,
            fScaleZ);
    }

        
        // 行列式を計算する
    IZ_FLOAT SMatrix44::Determinant(const SMatrix44& mtx)
    {
        IZ_FLOAT ret = D3DXMatrixDeterminant(reinterpret_cast<const D3DXMATRIX*>(&mtx));
        return ret;
    }

    // 逆マトリクスを Gauss/Jordan 法で求める
    void SMatrix44::Inverse(SMatrix44& dst, const SMatrix44& src)
    {
        D3DXMatrixInverse(
            reinterpret_cast<D3DXMATRIX*>(&dst),
            IZ_NULL,    // 行列式はいらない
            reinterpret_cast<const D3DXMATRIX*>(&src));
    }
}   // namespace math
}    // namespace izanagi

#endif    // #if !defined(__IZANAGI__MATH_MATRIX44_DX9_H__)
