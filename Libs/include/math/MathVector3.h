#if !defined(__IZANAGI__MATH_VECTOR3_H__)
#define __IZANAGI__MATH_VECTOR3_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"
#include "MathUtil.h"

namespace izanagi
{
namespace math
{
    struct SVector3 {
        union {
            IZ_FLOAT v[3];
            struct {
                IZ_FLOAT x, y, z;
            };
        };

        void Set(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z = 1.0f)
        {
            x = _x; y = _y; z = _z;;
        }

        void Set(const SVector3& rhs)
        {
            x = rhs.x; y = rhs.y; z = rhs.z;
        }

        // ベクトルを設定する
        static void Set(SVector3& vec, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            vec.x = x;
            vec.y = y;
            vec.z = z;
        }

        static void SetXYZ(SVector3& vec, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            vec.x = x;
            vec.y = y;
            vec.z = z;
        }

        // 全成分０のベクトルを設定する
        static void SetZero(SVector3& vec)
        {
            Set(vec, 0.0f, 0.0f, 0.0f);
        }

        static void SetDefaultZero(SVector3& vec)
        {
            Set(vec, 0.0f, 0.0f, 1.0f);
        }

        // ベクトルをコピー
        static void Copy(SVector3& dst, const SVector3& src)
        {
            dst.x = src.x;
            dst.y = src.y;
            dst.z = src.z;
        }

        // ベクトルをコピー(XY成分のみ)
        static void CopyXY(SVector3& dst, const SVector3& src)
        {
            dst.x = src.x;
            dst.y = src.y;
        }

        // ２つのベクトルをスワップする。
        static void Swap(SVector3& v1, SVector3& v2)
        {
            SVector3 v;
            Copy(v, v1);
            Copy(v1, v2);
            Copy(v2, v);
        }

        // ベクトルを加算する
        // dst = src1 + src2
        static inline void Add(SVector3& dst, const SVector3& src1, const SVector3& src2);

        // ベクトルを加算する(XY成分のみ)
        static inline void AddXY(SVector3& dst, const SVector3& src1, const SVector3& src2);

        // ベクトルを減算する
        // dst = src1 - src2
        static inline void Sub(SVector3& dst, const SVector3& src1, const SVector3& src2);

        // ベクトルを減算する(XY成分のみ)
        static inline void SubXY(SVector3& dst, const SVector3& src1, const SVector3& src2);

        // ベクトルを乗算する
        // dst = src1 * src2
        static inline void Mul(SVector3& dst, const SVector3& src1, const SVector3& src2)
        {
            dst.x = src1.x * src2.x;
            dst.y = src1.y * src2.y;
            dst.z = src1.z * src2.z;
        }

        // ベクトルをスケーリングする
        // dst = fQ * src
        static inline void Scale(SVector3& dst, const SVector3& src, IZ_FLOAT fQ);

        // ベクトルのＸＹ成分のみをスケーリングする
        // Z成分はそのまま
        // dst = (src.x x fQ, src.y x fQ, src.z)
        static inline void ScaleXY(SVector3& dst, const SVector3& src, IZ_FLOAT fQ);

        // ベクトルをスケーリング（除算）する
        // dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
        static inline void Div(SVector3& dst, const SVector3& src, IZ_FLOAT fQ);

        // ベクトルのＸＹ成分のみをスケーリング(除算)する
        // Z成分はそのまま
        // dst = (src.x / fQ, src.y / fQ, src.z)
        static inline void DivXY(SVector3& dst, const SVector3& src, IZ_FLOAT fQ);

        // ベクトルの内積を計算する
        static inline IZ_FLOAT Dot(const SVector3& vec1, const SVector3& vec2);

        // ベクトルの外積を計算する
        static inline void Cross(SVector3& dst, const SVector3& src1, const SVector3& src2);

        // ベクトルの大きさ(絶対値)を計算する
        static inline IZ_FLOAT Length(const SVector3& vec);

        // 位置ベクトル間の距離を計算する
        static IZ_FLOAT Length2(const SVector3& vec1, const SVector3& vec2)
        {
            SVector3 v;
            Sub(v, vec1, vec2);
            return Length(v);
        }

        // ベクトルを正規化する
        static inline void Normalize(SVector3& dst, const SVector3& src);

        // Ｘ軸に対してベクトルを回転する
        static void RotX(SVector3& dst, const SVector3& src, IZ_FLOAT fTheta)
        {
            IZ_FLOAT c, s;
            CMath::GetSinCosF(fTheta, s, c);

            Set(
                dst,
                src.v[0],
                src.v[1] * c - src.v[2] * s,
                src.v[1] * s + src.v[2] * c);
        }

        // Ｙ軸に対してベクトルを回転する
        static void RotY(SVector3& dst, const SVector3& src, IZ_FLOAT fTheta)
        {
            IZ_FLOAT c, s;
            CMath::GetSinCosF(fTheta, s, c);

            Set(
                dst,
                src.v[0] * c + src.v[2] * s,
                src.v[1],
                src.v[2] * c - src.v[0] * s);
        }

        // Ｚ軸に対してベクトルを回転する
        static void RotZ(SVector3& dst, const SVector3& src, IZ_FLOAT fTheta)
        {
            IZ_FLOAT c, s;
            CMath::GetSinCosF(fTheta, s, c);

            Set(
                dst,
                src.v[0] * c - src.v[1] * s,
                src.v[0] * s + src.v[1] * c,
                src.v[2]);  
        }

        // ベクトルの線形補間
        static inline void Lerp(SVector3& dst, const SVector3& src0, const SVector3& src1, IZ_FLOAT f);
    };
}   // namespace math
}   // namespace izanagi

#ifdef __USE_D3D_MATH__
    #include "math/dx9/MathVector3_DX9.h"
#else   // #ifdef __USE_D3D_MATH__
    #include "math/noplatform/MathVector3_NoPlatform.h"
#endif

#endif  // #if !defined(__IZANAGI__MATH_VECTOR3_H__)
