#if !defined(__IZANAGI__MATH_MATRIX_H__)
#define __IZANAGI__MATH_MATRIX_H__

#include <math.h>
#include "izDefs.h"
#include "MathDefs.h"
#include "MathVector.h"

namespace izanagi
{
namespace math
{
	/** 回転計算の順番
	 */
	enum E_MATH_ROTATION_ORDER
	{
		E_MATH_ROTATION_ORDER_XYZ = 0,
		E_MATH_ROTATION_ORDER_XZY,
		E_MATH_ROTATION_ORDER_YXZ,
		E_MATH_ROTATION_ORDER_YZX,
		E_MATH_ROTATION_ORDER_ZXY,
		E_MATH_ROTATION_ORDER_ZYX,

		E_MATH_ROTATION_ORDER_NUM,
		E_MATH_ROTATION_ORDER_FORCE_INT32 = 0x7fffffff,
	};

	/**
	 */
	struct SMatrix {
		union {
			IZ_FLOAT a[16];
			IZ_FLOAT m[4][4];
			SVector v[4];
			struct {
				IZ_FLOAT _00, _01, _02, _03;
				IZ_FLOAT _10, _11, _12, _13;
				IZ_FLOAT _20, _21, _22, _23;
				IZ_FLOAT _30, _31, _32, _33;
			};
		};

		// マトリクスをコピーする
		static void Copy(SMatrix& dst, const SMatrix& src)
		{
			memcpy(&dst, &src, sizeof(SMatrix));
		}

		// 単位マトリクスを参照する
		static const SMatrix& GetUnit()
		{
			static SMatrix mtxUnit = {
				1.0f, 0.0f, 0.0f, 0.0f, 
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};

			return mtxUnit;
		}

		// 単位マトリクスを設定する
		static inline void SetUnit(SMatrix& mtx);

		static IZ_BOOL IsUnit(const SMatrix& mtx)
		{
			const SMatrix& mtxUnit = GetUnit();
			for (IZ_UINT m = 0; m < COUNTOF(mtxUnit.a); ++m) {
				if (mtx.a[m] != mtxUnit.a[m]) {
					return IZ_FALSE;
				}
			}
			return IZ_TRUE;
		}

		// マトリクスをゼロクリアする
		static void SetZero(SMatrix& mtx)
		{
			FILL_ZERO(&mtx, sizeof(mtx));
		}

		// マトリクスを転置する
		static inline void Transpose(SMatrix& dst, const SMatrix& src);

		// マトリクスを加算する
		// dst = src1 + src2
		static void Add(SMatrix& dst, const SMatrix& src1, const SMatrix& src2)
		{
			for (IZ_UINT i = 0; i < 4; i++) {
				for (IZ_UINT n = 0; n < 4; n++) {
					dst.m[i][n] = src1.m[i][n] + src2.m[i][n];
				}
			}
		}

		// マトリクスを減算する
		// dst = src1 - src2
		static void Sub(SMatrix& dst, const SMatrix& src1, const SMatrix& src2)
		{
			for (IZ_UINT i = 0; i < 4; i++) {
				for (IZ_UINT n = 0; n < 4; n++) {
					dst.m[i][n] = src1.m[i][n] - src2.m[i][n];
				}
			}
		}

		// マトリクスを乗算する
		// dst = src1 x src2
		static inline void Mul(SMatrix& dst, const SMatrix& src1, const SMatrix& src2);

		// マトリクスとスカラーを乗算する
		static void MulScalar(SMatrix& dst, const SMatrix& src, IZ_FLOAT fScalar)
		{
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					dst.m[i][j] = src.m[i][j] * fScalar;
				}
			}
		}

		// ベクトルとマトリクスを乗算する
		// dst = vec x mtx
		static inline void Apply(SVector& dst, const SVector& vec, const SMatrix& mtx);

		// ベクトルとマトリクスのＸＹＺ成分のみを乗算する
		// ベクトルのＷ成分、およびマトリクスの第４行は無視される
		static inline void ApplyXYZ(SVector& dst, const SVector& vec, const SMatrix& mtx);

		// X軸を回転軸にして回転するマトリクスを取得
		static inline void GetRotByX(SMatrix& dst, IZ_FLOAT fTheta);

		// Y軸を回転軸にして回転するマトリクスを取得
		static inline void GetRotByY(SMatrix& dst, IZ_FLOAT fTheta);

		// Z軸を回転軸にして回転するマトリクスを取得
		static inline void GetRotByZ(SMatrix& dst, IZ_FLOAT fTheta);

		// XYZ軸を回転軸にしてX->Y->Zの順番で回転するマトリクスを取得
		static void GetRotByXYZ(SMatrix& dst, const SVector& angle)
        {
		    GetRotByX(dst, angle.x);
		    RotByY(dst, dst, angle.y);
		    RotByZ(dst, dst, angle.z);
	    }

		// 任意軸を回転軸にして回転するマトリクスを取得
		static void GetRot(
			SMatrix& dst, IZ_FLOAT fTheta,
			const SVector& axis)
        {
		    GetRot(dst, fTheta, axis.x, axis.y, axis.z);
	    }

		// 任意軸を回転軸にして回転するマトリクスを取得
		static inline void GetRot(
			SMatrix& dst, IZ_FLOAT fTheta,
			IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z);

		// X軸に対する回転を行なう
		static void RotByX(SMatrix& dst, const SMatrix& src, IZ_FLOAT fTheta)
		{
			SMatrix m;
			GetRotByX(m, fTheta);
			Mul(dst, src, m);
		}

		// Y軸に対する回転を行なう
		static void RotByY(SMatrix& dst, const SMatrix& src, IZ_FLOAT fTheta)
		{
			SMatrix m;
			GetRotByY(m, fTheta);
			Mul(dst, src, m);
		}

		// Z軸に対する回転を行なう
		static void RotByZ(SMatrix& dst, const SMatrix& src, IZ_FLOAT fTheta)
		{
			SMatrix m;
			GetRotByZ(m, fTheta);
			Mul(dst, src, m);
		}

		// X->Y->Z軸の順の回転を行なう
		static void RotByXYZ(SMatrix& dst, const SMatrix& src, const SVector& angle)
		{
			SMatrix m;
			RotByX(m, src, angle.x);
			RotByY(m, m, angle.y);
			RotByZ(dst, m, angle.z);
		}

		// 任意軸に対する回転を行なう
		static void Rot(
			SMatrix& dst, const SMatrix& src,
			IZ_FLOAT fTheta,
			IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
		{
			SMatrix m;
			GetRot(m, fTheta, x, y, z);
			Mul(dst, src, m);
		}

		// オフセットを指定したマトリクスを取得
		static void GetTrans(SMatrix& dst, const SVector& tv)
        {
		    GetTrans(dst, tv.x, tv.y, tv.z);
	    }

        // オフセットを指定したマトリクスを取得
		static inline void GetTrans(SMatrix& dst, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z);

		// 平行移動する
		static void Trans(SMatrix& dst, const SMatrix& src, const SVector& tv)
		{
			SMatrix m;
			GetTrans(m, tv);
			Mul(dst, src, m);
		}

        // 平行移動する
		static void Trans(
			SMatrix& dst,
			const SMatrix& src,
			IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
		{
			SVector v;
			v.Set(x, y, z);
			Trans(dst, src, v);
		}

		// Ｘ→Ｙ→Ｚ軸の順で回転を行う
		static void RotXYZ(SMatrix& dst, const SMatrix& src, const SVector& angle)
		{
			RotByX(dst, src, angle.v[0]);
			RotByY(dst, dst, angle.v[1]);
			RotByZ(dst, dst, angle.v[2]);
		}

		// スケーリングマトリクスを得る
		static inline void SetScale(SMatrix& dst, IZ_FLOAT fScaleX, IZ_FLOAT fScaleY, IZ_FLOAT fScaleZ);

		// スケーリングマトリクスを得る
		static void SetScale(SMatrix& dst, const SVector& scale)
		{
			SetScale(
				dst,
				scale.x, scale.y, scale.z);
		}

		// スケーリングする
		static void Scale(SMatrix& dst, const SMatrix& src, const SVector& scale)
		{
			SVector s;
			SVector::Set(s, scale.v[0], scale.v[1], scale.v[2], 1.0f);

			SVector::Mul(dst.v[0], src.v[0], s);
			SVector::Mul(dst.v[1], src.v[1], s);
			SVector::Mul(dst.v[2], src.v[2], s);
			SVector::Mul(dst.v[3], src.v[3], s);
		}

        // スケーリングする
		static void Scale(
			SMatrix& dst, 
			const SMatrix& src,
			IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
		{
			SVector scale;
			scale.Set(x, y, z);

			Scale(dst, src, scale);
		}

		// 行列式を計算する
		static inline IZ_FLOAT Determinant(const SMatrix& mtx);

		// 一般マトリクスの逆マトリクスを求める
		static inline void Inverse(SMatrix& dst, const SMatrix& src);

		// ベクトルから回転マトリクスを求める
		static void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec)
        {
		    SVector up;
		    up.Set(0.0f, 1.0f, 0.0f, 0.0f);

		    GetRotMatrixFromVector(mtx, vec, up);
	    }

        // ベクトルから回転マトリクスを求める
		static void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec, const SVector& up);

		// マトリクスからオイラー角を取得する
		static void GetEulerFromMatrix(SVector& angle, const SMatrix& mtx, E_MATH_ROTATION_ORDER order);

		// マトリクスの中身をダンプする
		static void Dump(const SMatrix& mtx)
		{
			for (IZ_UINT i = 0; i < 4; i++) {
				IZ_PRINTF("%f, %f, %f, %f\n", mtx.m[i][0], mtx.m[i][1], mtx.m[i][2], mtx.m[i][3]);
			}
		}
	};
}   // namespace math
}	// namespace izanagi

#ifdef __USE_D3D_MATH__
    #include "math/dx9/MathMatrix_DX9.h"
#else   // #ifdef __USE_D3D_MATH__
    #include "math/noplatform/MathMatrix_NoPlatform.h"
#endif

#endif	// #if !defined(__IZANAGI__MATH_MATRIX_H__)
