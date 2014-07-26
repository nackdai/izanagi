#if !defined(__IZANAGI__MATH_VECTOR3_NO_PLATFORM_H__)
#define __IZANAGI__MATH_VECTOR3_NO_PLATFORM_H__

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
        dst.x = src1.x + src2.x;
        dst.y = src1.y + src2.y;
        dst.z = src1.z + src2.z;
    }

    // ベクトルを加算する(XY成分のみ)
    void SVector3::AddXY(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        dst.x = src1.x + src2.x;
        dst.y = src1.y + src2.y;
        dst.z = src1.z;
    }

    // ベクトルを減算する
    // dst = src1 - src2
    void SVector3::Sub(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        dst.x = src1.x - src2.x;
        dst.y = src1.y - src2.y;
        dst.z = src1.z - src2.z;
    }

    // ベクトルを減算する(XY成分のみ)
    void SVector3::SubXY(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        dst.x = src1.x - src2.x;
        dst.y = src1.y - src2.y;
        dst.z = src1.z;
    }

    // ベクトルをスケーリングする
    // dst = fQ * src
    void SVector3::Scale(SVector3& dst, const SVector3& src, IZ_FLOAT fQ)
    {
        dst.x = fQ * src.x;
        dst.y = fQ * src.y;
        dst.z = fQ * src.z;
    }

    // ベクトルのＸＹ成分のみをスケーリングする
    // Z成分はそのまま
    void SVector3::ScaleXY(SVector3& dst, const SVector3& src, IZ_FLOAT fQ)
    {
        dst.x = fQ * src.x;
        dst.y = fQ * src.y;
        dst.z = src.z;
    }

    // ベクトルをスケーリング（除算）する
    void SVector3::Div(SVector3& dst, const SVector3& src, IZ_FLOAT fQ)
    {
        dst.x = src.x / fQ;
        dst.y = src.y / fQ;
        dst.z = src.z / fQ;
    }

    // ベクトルのＸＹ成分のみをスケーリング(除算)する
    // Z成分はそのまま
    void SVector3::DivXY(SVector3& dst, const SVector3& src, IZ_FLOAT fQ)
    {
        dst.x = src.x / fQ;
        dst.y = src.y / fQ;
        dst.z = src.z;
    }

    // ベクトルの内積を計算する
    IZ_FLOAT SVector3::Dot(const SVector3& vec1, const SVector3& vec2)
    {
        IZ_FLOAT ret = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
        return ret;
    }

    // ベクトルの外積を計算する
    void SVector3::Cross(SVector3& dst, const SVector3& src1, const SVector3& src2)
    {
        dst.x = src1.y * src2.z - src1.z * src2.y;
        dst.y = src1.z * src2.x - src1.x * src2.z;
        dst.z = src1.x * src2.y - src1.y * src2.x;
    }

    // ベクトルの大きさ(絶対値)を計算する
    IZ_FLOAT SVector3::Length(const SVector3& vec)
    {
        IZ_FLOAT ret = sqrtf(Dot(vec, vec));
        return ret;
    }

    // ベクトルを正規化する
    void SVector3::Normalize(SVector3& dst, const SVector3& src)
    {
        SVector3::Div(dst, src, Length(src));
    }

    // ベクトルの線形補間
    void SVector3::Lerp(SVector3& dst, const SVector3& src0, const SVector3& src1, IZ_FLOAT f)
    {
        IZ_FLOAT ff = 1.0f - f;
        dst.x = ff * src0.x + f * src1.x;
        dst.y = ff * src0.y + f * src1.y;
        dst.z = ff * src0.z + f * src1.z;
    }
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI__MATH_VECTOR3_NO_PLATFORM_H__)
