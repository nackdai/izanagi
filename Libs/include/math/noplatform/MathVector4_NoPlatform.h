#if !defined(__IZANAGI__MATH_VECTOR_NO_PLATFORM_H__)
#define __IZANAGI__MATH_VECTOR_NO_PLATFORM_H__

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
        dst.x = src1.x + src2.x;
        dst.y = src1.y + src2.y;
        dst.z = src1.z + src2.z;
        dst.w = src1.w + src2.w;
    }

    // ベクトルを加算する(XYZ成分のみ)
    void SVector4::AddXYZ(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        dst.x = src1.x + src2.x;
        dst.y = src1.y + src2.y;
        dst.z = src1.z + src2.z;
        dst.w = src1.w;
    }

    // ベクトルを減算する
    // dst = src1 - src2
    void SVector4::Sub(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        dst.x = src1.x - src2.x;
        dst.y = src1.y - src2.y;
        dst.z = src1.z - src2.z;
        dst.w = src1.w - src2.w;
    }

    // ベクトルを減算する(XYZ成分のみ)
    void SVector4::SubXYZ(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        dst.x = src1.x - src2.x;
        dst.y = src1.y - src2.y;
        dst.z = src1.z - src2.z;
        dst.w = src1.w;
    }

    // ベクトルをスケーリングする
    // dst = fQ * src
    void SVector4::Scale(SVector4& dst, const SVector4& src, IZ_FLOAT fQ)
    {
        dst.x = fQ * src.x;
        dst.y = fQ * src.y;
        dst.z = fQ * src.z;
        dst.w = fQ * src.w;
    }

    // ベクトルのＸＹＺ成分のみをスケーリングする
    // Ｗ成分はそのまま
    // dst = (src.x x fQ, src.y x fQ, src.z x fQ, src.w)
    void SVector4::ScaleXYZ(SVector4& dst, const SVector4& src, IZ_FLOAT fQ)
    {
        dst.x = fQ * src.x;
        dst.y = fQ * src.y;
        dst.z = fQ * src.z;
        dst.w = src.w;
    }

    // ベクトルをスケーリング（除算）する
    // dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
    void SVector4::Div(SVector4& dst, const SVector4& src, IZ_FLOAT fQ)
    {
        dst.x = src.x / fQ;
        dst.y = src.y / fQ;
        dst.z = src.z / fQ;
        dst.w = src.w / fQ;
    }

    // ベクトルのＸＹＺ成分のみをスケーリング(除算)する
    // Ｗ成分はそのまま
    // dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w)
    void SVector4::DivXYZ(SVector4& dst, const SVector4& src, IZ_FLOAT fQ)
    {
        dst.x = src.x / fQ;
        dst.y = src.y / fQ;
        dst.z = src.z / fQ;
        dst.w = src.w;
    }

    // ベクトルの内積を計算する
    IZ_FLOAT SVector4::Dot(const SVector4& vec1, const SVector4& vec2)
    {
        IZ_FLOAT ret = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
        return ret;
    }

    // ベクトルの外積を計算する
    void SVector4::Cross(SVector4& dst, const SVector4& src1, const SVector4& src2)
    {
        dst.x = src1.y * src2.z - src1.z * src2.y;
        dst.y = src1.z * src2.x - src1.x * src2.z;
        dst.z = src1.x * src2.y - src1.y * src2.x;
        dst.w = src1.w;
    }

    // ベクトルの大きさ(絶対値)を計算する
    IZ_FLOAT SVector4::Length(const SVector4& vec)
    {
        IZ_FLOAT ret = sqrtf(Dot(vec, vec));
        return ret;
    }

    // ベクトルを正規化する
    void SVector4::Normalize(SVector4& dst, const SVector4& src)
    {
        SVector4::DivXYZ(dst, src, Length(src));
        dst.w = 0.0f;
    }

    // ベクトルの線形補間
    void SVector4::Lerp(SVector4& dst, const SVector4& src0, const SVector4& src1, IZ_FLOAT f)
    {
        IZ_FLOAT ff = 1.0f - f;
        dst.x = ff * src0.x + f * src1.x;
        dst.y = ff * src0.y + f * src1.y;
        dst.z = ff * src0.z + f * src1.z;
        dst.w = ff * src0.w + f * src1.w;
    }
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI__MATH_VECTOR_NO_PLATFORM_H__)
