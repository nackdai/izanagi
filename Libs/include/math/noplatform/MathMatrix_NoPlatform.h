#if !defined(__IZANAGI__MATH_MATRIX_NO_PLATFORM_H__)
#define __IZANAGI__MATH_MATRIX_NO_PLATFORM_H__

#include <math.h>
#include "izDefs.h"
#include "math/MathDefs.h"
#include "math/MathVector.h"

namespace izanagi
{
namespace math
{
    // 単位マトリクスを設定する
    void SMatrix::SetUnit(SMatrix& mtx)
    {
        Copy(mtx, GetUnit());
    }

    // マトリクスを転置する
    void SMatrix::Transpose(SMatrix& dst, const SMatrix& src)
    {
        SMatrix m;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m.m[i][j] = src.m[j][i];
            }
        }

        Copy(dst, m);
    }

    // マトリクスを乗算する
    // dst = src1 x src2
    void SMatrix::Mul(SMatrix& dst, const SMatrix& src1, const SMatrix& src2)
    {
        SMatrix m;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m.m[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    m.m[i][j] += src1.m[i][k] * src2.m[k][j];
                }
            }
        }

        Copy(dst, m);
    }

    // ベクトルとマトリクスを乗算する
    // dst = vec x mtx
    void SMatrix::Apply(SVector& dst, const SVector& vec, const SMatrix& mtx)
    {
        SVector v;
        for (int i = 0; i < 4; ++i) {
            v.v[i] = 0.0f;
            for (int j = 0; j < 4; ++j) {
                v.v[i] += vec.v[j] * mtx.m[j][i];
            }
        }

        SVector::Copy(dst, v);
    }

    // ベクトルとマトリクスのＸＹＺ成分のみを乗算する
    // ベクトルのＷ成分、およびマトリクスの第４行は無視される
    void SMatrix::ApplyXYZ(SVector& dst, const SVector& vec, const SMatrix& mtx)
    {
        SVector v;
        for (int i = 0; i < 3; ++i) {
            v.v[i] = 0.0f;
            for (int j = 0; j < 3; ++j) {
                v.v[i] += vec.v[j] * mtx.m[j][i];
            }
        }

        dst.x = v.x;
        dst.y = v.y;
        dst.z = v.z;
        
        //v.v[3] = vec.v[3];
    }

    // X軸を回転軸にして回転するマトリクスを取得
    void SMatrix::GetRotByX(SMatrix& dst, IZ_FLOAT fTheta)
    {
        const IZ_FLOAT c = cosf(fTheta);
        const IZ_FLOAT s = sinf(fTheta);

        dst.m[0][0] = 1.0f; dst.m[0][1] = 0.0f; dst.m[0][2] = 0.0f; dst.m[0][3] = 0.0f;
        dst.m[1][0] = 0.0f; dst.m[1][1] = c;    dst.m[1][2] = s;    dst.m[1][3] = 0.0f;
        dst.m[2][0] = 0.0f; dst.m[2][1] = -s;   dst.m[2][2] = c;    dst.m[2][3] = 0.0f;
        dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
    }

    // Y軸を回転軸にして回転するマトリクスを取得
    void SMatrix::GetRotByY(SMatrix& dst, IZ_FLOAT fTheta)
    {
        const IZ_FLOAT c = cosf(fTheta);
        const IZ_FLOAT s = sinf(fTheta);

        dst.m[0][0] = c;    dst.m[0][1] = 0.0f; dst.m[0][2] = -s;   dst.m[0][3] = 0.0f;
        dst.m[1][0] = 0.0f; dst.m[1][1] = 1.0f; dst.m[1][2] = 0.0f; dst.m[1][3] = 0.0f;
        dst.m[2][0] = s;    dst.m[2][1] = 0.0f; dst.m[2][2] = c;    dst.m[2][3] = 0.0f;
        dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
    }

    // Z軸を回転軸にして回転するマトリクスを取得
    void SMatrix::GetRotByZ(SMatrix& dst, IZ_FLOAT fTheta)
    {
        const IZ_FLOAT c = cosf(fTheta);
        const IZ_FLOAT s = sinf(fTheta);

        dst.m[0][0] = c;    dst.m[0][1] = s;    dst.m[0][2] = 0.0f; dst.m[0][3] = 0.0f;
        dst.m[1][0] = -s;   dst.m[1][1] = c;    dst.m[1][2] = 0.0f; dst.m[1][3] = 0.0f;
        dst.m[2][0] = 0.0f; dst.m[2][1] = 0.0f; dst.m[2][2] = 1.0f; dst.m[2][3] = 0.0f;
        dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
    }

    // スケーリングマトリクスを得る
    void SMatrix::SetScale(SMatrix& dst, IZ_FLOAT fScaleX, IZ_FLOAT fScaleY, IZ_FLOAT fScaleZ)
    {
        SVector::Set(dst.v[0], fScaleX, 0.0f, 0.0f, 0.0f);
        SVector::Set(dst.v[1], 0.0f, fScaleY, 0.0f, 0.0f);
        SVector::Set(dst.v[2], 0.0f, 0.0f, fScaleZ, 0.0f);

        SVector::Set(dst.v[3], 0.0f, 0.0f, 0.0f, 1.0f);
    }

    // 任意軸を回転軸にして回転するマトリクスを取得
    void SMatrix::GetRot(
        SMatrix& dst, IZ_FLOAT fTheta,
        IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
    {
        IZ_FLOAT c = ::cosf(fTheta);
        IZ_FLOAT _1_c = 1.0f - c;

        IZ_FLOAT s = ::sinf(fTheta);

        dst._00 = c + x * x * _1_c;
        dst._01 = x * y * _1_c + z * s;
        dst._02 = x * z * _1_c - y * s;
        dst._03 = 0.0f;

        dst._10 = x * y * _1_c - z * s;
        dst._11 = c + y * y * _1_c;
        dst._12 = y * z * _1_c + x * s;
        dst._13 = 0.0f;

        dst._20 = x * z * _1_c + y * s;
        dst._21 = y * z * _1_c - x * s;
        dst._22 = c + z * z * _1_c;
        dst._23 = 0.0f;

        dst._30 = 0.0f;
        dst._31 = 0.0f;
        dst._32 = 0.0f;
        dst._33 = 1.0f;
    }

    void SMatrix::GetTrans(SMatrix& dst, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
    {
        dst.m[0][0] = 1.0f; dst.m[0][1] = 0.0f; dst.m[0][2] = 0.0f; dst.m[0][3] = 0.0f;
        dst.m[1][0] = 0.0f; dst.m[1][1] = 1.0f; dst.m[1][2] = 0.0f; dst.m[1][3] = 0.0f;
        dst.m[2][0] = 0.0f; dst.m[2][1] = 0.0f; dst.m[2][2] = 1.0f; dst.m[2][3] = 0.0f;
        dst.m[3][0] = x;    dst.m[3][1] = y;    dst.m[3][2] = z;    dst.m[3][3] = 1.0f;
    }

    // 行列式を計算する
    IZ_FLOAT SMatrix::Determinant(const SMatrix& mtx)
    {
        // TODO
        IZ_ASSERT(IZ_FALSE);
        return 0.0f;
    }

    // 逆マトリクスを Gauss/Jordan 法で求める
    void SMatrix::Inverse(SMatrix& dst, const SMatrix& src)
    {
        // Gauss/Jordan法で求める
        SMatrix mtx;
        Copy(mtx, src);
        SMatrix::SetUnit(dst);

        for (int i = 0; i < 4; ++i) {
            // ピボット選択
            // NOTE: 対象となる列中の最大値が対角値になるように行を入れ替える
            IZ_FLOAT f = CMath::Absf(mtx.m[i][i]);
            for (int j = i + 1; j < 4; ++j) {
                if (f < CMath::Absf(mtx.m[j][i])) {
                    f = CMath::Absf(mtx.m[j][i]);
                    SVector::Swap(mtx.v[i], mtx.v[j]);
                    SVector::Swap(dst.v[i], dst.v[j]);
                }
            }

            // 対象となる行の対角値を 1 にする
            f = 1.0f / mtx.m[i][i];
            SVector::Scale(mtx.v[i], mtx.v[i], f);
            SVector::Scale(dst.v[i], dst.v[i], f);

            // 対象とならない列の値を 0 にする
            for (int j = 0; j < 4; ++j) {
                if (j != i) {
                    IZ_FLOAT fTemp = mtx.m[j][i];

                    SVector v1, v2;
                    SVector::Scale(v1, mtx.v[i], fTemp);
                    SVector::Scale(v2, dst.v[i], fTemp);

                    SVector::Sub(mtx.v[j], mtx.v[j], v1);
                    SVector::Sub(dst.v[j], dst.v[j], v2);
                }
            }
        }
    }
}   // namespace math
}    // namespace izanagi

#endif    // #if !defined(__IZANAGI__MATH_MATRIX_NO_PLATFORM_H__)
