#include "math/MathMatrix.h"

namespace uranus {
	static SMatrix mtxUnit = {
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	// �P�ʃ}�g���N�X���Q�Ƃ���
	const SMatrix& GetUnitMatrix()
	{
		return mtxUnit;
	}

	// X������]���ɂ��ĉ�]����}�g���N�X���擾
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

	// Y������]���ɂ��ĉ�]����}�g���N�X���擾
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

	// Z������]���ɂ��ĉ�]����}�g���N�X���擾
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

	// �I�t�Z�b�g���w�肵���}�g���N�X���擾
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

	// �t�}�g���N�X�� Gauss/Jordan �@�ŋ��߂�
	void InverseMatrix(SMatrix& dst, const SMatrix& src)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixInverse(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			UN_NULL,	// �s�񎮂͂���Ȃ�
			reinterpret_cast<const D3DXMATRIX*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
		// Gauss/Jordan�@�ŋ��߂�
		SMatrix mtx;
		CopyMatrix(mtx, src);
		SetUnitMatrix(dst);

		for (int i = 0; i < 4; ++i) {
			// �s�{�b�g�I��
			// NOTE: �ΏۂƂȂ�񒆂̍ő�l���Ίp�l�ɂȂ�悤�ɍs�����ւ���
			UN_FLOAT f = Absf(mtx.m[i][i]);
			for (int j = i + 1; j < 4; ++j) {
				if (f < Absf(mtx.m[j][i])) {
					f = Absf(mtx.m[j][i]);
					SwapVector(mtx.v[i], mtx.v[j]);
					SwapVector(dst.v[i], dst.v[j]);
				}
			}

			// �ΏۂƂȂ�s�̑Ίp�l�� 1 �ɂ���
			f = 1.0f / mtx.m[i][i];
			ScaleVector(mtx.v[i], mtx.v[i], f);
			ScaleVector(dst.v[i], dst.v[i], f);

			// �ΏۂƂȂ�Ȃ���̒l�� 0 �ɂ���
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

	// �x�N�g�������]�}�g���N�X�����߂�
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
			// UP�x�N�g���Ƃ̊O�ς��v�Z�ł��Ȃ��̂ŁA
			// �V����UP�x�N�g�����ł���������E�E�E
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
