﻿#include "math/MathMatrix.h"

namespace izanagi {
#if 0
	static SMatrix mtxUnit = {
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	// 単位マトリクスを参照する
	const SMatrix& GetUnitMatrix()
	{
		return mtxUnit;
	}
#endif

	// X軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrixX(SMatrix& dst, IZ_FLOAT fTheta)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixRotationX(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			fTheta);
#else	// #if defined(__USE_D3D_MATH__)
		const IZ_FLOAT c = cosf(fTheta);
		const IZ_FLOAT s = sinf(fTheta);

		dst.m[0][0] = 1.0f; dst.m[0][1] = 0.0f; dst.m[0][2] = 0.0f; dst.m[0][3] = 0.0f;
		dst.m[1][0] = 0.0f; dst.m[1][1] = c;    dst.m[1][2] = s;    dst.m[1][3] = 0.0f;
		dst.m[2][0] = 0.0f; dst.m[2][1] = -s;   dst.m[2][2] = c;    dst.m[2][3] = 0.0f;
		dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// Y軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrixY(SMatrix& dst, IZ_FLOAT fTheta)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixRotationY(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			fTheta);
#else	// #if defined(__USE_D3D_MATH__)
		const IZ_FLOAT c = cosf(fTheta);
		const IZ_FLOAT s = sinf(fTheta);

		dst.m[0][0] = c;    dst.m[0][1] = 0.0f; dst.m[0][2] = -s;   dst.m[0][3] = 0.0f;
		dst.m[1][0] = 0.0f; dst.m[1][1] = 1.0f; dst.m[1][2] = 0.0f; dst.m[1][3] = 0.0f;
		dst.m[2][0] = s;    dst.m[2][1] = 0.0f; dst.m[2][2] = c;    dst.m[2][3] = 0.0f;
		dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// Z軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrixZ(SMatrix& dst, IZ_FLOAT fTheta)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixRotationZ(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			fTheta);
#else	// #if defined(__USE_D3D_MATH__)
		const IZ_FLOAT c = cosf(fTheta);
		const IZ_FLOAT s = sinf(fTheta);

		dst.m[0][0] = c;    dst.m[0][1] = s;    dst.m[0][2] = 0.0f; dst.m[0][3] = 0.0f;
		dst.m[1][0] = -s;   dst.m[1][1] = c;    dst.m[1][2] = 0.0f; dst.m[1][3] = 0.0f;
		dst.m[2][0] = 0.0f; dst.m[2][1] = 0.0f; dst.m[2][2] = 1.0f; dst.m[2][3] = 0.0f;
		dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// 任意軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrix(
		SMatrix& dst, IZ_FLOAT fTheta,
		IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVECTOR3 v(x, y, z);

		D3DXMatrixRotationAxis(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			&v,
			fTheta);
#else
		IZ_FLAOT c = ::cosf(fTheta);
		IZ_FLOAT 1_c = 1.0f - c;

		IZ_FLOAT s = ::sinf(fTheta);

		dst._00 = c + x * x * 1_c;
		dst._01 = x * y * 1_c + z * s;
		dst._02 = x * z * 1_c - y * s;
		dst._03 = 0.0f;

		dst._10 = x * y * 1_c - z * s;
		dst._11 = c + y * y * 1_c;
		dst._12 = y * z * 1_c + x * s;
		dst._13 = 0.0f;

		dst._20 = x * z * 1_c + y * s;
		dst._21 = y * z * 1_c - x * s;
		dst._22 = c + z * z * 1_c;
		dst._23 = 0.0f;

		dst._30 = 0.0f;
		dst._31 = 0.0f;
		dst._32 = 0.0f;
		dst._33 = 1.0f;
#endif
	}

	// オフセットを指定したマトリクスを取得
	void GetTransMatrix(SMatrix& dst, const SVector& tv)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixTranslation(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			tv.x,
			tv.y,
			tv.z);
#else	// #if defined(__USE_D3D_MATH__)
		dst.m[0][0] = 1.0f;    dst.m[0][1] = 0.0f;    dst.m[0][2] = 0.0f;    dst.m[0][3] = 0.0f;
		dst.m[1][0] = 0.0f;    dst.m[1][1] = 1.0f;    dst.m[1][2] = 0.0f;    dst.m[1][3] = 0.0f;
		dst.m[2][0] = 0.0f;    dst.m[2][1] = 0.0f;    dst.m[2][2] = 1.0f;    dst.m[2][3] = 0.0f;
		dst.m[3][0] = tv.v[0]; dst.m[3][1] = tv.v[1]; dst.m[3][2] = tv.v[2]; dst.m[3][3] = 1.0f;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// 行列式を計算する
	IZ_FLOAT DeterminantMatrix(const SMatrix& mtx)
	{
#if defined(__USE_D3D_MATH__)
		IZ_FLOAT ret = D3DXMatrixDeterminant(reinterpret_cast<const D3DXMATRIX*>(&mtx));
		return ret;
#else
		// TODO
		IZ_ASSERT(IZ_FALSE);
		return 0.0f;
#endif
	}

	// 逆マトリクスを Gauss/Jordan 法で求める
	void InverseMatrix(SMatrix& dst, const SMatrix& src)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixInverse(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			IZ_NULL,	// 行列式はいらない
			reinterpret_cast<const D3DXMATRIX*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
		// Gauss/Jordan法で求める
		SMatrix mtx;
		CopyMatrix(mtx, src);
		SetUnitMatrix(dst);

		for (int i = 0; i < 4; ++i) {
			// ピボット選択
			// NOTE: 対象となる列中の最大値が対角値になるように行を入れ替える
			IZ_FLOAT f = CMath::Absf(mtx.m[i][i]);
			for (int j = i + 1; j < 4; ++j) {
				if (f < CMath::Absf(mtx.m[j][i])) {
					f = CMath::Absf(mtx.m[j][i]);
					SwapVector(mtx.v[i], mtx.v[j]);
					SwapVector(dst.v[i], dst.v[j]);
				}
			}

			// 対象となる行の対角値を 1 にする
			f = 1.0f / mtx.m[i][i];
			ScaleVector(mtx.v[i], mtx.v[i], f);
			ScaleVector(dst.v[i], dst.v[i], f);

			// 対象とならない列の値を 0 にする
			for (int j = 0; j < 4; ++j) {
				if (j != i) {
					IZ_FLOAT fTemp = mtx.m[j][i];

					SVector v1, v2;
					ScaleVector(v1, mtx.v[i], fTemp);
					ScaleVector(v2, dst.v[i], fTemp);

					SubVector(mtx.v[j], mtx.v[j], v1);
					SubVector(dst.v[j], dst.v[j], v2);
				}
			}
		}
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// ベクトルから回転マトリクスを求める
	void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec)
	{
		SVector up;
		up.Set(0.0f, 1.0f, 0.0f, 0.0f);

		GetRotMatrixFromVector(mtx, vec, up);
	}

	void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec, const SVector& up)
	{
		SVector& vX = mtx.v[0];
		SVector& vY = mtx.v[1];
		SVector& vZ = mtx.v[2];

		SVector::Normalize(vZ, vec);

		SVector vUp;
		if (CMath::IsNearyEqualZero(vZ.x) && CMath::IsNearyEqualZero(vZ.z)) {
			// UPベクトルとの外積を計算できないので、
			// 新しいUPベクトルをでっちあげる・・・
			if (up.y > 0.0f) {
				vUp.Set(0.0f, 0.0f, -vZ.y, 0.0f);
			}
			else {
				vUp.Set(0.0f, 0.0f, vZ.y, 0.0f);
			}
		}
		else {
			SVector::Copy(vUp, up);
		}

		SVector::Cross(vX, vUp, vZ);
		SVector::Normalize(vX, vX);

		SVector::Cross(vY, vZ, vX);

		vX.w = vY.w = vZ.w = 0.0f;
	}
}	// namespace izanagi
