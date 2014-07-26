#if !defined(__IZANAGI__MATH_VECTOR4_DX9_H__)
#define __IZANAGI__MATH_VECTOR4_DX9_H__

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
    void SVector4::Add(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        D3DXVec4Add(
            reinterpret_cast<D3DXVECTOR4*>(&dst),
            reinterpret_cast<const D3DXVECTOR4*>(&src1),
            reinterpret_cast<const D3DXVECTOR4*>(&src2));
    }

    // ベクトルを加算する(XYZ成分のみ)
    void SVector4::AddXYZ(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        D3DXVec3Add(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src1),
            reinterpret_cast<const D3DXVECTOR3*>(&src2));
        dst.w = src1.w;
    }

    // ベクトルを減算する
    // dst = src1 - src2
    void SVector4::Sub(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        D3DXVec4Subtract(
            reinterpret_cast<D3DXVECTOR4*>(&dst),
            reinterpret_cast<const D3DXVECTOR4*>(&src1),
            reinterpret_cast<const D3DXVECTOR4*>(&src2));
    }

    // ベクトルを減算する(XYZ成分のみ)
    void SVector4::SubXYZ(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        D3DXVec3Subtract(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src1),
            reinterpret_cast<const D3DXVECTOR3*>(&src2));
        dst.w = src1.w;
    }

    // ベクトルをスケーリングする
    // dst = fQ * src
    void SVector4::Scale(SVector4& dst, const SVector4& src, IZ_FLOAT fQ)
    {
        D3DXVec4Scale(
            reinterpret_cast<D3DXVECTOR4*>(&dst),
            reinterpret_cast<const D3DXVECTOR4*>(&src),
            fQ);
    }

    // ベクトルのＸＹＺ成分のみをスケーリングする
    // Ｗ成分はそのまま
    // dst = (src.x x fQ, src.y x fQ, src.z x fQ, src.w)
    void SVector4::ScaleXYZ(SVector4& dst, const SVector4& src, IZ_FLOAT fQ)
    {
        D3DXVec3Scale(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src),
            fQ);
        dst.w = src.w;
    }

    // ベクトルをスケーリング（除算）する
    // dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
    void SVector4::Div(SVector4& dst, const SVector4& src, IZ_FLOAT fQ)
    {
        D3DXVec4Scale(
            reinterpret_cast<D3DXVECTOR4*>(&dst),
            reinterpret_cast<const D3DXVECTOR4*>(&src),
            1.0f / fQ);
    }

    // ベクトルのＸＹＺ成分のみをスケーリング(除算)する
    // Ｗ成分はそのまま
    // dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w)
    void SVector4::DivXYZ(SVector4& dst, const SVector4& src, IZ_FLOAT fQ)
    {
        D3DXVec3Scale(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src),
            1.0f / fQ);
        dst.w = src.w;
    }

    // ベクトルの内積を計算する
    IZ_FLOAT SVector4::Dot(const SVector4& vec1, const SVector4& vec2)
    {
        IZ_FLOAT ret = D3DXVec4Dot(
                        reinterpret_cast<const D3DXVECTOR4*>(&vec1),
                        reinterpret_cast<const D3DXVECTOR4*>(&vec2));
        return ret;
    }

    // ベクトルの外積を計算する
    void SVector4::Cross(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        D3DXVec3Cross(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src1),
            reinterpret_cast<const D3DXVECTOR3*>(&src2));
    }


    // ベクトルの大きさ(絶対値)を計算する
    IZ_FLOAT SVector4::Length(const SVector4& vec)
    {
        IZ_FLOAT ret = D3DXVec3Length(reinterpret_cast<const D3DXVECTOR3*>(&vec));
        return ret;
    }

    // ベクトルを正規化する
    void SVector4::Normalize(SVector4& dst, const SVector4& src)
    {
        D3DXVec3Normalize(
            reinterpret_cast<D3DXVECTOR3*>(&dst),
            reinterpret_cast<const D3DXVECTOR3*>(&src));
        dst.w = 0.0f;
    }

    // ベクトルの線形補間
    void SVector4::Lerp(SVector4& dst, const SVector4& src0, const SVector4& src1, IZ_FLOAT f)
    {
        D3DXVec4Lerp(
            reinterpret_cast<D3DXVECTOR4*>(&dst),
            reinterpret_cast<const D3DXVECTOR4*>(&src0),
            reinterpret_cast<const D3DXVECTOR4*>(&src1),
            f);
    }
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI__MATH_VECTOR4_DX9_H__)
