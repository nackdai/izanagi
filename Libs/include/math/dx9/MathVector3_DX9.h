#if !defined(__IZANAGI__MATH_VECTOR3_DX9_H__)
#define __IZANAGI__MATH_VECTOR3_DX9_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"
#include "math/MathUtil.h"

namespace izanagi
{
namespace math
{
    // ベクトルを加算する
    // dst = src1 + src2
    void SVector3::Add(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        D3DXVec3Add(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src1),
            reinterpret_cast<const D3DXVECTOR3*>(&src2));
    }

    // ベクトルを加算する(XY成分のみ)
    void SVector3::AddXY(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        D3DXVec2Add(
            reinterpret_cast<D3DXVECTOR2*>(&dst),
            reinterpret_cast<const D3DXVECTOR2*>(&src1),
            reinterpret_cast<const D3DXVECTOR2*>(&src2));
        dst.z = src1.z;
    }

    // ベクトルを減算する
    // dst = src1 - src2
    void SVector3::Sub(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        D3DXVec3Subtract(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src1),
            reinterpret_cast<const D3DXVECTOR3*>(&src2));
    }

    // ベクトルを減算する(XY成分のみ)
    void SVector3::SubXY(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        D3DXVec2Subtract(
            reinterpret_cast<D3DXVECTOR2*>(&dst),
            reinterpret_cast<const D3DXVECTOR2*>(&src1),
            reinterpret_cast<const D3DXVECTOR2*>(&src2));
        dst.z = src1.z;
    }

    // ベクトルをスケーリングする
    // dst = fQ * src
    void SVector3::Scale(SVector3& dst, const SVector3& src, IZ_FLOAT fQ)
    {
        D3DXVec3Scale(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src),
            fQ);
    }

    // ベクトルのＸＹ成分のみをスケーリングする
    // Z成分はそのまま
    void SVector3::ScaleXY(SVector3& dst, const SVector3& src, IZ_FLOAT fQ)
    {
        D3DXVec2Scale(
            reinterpret_cast<D3DXVECTOR2*>(&dst),
            reinterpret_cast<const D3DXVECTOR2*>(&src),
            fQ);
        dst.z = src.z;
    }

    // ベクトルをスケーリング（除算）する
    void SVector3::Div(SVector3& dst, const SVector3& src, IZ_FLOAT fQ)
    {
        D3DXVec3Scale(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src),
            1.0f / fQ);
    }

    // ベクトルのＸＹ成分のみをスケーリング(除算)する
    // Z成分はそのまま
    void SVector3::DivXY(SVector3& dst, const SVector3& src, IZ_FLOAT fQ)
    {
        D3DXVec2Scale(
            reinterpret_cast<D3DXVECTOR2*>(&dst),
            reinterpret_cast<const D3DXVECTOR2*>(&src),
            1.0f / fQ);
        dst.z = src.z;
    }

    // ベクトルの内積を計算する
    IZ_FLOAT SVector3::Dot(const SVector3& vec1, const SVector3& vec2)
    {
        IZ_FLOAT ret = D3DXVec3Dot(
                        reinterpret_cast<const D3DXVECTOR3*>(&vec1),
                        reinterpret_cast<const D3DXVECTOR3*>(&vec2));
        return ret;
    }

    // ベクトルの外積を計算する
    void SVector3::Cross(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        D3DXVec3Cross(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src1),
            reinterpret_cast<const D3DXVECTOR3*>(&src2));
    }

    // ベクトルの大きさ(絶対値)を計算する
    IZ_FLOAT SVector3::Length(const SVector3& vec)
    {
        IZ_FLOAT ret = D3DXVec2Length(reinterpret_cast<const D3DXVECTOR2*>(&vec));
        return ret;
    }

    // ベクトルを正規化する
    void SVector3::Normalize(SVector3& dst, const SVector3& src)
    {
        D3DXVec2Normalize(
            reinterpret_cast<D3DXVECTOR2*>(&dst),
            reinterpret_cast<const D3DXVECTOR2*>(&src));
        dst.z = 0.0f;
    }

    // ベクトルの線形補間
    void SVector3::Lerp(SVector3& dst, const SVector3& src0, const SVector3& src1, IZ_FLOAT f)
    {
        D3DXVec3Lerp(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src0),
            reinterpret_cast<const D3DXVECTOR3*>(&src1),
            f);
    }
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI__MATH_VECTOR3_DX9_H__)
