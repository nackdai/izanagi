#if !defined(__IZANAGI__MATH_MATRIX_H__)
#define __IZANAGI__MATH_MATRIX_H__

#include <math.h>
#include "izDefs.h"
#include "MathDefs.h"
#include "MathVector.h"

namespace izanagi {

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

#if 0
		// 単位マトリクスを参照する
		const SMatrix& GetUnit();
#else
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
#endif

		// 単位マトリクスを設定する
		static void SetUnit(SMatrix& mtx)
		{
#if defined(__USE_D3D_MATH__)
			D3DXMatrixIdentity(reinterpret_cast<D3DXMATRIX*>(&mtx));
#else	// #if defined(__USE_D3D_MATH__)
			Copy(mtx, GetUnit());
#endif	// #if defined(__USE_D3D_MATH__)
		}

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
		static void Transpose(SMatrix& dst, const SMatrix& src)
		{
#if defined(__USE_D3D_MATH__)
			D3DXMatrixTranspose(
				reinterpret_cast<D3DXMATRIX*>(&dst),
				reinterpret_cast<const D3DXMATRIX*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
			SMatrix m;
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					m.m[i][j] = src.m[j][i];
				}
			}

			Copy(dst, m);
#endif	// #if defined(__USE_D3D_MATH__)
		}

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
		static void Mul(SMatrix& dst, const SMatrix& src1, const SMatrix& src2)
		{
#if defined(__USE_D3D_MATH__)
			D3DXMatrixMultiply(
				reinterpret_cast<D3DXMATRIX*>(&dst),
				reinterpret_cast<const D3DXMATRIX*>(&src1),
				reinterpret_cast<const D3DXMATRIX*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
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
#endif	// #if defined(__USE_D3D_MATH__)
		}

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
		static void Apply(SVector& dst, const SVector& vec, const SMatrix& mtx)
		{
#if defined(__USE_D3D_MATH__)
			D3DXVec4Transform(
				reinterpret_cast<D3DXVECTOR4*>(&dst),
				reinterpret_cast<const D3DXVECTOR4*>(&vec),
				reinterpret_cast<const D3DXMATRIX*>(&mtx));
#else	// #if defined(__USE_D3D_MATH__)
			SVector v;
			for (int i = 0; i < 4; ++i) {
				v.v[i] = 0.0f;
				for (int j = 0; j < 4; ++j) {
					v.v[i] += vec.v[j] * mtx.m[j][i];
				}
			}

			SVector::Copy(dst, v);
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// ベクトルとマトリクスのＸＹＺ成分のみを乗算する
		// ベクトルのＷ成分、およびマトリクスの第４行は無視される
		static void ApplyXYZ(SVector& dst, const SVector& vec, const SMatrix& mtx)
		{
#if defined(__USE_D3D_MATH__)
			SVector v;
			SVector::Copy(v, vec);
			v.w = 0.0f;

			D3DXVec4Transform(
				reinterpret_cast<D3DXVECTOR4*>(&dst),
				reinterpret_cast<const D3DXVECTOR4*>(&v),
				reinterpret_cast<const D3DXMATRIX*>(&mtx));

			v.w = vec.w;
#else	// #if defined(__USE_D3D_MATH__)
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
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// X軸を回転軸にして回転するマトリクスを取得
		static void GetRotByX(SMatrix& dst, IZ_FLOAT fTheta);

		// Y軸を回転軸にして回転するマトリクスを取得
		static void GetRotByY(SMatrix& dst, IZ_FLOAT fTheta);

		// Z軸を回転軸にして回転するマトリクスを取得
		static void GetRotByZ(SMatrix& dst, IZ_FLOAT fTheta);

		// 任意軸を回転軸にして回転するマトリクスを取得
		static void GetRot(
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
		static void GetTrans(SMatrix& dst, const SVector& tv);

		// 平行移動する
		static void Trans(SMatrix& dst, const SMatrix& src, const SVector& tv)
		{
			SMatrix m;
			GetTrans(m, tv);
			Mul(dst, src, m);
		}

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
		static void SetScale(SMatrix& dst, IZ_FLOAT fScaleX, IZ_FLOAT fScaleY, IZ_FLOAT fScaleZ)
		{
#if defined(__USE_D3D_MATH__)
			D3DXMatrixScaling(
				reinterpret_cast<D3DXMATRIX*>(&dst),
				fScaleX,
				fScaleY,
				fScaleZ);
#else	// #if defined(__USE_D3D_MATH__)
			SVector::Set(dst.v[0], fScaleX, 0.0f, 0.0f, 0.0f);
			SVector::Set(dst.v[1], 0.0f, fScaleY, 0.0f, 0.0f);
			SVector::Set(dst.v[2], 0.0f, 0.0f, fScaleZ, 0.0f);

			SVector::Set(dst.v[3], 0.0f, 0.0f, 0.0f, 1.0f);
#endif	// #if defined(__USE_D3D_MATH__)
		}

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
		static IZ_FLOAT Determinant(const SMatrix& mtx);

		// 一般マトリクスの逆マトリクスを求める
		static void Inverse(SMatrix& dst, const SMatrix& src);

		// ベクトルから回転マトリクスを求める
		static void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec);
		static void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec, const SVector& up);

		// マトリクスの中身をダンプする
		static void Dump(const SMatrix& mtx)
		{
			for (IZ_UINT i = 0; i < 4; i++) {
				IZ_PRINTF("%f, %f, %f, %f\n", mtx.m[i][0], mtx.m[i][1], mtx.m[i][2], mtx.m[i][3]);
			}
		}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_MATRIX_H__)
