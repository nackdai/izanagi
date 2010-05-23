#if !defined(__URANUS__MATH_MATRIX_H__)
#define __URANUS__MATH_MATRIX_H__

#include <math.h>
#include "../unDefs.h"
#include "MathDefs.h"
#include "MathVector.h"

namespace uranus {

	struct SMatrix {
		union {
			UN_FLOAT m[4][4];
			SVector v[4];
			struct {
				UN_FLOAT _00, _01, _02, _03;
				UN_FLOAT _10, _11, _12, _13;
				UN_FLOAT _20, _21, _22, _23;
				UN_FLOAT _30, _31, _32, _33;
			};
		};
	};

	// �}�g���N�X���R�s�[����
	inline void CopyMatrix(SMatrix& dst, const SMatrix& src)
	{
		memcpy(&dst, &src, sizeof(SMatrix));
	}

	// �P�ʃ}�g���N�X���Q�Ƃ���
	extern const SMatrix& GetUnitMatrix();

	// �P�ʃ}�g���N�X��ݒ肷��
	inline void SetUnitMatrix(SMatrix& mtx)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixIdentity(reinterpret_cast<D3DXMATRIX*>(&mtx));
#else	// #if defined(__USE_D3D_MATH__)
		CopyMatrix(mtx, GetUnitMatrix());
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �}�g���N�X���[���N���A����
	inline void SetZeroMatrix(SMatrix& mtx)
	{
		FILL_ZERO(&mtx, sizeof(mtx));
	}

	// �}�g���N�X��]�u����
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

	// �}�g���N�X����Z����
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

	// �}�g���N�X�ƃX�J���[����Z����
	inline void MulScalarMatrix(SMatrix& dst, const SMatrix& src, UN_FLOAT fScalar)
	{
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				dst.m[i][j] = src.m[i][j] * fScalar;
			}
		}
	}

	// �x�N�g���ƃ}�g���N�X����Z����
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

		CopyVector(dst, v);
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g���ƃ}�g���N�X�̂w�x�y�����݂̂���Z����
	// �x�N�g���̂v�����A����у}�g���N�X�̑�S�s�͖��������
	inline void ApplyMatrixXYZ(SVector& dst, const SVector& vec, const SMatrix& mtx)
	{
#if defined(__USE_D3D_MATH__)
		SVector v;
		CopyVector(v, vec);
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

	// X������]���ɂ��ĉ�]����}�g���N�X���擾
	extern void GetRotMatrixX(SMatrix& dst, UN_FLOAT fTheta);

	// Y������]���ɂ��ĉ�]����}�g���N�X���擾
	extern void GetRotMatrixY(SMatrix& dst, UN_FLOAT fTheta);

	// Z������]���ɂ��ĉ�]����}�g���N�X���擾
	extern void GetRotMatrixZ(SMatrix& dst, UN_FLOAT fTheta);

	// X���ɑ΂����]���s�Ȃ�
	inline void RotMatrixX(SMatrix& dst, const SMatrix& src, UN_FLOAT fTheta)
	{
		SMatrix m;
		GetRotMatrixX(m, fTheta);
		MulMatrix(dst, src, m);
	}

	// Y���ɑ΂����]���s�Ȃ�
	inline void RotMatrixY(SMatrix& dst, const SMatrix& src, UN_FLOAT fTheta)
	{
		SMatrix m;
		GetRotMatrixY(m, fTheta);
		MulMatrix(dst, src, m);
	}

	// Z���ɑ΂����]���s�Ȃ�
	inline void RotMatrixZ(SMatrix& dst, const SMatrix& src, UN_FLOAT fTheta)
	{
		SMatrix m;
		GetRotMatrixZ(m, fTheta);
		MulMatrix(dst, src, m);
	}

	// �I�t�Z�b�g���w�肵���}�g���N�X���擾
	extern void GetTransMatrix(SMatrix& dst, const SVector& tv);

	// ���s�ړ�����
	inline void TransMatrix(SMatrix& dst, const SMatrix& src, const SVector& tv)
	{
		SMatrix m;
		GetTransMatrix(m, tv);
		MulMatrix(dst, src, m);
	}

	// �w���x���y���̏��ŉ�]���s��
	inline void RotMatrixXYZ(SMatrix& dst, const SMatrix& src, const SVector& angle)
	{
		RotMatrixX(dst, src, angle.v[0]);
		RotMatrixY(dst, dst, angle.v[1]);
		RotMatrixZ(dst, dst, angle.v[2]);
	}

	// �X�P�[�����O�}�g���N�X�𓾂�
	inline void SetScaleMatrix(SMatrix& dst, UN_FLOAT fScaleX, UN_FLOAT fScaleY, UN_FLOAT fScaleZ)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixScaling(
			reinterpret_cast<D3DXMATRIX*>(&dst),
			fScaleX,
			fScaleY,
			fScaleZ);
#else	// #if defined(__USE_D3D_MATH__)
		SetVector(dst.v[0], fScaleX, 0.0f, 0.0f, 0.0f);
		SetVector(dst.v[1], 0.0f, fScaleY, 0.0f, 0.0f);
		SetVector(dst.v[2], 0.0f, 0.0f, fScaleZ, 0.0f);

		SetVector(dst.v[3], 0.0f, 0.0f, 0.0f, 1.0f);
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �X�P�[�����O�}�g���N�X�𓾂�
	inline void SetScaleMatrix(SMatrix& dst, const SVector& scale)
	{
		SetScaleMatrix(
			dst,
			scale.x, scale.y, scale.z);
	}

	// �X�P�[�����O����
	inline void ScaleMatrix(SMatrix& dst, const SMatrix& src, const SVector& scale)
	{
		SVector s;
		SetVector(s, scale.v[0], scale.v[1], scale.v[2], 1.0f);

		MulVector(dst.v[0], src.v[0], s);
		MulVector(dst.v[1], src.v[1], s);
		MulVector(dst.v[2], src.v[2], s);
		MulVector(dst.v[3], src.v[3], s);
	}

	// ��ʃ}�g���N�X�̋t�}�g���N�X�����߂�
	extern void InverseMatrix(SMatrix& dst, const SMatrix& src);

	// �x�N�g�������]�}�g���N�X�����߂�
	extern void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec);
	extern void GetRotMatrixFromVector(SMatrix& mtx, const SVector& vec, const SVector& up);
}	// namespace uranus

#endif	// #if !defined(__URANUS__MATH_MATRIX_H__)
