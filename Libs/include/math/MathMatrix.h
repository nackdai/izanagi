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
	};

	// マトリクスをコピーする
	inline void CopyMatrix(SMatrix& dst, const SMatrix& src)
	{
		memcpy(&dst, &src, sizeof(SMatrix));
	}

#if 0
	// 単位マトリクスを参照する
	const SMatrix& GetUnitMatrix();
#else
	// 単位マトリクスを参照する
	inline const SMatrix& GetUnitMatrix()
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
	inline void SetUnitMatrix(SMatrix& mtx)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixIdentity(reinterpret_cast<D3DXMATRIX*>(&mtx));
#else	// #if defined(__USE_D3D_MATH__)
		CopyMatrix(mtx, GetUnitMatrix());
#endif	// #if defined(__USE_D3D_MATH__)
	}

	inline IZ_BOOL IsUnitMatrix(const SMatrix& mtx)
	{
		const SMatrix& mtxUnit = GetUnitMatrix();
		for (IZ_UINT m = 0; m < COUNTOF(mtxUnit.a); ++m) {
			if (mtx.a[m] != mtxUnit.a[m]) {
				return IZ_FALSE;
			}
		}
		return IZ_TRUE;
	}

	// マトリクスをゼロクリアする
	inline void SetZeroMatrix(SMatrix& mtx)
	{
		FILL_ZERO(&mtx, sizeof(mtx));
	}

	// マトリクスを転置する
	inline void TransposeMatrix(SMatrix& dst, const SMatrix& src)
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

		CopyMatrix(dst, m);
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// マトリクスを乗算する
	// dst = src1 x src2
	inline void MulMatrix(SMatrix& dst, const SMatrix& src1, const SMatrix& src2)
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

		CopyMatrix(dst, m);
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// マトリクスとスカラーを乗算する
	inline void MulScalarMatrix(SMatrix& dst, const SMatrix& src, IZ_FLOAT fScalar)
	{
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				dst.m[i][j] = src.m[i][j] * fScalar;
			}
		}
	}

	// ベクトルとマトリクスを乗算する
	// dst = vec x mtx
	inline void ApplyMatrix(SVector& dst, const SVector& vec, const SMatrix& mtx)
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
	inline void ApplyMatrixXYZ(SVector& dst, const SVector& vec, const SMatrix& mtx)
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
	void GetRotMatrixX(SMatrix& dst, IZ_FLOAT fTheta);

	// Y軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrixY(SMatrix& dst, IZ_FLOAT fTheta);

	// Z軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrixZ(SMatrix& dst, IZ_FLOAT fTheta);

	// 任意軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrix(
		SMatrix& dst, IZ_FLOAT fTheta,
		IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z);

	// X軸に対する回転を行なう
	inline void RotMatrixX(SMatrix& dst, const SMatrix& src, IZ_FLOAT fTheta)
	{
		SMatrix m;
		GetRotMatrixX(m, fTheta);
		MulMatrix(dst, src, m);
	}

	// Y軸に対する回転を行なう
	inline void RotMatrixY(SMatrix& dst, const SMatrix& src, IZ_FLOAT fTheta)
	{
		SMatrix m;
		GetRotMatrixY(m, fTheta);
		MulMatrix(dst, src, m);
	}

	// Z軸に対する回転を行なう
	inline void RotMatrixZ(SMatrix& dst, const SMatrix& src, IZ_FLOAT fTheta)
	{
		SMatrix m;
		GetRotMatrixZ(m, fTheta);
		MulMatrix(dst, src, m);
	}

	// 任意軸に対する回転を行なう
	inline void RotMatrix(
		SMatrix& dst, const SMatrix& src,
		IZ_FLOAT fTheta,
		IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
	{
		SMatrix m;
		GetRotMatrix(m, fTheta, x, y, z);
		MulMatrix(dst, src, m);
	}

	// オフセットを指定したマトリクスを取得
	void GetTransMatrix(SMatrix& dst, const SVector& tv);

	// 平行移動する
	inline void TransMatrix(SMatrix& dst, const SMatrix& src, const SVector& tv)
	{
		SMatrix m;
		GetTransMatrix(m, tv);
		MulMatrix(dst, src, m);
	}

	inline void TransMatrix(
		SMatrix& dst,
		const SMatrix& src,
		IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
	{
		SVector v;
		v.Set(x, y, z);
		TransMatrix(dst, src, v);
	}

	// Ｘ→Ｙ→Ｚ軸の順で回転を行う
	inline void RotMatrixXYZ(SMatrix& dst, const SMatrix& src, const SVector& angle)
	{
		RotMatrixX(dst, src, angle.v[0]);
		RotMatrixY(dst, dst, angle.v[1]);
		RotMatrixZ(dst, dst, angle.v[2]);
	}

	// スケーリングマトリクスを得る
	inline void SetScaleMatrix(SMatrix& dst, IZ_FLOAT fScaleX, IZ_FLOAT fScaleY, IZ_FLOAT fScaleZ)
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
	inline void SetScaleMatrix(SMatrix& dst, const SVector& scale)
	{
		SetScaleMatrix(
			dst,
			scale.x, scale.y, scale.z);
	}

	// スケーリングする
	inline void ScaleMatrix(SMatrix& dst, const SMatrix& src, const SVector& scale)
	{
		SVector s;
		SVector::Set(s, scale.v[0], scale.v[1], scale.v[2], 1.0f);

		SVector::Mul(dst.v[0], src.v[0], s);
		SVector::Mul(dst.v[1], src.v[1], s);
		SVector::Mul(dst.v[2], src.v[2], s);
		SVector::Mul(dst.v[3], src.v[3], s);
	}

	inline void ScaleMatrix(
		SMatrix& dst, 
		const SMatrix& src,
		IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
	{
		SVector scale;
		scale.Set(x, y, z);

		ScaleMatrix(dst, src, scale);
	}

	// 行列式を計算する
	IZ_FLOAT DeterminantMatrix(const SMatrix& mtx);

	// 一般マトリクスの逆マトリクスを求める
	void InverseMatrix(SMatrix& dst, const SMatrix& src);

	// ベクトルから回転マトリクスを求める
	void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec);
	void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec, const SVector& up);
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_MATRIX_H__)
