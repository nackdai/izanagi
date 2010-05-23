#include "math/MathMatrix.h"

namespace uranus {
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

	// X軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrixX(SMatrix& dst, UN_FLOAT fTheta)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixRotationX(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			fTheta);
#else	// #if defined(__USE_D3D_MATH__)
		const UN_FLOAT c = cosf(fTheta);
		const UN_FLOAT s = sinf(fTheta);

		dst.m[0][0] = 1.0f; dst.m[0][1] = 0.0f; dst.m[0][2] = 0.0f; dst.m[0][3] = 0.0f;
		dst.m[1][0] = 0.0f; dst.m[1][1] = c;    dst.m[1][2] = s;    dst.m[1][3] = 0.0f;
		dst.m[2][0] = 0.0f; dst.m[2][1] = -s;   dst.m[2][2] = c;    dst.m[2][3] = 0.0f;
		dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// Y軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrixY(SMatrix& dst, UN_FLOAT fTheta)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixRotationY(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			fTheta);
#else	// #if defined(__USE_D3D_MATH__)
		const UN_FLOAT c = cosf(fTheta);
		const UN_FLOAT s = sinf(fTheta);

		dst.m[0][0] = c;    dst.m[0][1] = 0.0f; dst.m[0][2] = -s;   dst.m[0][3] = 0.0f;
		dst.m[1][0] = 0.0f; dst.m[1][1] = 1.0f; dst.m[1][2] = 0.0f; dst.m[1][3] = 0.0f;
		dst.m[2][0] = s;    dst.m[2][1] = 0.0f; dst.m[2][2] = c;    dst.m[2][3] = 0.0f;
		dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// Z軸を回転軸にして回転するマトリクスを取得
	void GetRotMatrixZ(SMatrix& dst, UN_FLOAT fTheta)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixRotationZ(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			fTheta);
#else	// #if defined(__USE_D3D_MATH__)
		const UN_FLOAT c = cosf(fTheta);
		const UN_FLOAT s = sinf(fTheta);

		dst.m[0][0] = c;    dst.m[0][1] = s;    dst.m[0][2] = 0.0f; dst.m[0][3] = 0.0f;
		dst.m[1][0] = -s;   dst.m[1][1] = c;    dst.m[1][2] = 0.0f; dst.m[1][3] = 0.0f;
		dst.m[2][0] = 0.0f; dst.m[2][1] = 0.0f; dst.m[2][2] = 1.0f; dst.m[2][3] = 0.0f;
		dst.m[3][0] = 0.0f; dst.m[3][1] = 0.0f; dst.m[3][2] = 0.0f; dst.m[3][3] = 1.0f;
#endif	// #if defined(__USE_D3D_MATH__)
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

	// 逆マトリクスを Gauss/Jordan 法で求める
	void InverseMatrix(SMatrix& dst, const SMatrix& src)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixInverse(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			UN_NULL,	// 行列式はいらない
			reinterpret_cast<const D3DXMATRIX*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
		// Gauss/Jordan法で求める
		SMatrix mtx;
		CopyMatrix(mtx, src);
		SetUnitMatrix(dst);

		for (int i = 0; i < 4; ++i) {
			// ピボット選択
			// NOTE: 対象となる列中の最大値が対角値になるように行を入れ替える
			UN_FLOAT f = Absf(mtx.m[i][i]);
			for (int j = i + 1; j < 4; ++j) {
				if (f < Absf(mtx.m[j][i])) {
					f = Absf(mtx.m[j][i]);
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
					UN_FLOAT fTemp = mtx.m[j][i];

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

		NormalizeVector(vZ, vec);

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
			CopyVector(vUp, up);
		}

		CrossVector(vX, vUp, vZ);
		NormalizeVector(vX, vX);

		CrossVector(vY, vZ, vX);

		vX.w = vY.w = vZ.w = 0.0f;
	}
}	// namespace uranus
