#if !defined(__IZANAGI__MATH_MATRIX33_H__)
#define __IZANAGI__MATH_MATRIX33_H__

#include <math.h>
#include "izDefs.h"
#include "MathDefs.h"
#include "MathVector3.h"
#include "MathMatrix44.h"

namespace izanagi
{
namespace math
{
    /**
     */
    struct SMatrix33 {
        union {
            IZ_FLOAT a[9];
            IZ_FLOAT m[3][3];
            SVector3 v[3];
            struct {
                IZ_FLOAT _00, _01, _02, _03;
                IZ_FLOAT _10, _11, _12, _13;
                IZ_FLOAT _20, _21, _22, _23;
            };
        };

        // マトリクスをコピーする
        static void Copy(SMatrix33& dst, const SMatrix33& src)
        {
            memcpy(&dst, &src, sizeof(SMatrix33));
        }

        // 単位マトリクスを参照する
        static const SMatrix33& GetUnit()
        {
            static SMatrix33 mtxUnit = {
                1.0f, 0.0f, 0.0f, 
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
            };

            return mtxUnit;
        }

        // 単位マトリクスを設定する
        static inline void SetUnit(SMatrix33& mtx);

        static IZ_BOOL IsUnit(const SMatrix33& mtx)
        {
            const SMatrix33& mtxUnit = GetUnit();
            for (IZ_UINT m = 0; m < COUNTOF(mtxUnit.a); ++m) {
                if (mtx.a[m] != mtxUnit.a[m]) {
                    return IZ_FALSE;
                }
            }
            return IZ_TRUE;
        }

        // マトリクスをゼロクリアする
        static void SetZero(SMatrix33& mtx)
        {
            FILL_ZERO(&mtx, sizeof(mtx));
        }

        // マトリクスを転置する
        static inline void Transpose(SMatrix33& dst, const SMatrix33& src);

        // マトリクスを加算する
        // dst = src1 + src2
        static void Add(SMatrix33& dst, const SMatrix33& src1, const SMatrix33& src2)
        {
            for (IZ_UINT i = 0; i < 3; i++) {
                for (IZ_UINT n = 0; n < 3; n++) {
                    dst.m[i][n] = src1.m[i][n] + src2.m[i][n];
                }
            }
        }

        // マトリクスを減算する
        // dst = src1 - src2
        static void Sub(SMatrix33& dst, const SMatrix33& src1, const SMatrix33& src2)
        {
            for (IZ_UINT i = 0; i < 3; i++) {
                for (IZ_UINT n = 0; n < 3; n++) {
                    dst.m[i][n] = src1.m[i][n] - src2.m[i][n];
                }
            }
        }

        // マトリクスを乗算する
        // dst = src1 x src2
        static inline void Mul(SMatrix33& dst, const SMatrix33& src1, const SMatrix33& src2);

        // マトリクスとスカラーを乗算する
        static void MulScalar(SMatrix33& dst, const SMatrix33& src, IZ_FLOAT fScalar)
        {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    dst.m[i][j] = src.m[i][j] * fScalar;
                }
            }
        }

        // ベクトルとマトリクスを乗算する
        // dst = vec x mtx
        static inline void Apply(SVector3& dst, const SVector3& vec, const SMatrix33& mtx);

        // X軸を回転軸にして回転するマトリクスを取得
        static inline void GetRotByX(SMatrix33& dst, IZ_FLOAT fTheta);

        // Y軸を回転軸にして回転するマトリクスを取得
        static inline void GetRotByY(SMatrix33& dst, IZ_FLOAT fTheta);

        // Z軸を回転軸にして回転するマトリクスを取得
        static inline void GetRotByZ(SMatrix33& dst, IZ_FLOAT fTheta);

        // XYZ軸を回転軸にしてX->Y->Zの順番で回転するマトリクスを取得
        static void GetRotByXYZ(SMatrix33& dst, const SVector3& angle)
        {
            GetRotByX(dst, angle.x);
            RotByY(dst, dst, angle.y);
            RotByZ(dst, dst, angle.z);
        }

        // 任意軸を回転軸にして回転するマトリクスを取得
        static void GetRot(
            SMatrix33& dst, IZ_FLOAT fTheta,
            const SVector3& axis)
        {
            GetRot(dst, fTheta, axis.x, axis.y, axis.z);
        }

        // 任意軸を回転軸にして回転するマトリクスを取得
        static inline void GetRot(
            SMatrix33& dst, IZ_FLOAT fTheta,
            IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z);

        // X軸に対する回転を行なう
        static void RotByX(SMatrix33& dst, const SMatrix33& src, IZ_FLOAT fTheta)
        {
            SMatrix33 m;
            GetRotByX(m, fTheta);
            Mul(dst, src, m);
        }

        // Y軸に対する回転を行なう
        static void RotByY(SMatrix33& dst, const SMatrix33& src, IZ_FLOAT fTheta)
        {
            SMatrix33 m;
            GetRotByY(m, fTheta);
            Mul(dst, src, m);
        }

        // Z軸に対する回転を行なう
        static void RotByZ(SMatrix33& dst, const SMatrix33& src, IZ_FLOAT fTheta)
        {
            SMatrix33 m;
            GetRotByZ(m, fTheta);
            Mul(dst, src, m);
        }

        // X->Y->Z軸の順の回転を行なう
        static void RotByXYZ(SMatrix33& dst, const SMatrix33& src, const SVector3& angle)
        {
            SMatrix33 m;
            RotByX(m, src, angle.x);
            RotByY(m, m, angle.y);
            RotByZ(dst, m, angle.z);
        }

        // 任意軸に対する回転を行なう
        static void Rot(
            SMatrix33& dst, const SMatrix33& src,
            IZ_FLOAT fTheta,
            IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            SMatrix33 m;
            GetRot(m, fTheta, x, y, z);
            Mul(dst, src, m);
        }

        // オフセットを指定したマトリクスを取得
        static void GetTrans(SMatrix33& dst, const SVector3& tv)
        {
            GetTrans(dst, tv.x, tv.y, tv.z);
        }

        // オフセットを指定したマトリクスを取得
        static inline void GetTrans(SMatrix33& dst, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z);

        // 平行移動する
        static void Trans(SMatrix33& dst, const SMatrix33& src, const SVector3& tv)
        {
            SMatrix33 m;
            GetTrans(m, tv);
            Mul(dst, src, m);
        }

        // 平行移動する
        static void Trans(
            SMatrix33& dst,
            const SMatrix33& src,
            IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            SVector3 v;
            v.Set(x, y, z);
            Trans(dst, src, v);
        }

        // Ｘ→Ｙ→Ｚ軸の順で回転を行う
        static void RotXYZ(SMatrix33& dst, const SMatrix33& src, const SVector3& angle)
        {
            RotByX(dst, src, angle.v[0]);
            RotByY(dst, dst, angle.v[1]);
            RotByZ(dst, dst, angle.v[2]);
        }

        // スケーリングマトリクスを得る
        static inline void SetScale(SMatrix33& dst, IZ_FLOAT fScaleX, IZ_FLOAT fScaleY, IZ_FLOAT fScaleZ = 1.0f);

        // スケーリングマトリクスを得る
        static void SetScale(SMatrix33& dst, const SVector3& scale)
        {
            SetScale(
                dst,
                scale.x, scale.y, scale.z);
        }

        // スケーリングする
        static void Scale(SMatrix33& dst, const SMatrix33& src, const SVector3& scale)
        {
            SVector3 s;
            SVector3::Set(s, scale.v[0], scale.v[1], scale.v[2]);

            SVector3::Mul(dst.v[0], src.v[0], s);
            SVector3::Mul(dst.v[1], src.v[1], s);
            SVector3::Mul(dst.v[2], src.v[2], s);
        }

        // スケーリングする
        static void Scale(
            SMatrix33& dst, 
            const SMatrix33& src,
            IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            SVector3 scale;
            scale.Set(x, y, z);

            Scale(dst, src, scale);
        }

        // 行列式を計算する
        static inline IZ_FLOAT Determinant(const SMatrix33& mtx);

        // 一般マトリクスの逆マトリクスを求める
        static inline void Inverse(SMatrix33& dst, const SMatrix33& src);

        // ベクトルから回転マトリクスを求める
        static void GetRotMatrixFromVector(SMatrix33& mtx, const SVector3& vec)
        {
            SVector3 up;
            up.Set(0.0f, 1.0f, 0.0f);

            GetRotMatrixFromVector(mtx, vec, up);
        }

        // ベクトルから回転マトリクスを求める
        static void GetRotMatrixFromVector(SMatrix33& mtx, const SVector3& vec, const SVector3& up);

        // マトリクスからオイラー角を取得する
        static void GetEulerFromMatrix(SVector3& angle, const SMatrix33& mtx, E_MATH_ROTATION_ORDER order);

        // マトリクスの中身をダンプする
        static void Dump(const SMatrix33& mtx)
        {
            for (IZ_UINT i = 0; i < 3; i++) {
                IZ_PRINTF("%f, %f, %f\n", mtx.m[i][0], mtx.m[i][1], mtx.m[i][2]);
            }
        }
    };
}   // namespace math
}   // namespace izanagi

#include "math/noplatform/MathMatrix33_NoPlatform.h"

#endif  // #if !defined(__IZANAGI__MATH_MATRIX44_H__)
