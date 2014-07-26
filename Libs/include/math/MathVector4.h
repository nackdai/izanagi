#if !defined(__IZANAGI__MATH_VECTOR4_H__)
#define __IZANAGI__MATH_VECTOR4_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"
#include "MathUtil.h"

namespace izanagi
{
namespace math
{
    struct SVector4 {
        union {
            IZ_FLOAT v[4];
            struct {
                IZ_FLOAT x, y, z, w;
            };
            struct {
                IZ_FLOAT r, g, b, a;
            };
        };

        void Set(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w = 1.0f)
        {
            x = _x; y = _y; z = _z; w = _w;
        }

        void Set(const SVector4& rhs)
        {
            x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
        }

        // ベクトルを設定する
        static void Set(SVector4& vec, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT w)
        {
            vec.x = x;
            vec.y = y;
            vec.z = z;
            vec.w = w;
        }

        static void SetXYZ(SVector4& vec, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            vec.x = x;
            vec.y = y;
            vec.z = z;
        }

        // 全成分０のベクトルを設定する
        static void SetZero(SVector4& vec)
        {
            Set(vec, 0.0f, 0.0f, 0.0f, 0.0f);
        }

        static void SetDefaultZero(SVector4& vec)
        {
            Set(vec, 0.0f, 0.0f, 0.0f, 1.0f);
        }

        // ベクトルをコピー
        static void Copy(SVector4& dst, const SVector4& src)
        {
            dst.x = src.x;
            dst.y = src.y;
            dst.z = src.z;
            dst.w = src.w;
        }

        // ベクトルをコピー(XYZ成分のみ)
        static void CopyXYZ(SVector4& dst, const SVector4& src)
        {
            dst.x = src.x;
            dst.y = src.y;
            dst.z = src.z;
        }

        // ２つのベクトルをスワップする。
        static void Swap(SVector4& v1, SVector4& v2)
        {
            SVector4 v;
            Copy(v, v1);
            Copy(v1, v2);
            Copy(v2, v);
        }

        // ベクトルを加算する
        // dst = src1 + src2
        static inline void Add(SVector4& dst, const SVector4& src1, const SVector4& src2);

        // ベクトルを加算する(XYZ成分のみ)
        static inline void AddXYZ(SVector4& dst, const SVector4& src1, const SVector4& src2);

        // ベクトルを減算する
        // dst = src1 - src2
        static inline void Sub(SVector4& dst, const SVector4& src1, const SVector4& src2);

        // ベクトルを減算する(XYZ成分のみ)
        static inline void SubXYZ(SVector4& dst, const SVector4& src1, const SVector4& src2);

        // ベクトルを乗算する
        // dst = src1 * src2
        static inline void Mul(SVector4& dst, const SVector4& src1, const SVector4& src2)
        {
            dst.x = src1.x * src2.x;
            dst.y = src1.y * src2.y;
            dst.z = src1.z * src2.z;
            dst.w = src1.w * src2.w;
        }

        // ベクトルをスケーリングする
        // dst = fQ * src
        static inline void Scale(SVector4& dst, const SVector4& src, IZ_FLOAT fQ);

        // ベクトルのＸＹＺ成分のみをスケーリングする
        // Ｗ成分はそのまま
        // dst = (src.x x fQ, src.y x fQ, src.z x fQ, src.w)
        static inline void ScaleXYZ(SVector4& dst, const SVector4& src, IZ_FLOAT fQ);

        // ベクトルをスケーリング（除算）する
        // dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
        static inline void Div(SVector4& dst, const SVector4& src, IZ_FLOAT fQ);

        // ベクトルのＸＹＺ成分のみをスケーリング(除算)する
        // Ｗ成分はそのまま
        // dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w)
        static inline void DivXYZ(SVector4& dst, const SVector4& src, IZ_FLOAT fQ);

        // ベクトルの内積を計算する
        static inline IZ_FLOAT Dot(const SVector4& vec1, const SVector4& vec2);

        // ベクトルの外積を計算する
        static inline void Cross(SVector4& dst, const SVector4& src1, const SVector4& src2);

        // ベクトルの大きさ(絶対値)を計算する
        static inline IZ_FLOAT Length(const SVector4& vec);

        // 位置ベクトル間の距離を計算する
        static IZ_FLOAT Length2(const SVector4& vec1, const SVector4& vec2)
        {
            SVector4 v;
            Sub(v, vec1, vec2);
            return Length(v);
        }

        // ベクトルを正規化する
        static inline void Normalize(SVector4& dst, const SVector4& src);

        // Ｘ軸に対してベクトルを回転する
        static void RotX(SVector4& dst, const SVector4& src, IZ_FLOAT fTheta)
        {
            IZ_FLOAT c, s;
            CMath::GetSinCosF(fTheta, s, c);

            Set(
                dst,
                src.v[0],
                src.v[1] * c - src.v[2] * s,
                src.v[1] * s + src.v[2] * c,
                src.v[3]);
        }

        // Ｙ軸に対してベクトルを回転する
        static void RotY(SVector4& dst, const SVector4& src, IZ_FLOAT fTheta)
        {
            IZ_FLOAT c, s;
            CMath::GetSinCosF(fTheta, s, c);

            Set(
                dst,
                src.v[0] * c + src.v[2] * s,
                src.v[1],
                src.v[2] * c - src.v[0] * s,
                src.v[3]);
        }

        // Ｚ軸に対してベクトルを回転する
        static void RotZ(SVector4& dst, const SVector4& src, IZ_FLOAT fTheta)
        {
            IZ_FLOAT c, s;
            CMath::GetSinCosF(fTheta, s, c);

            Set(
                dst,
                src.v[0] * c - src.v[1] * s,
                src.v[0] * s + src.v[1] * c,
                src.v[2],
                src.v[3]);  
        }

        // ベクトルの線形補間
        static inline void Lerp(SVector4& dst, const SVector4& src0, const SVector4& src1, IZ_FLOAT f);
    };
}   // namespace math
}   // namespace izanagi

#ifdef __USE_D3D_MATH__
    #include "math/dx9/MathVector4_DX9.h"
#else   // #ifdef __USE_D3D_MATH__
    #include "math/noplatform/MathVector4_NoPlatform.h"
#endif

#endif  // #if !defined(__IZANAGI__MATH_VECTOR4_H__)
