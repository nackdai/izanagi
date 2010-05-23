#if !defined(__URANUS__MATH_VECTOR_H__)
#define __URANUS__MATH_VECTOR_H__

#include <math.h>
#include <memory.h>
#include "../unDefs.h"
#include "MathUtil.h"

namespace uranus {

	struct SVector {
		union {
			UN_FLOAT v[4];
			struct {
				UN_FLOAT x, y, z, w;
			};
			struct {
				UN_FLOAT r, g, b, a;
			};
		};

		void Set(UN_FLOAT _x, UN_FLOAT _y, UN_FLOAT _z, UN_FLOAT _w = 1.0f)
		{
			x = _x; y = _y; z = _z; w = _w;
		}

		void Set(const SVector& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}
	};

	// �x�N�g����ݒ肷��
	inline void SetVector(SVector& vec, UN_FLOAT x, UN_FLOAT y, UN_FLOAT z, UN_FLOAT w)
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;
		vec.w = w;
	}

	// �S�����O�̃x�N�g����ݒ肷��
	inline void SetZeroVector(SVector& vec)
	{
		SetVector(vec, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	// �x�N�g�����R�s�[
	inline void CopyVector(SVector& dst, const SVector& src)
	{
		memcpy(&dst, &src, sizeof(SVector));
	}

	// �x�N�g�����R�s�[(XYZ�����̂�)
	inline void CopyVectorXYZ(SVector& dst, const SVector& src)
	{
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}

	// �Q�̃x�N�g�����X���b�v����B
	inline void SwapVector(SVector& v1, SVector& v2)
	{
		SVector v;
		CopyVector(v, v1);
		CopyVector(v1, v2);
		CopyVector(v2, v);
	}

	// �x�N�g�������Z����
	// dst = src1 + src2
	inline void AddVector(SVector& dst, const SVector& src1, const SVector& src2)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec4Add(
			reinterpret_cast<D3DXVECTOR4*>(&dst),
			reinterpret_cast<const D3DXVECTOR4*>(&src1),
			reinterpret_cast<const D3DXVECTOR4*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = src1.x + src2.x;
		dst.y = src1.y + src2.y;
		dst.z = src1.z + src2.z;
		dst.w = src1.w + src2.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g�������Z����(XYZ�����̂�)
	inline void AddVectorXYZ(SVector& dst, const SVector& src1, const SVector& src2)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec3Add(
			reinterpret_cast<D3DXVECTOR3*>(&dst),
			reinterpret_cast<const D3DXVECTOR3*>(&src1),
			reinterpret_cast<const D3DXVECTOR3*>(&src2));
		dst.w = src1.w;
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = src1.x + src2.x;
		dst.y = src1.y + src2.y;
		dst.z = src1.z + src2.z;
		dst.w = src1.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g�������Z����
	// dst = src1 - src2
	inline void SubVector(SVector& dst, const SVector& src1, const SVector& src2)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec4Subtract(
			reinterpret_cast<D3DXVECTOR4*>(&dst),
			reinterpret_cast<const D3DXVECTOR4*>(&src1),
			reinterpret_cast<const D3DXVECTOR4*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = src1.x - src2.x;
		dst.y = src1.y - src2.y;
		dst.z = src1.z - src2.z;
		dst.w = src1.w - src2.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g�������Z����(XYZ�����̂�)
	inline void SubVectorXYZ(SVector& dst, const SVector& src1, const SVector& src2)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec3Subtract(
			reinterpret_cast<D3DXVECTOR3*>(&dst),
			reinterpret_cast<const D3DXVECTOR3*>(&src1),
			reinterpret_cast<const D3DXVECTOR3*>(&src2));
		dst.w = src1.w;
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = src1.x - src2.x;
		dst.y = src1.y - src2.y;
		dst.z = src1.z - src2.z;
		dst.w = src1.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g������Z����
	// dst = src1 * src2
	inline void MulVector(SVector& dst, const SVector& src1, const SVector& src2)
	{
		dst.x = src1.x * src2.x;
		dst.y = src1.y * src2.y;
		dst.z = src1.z * src2.z;
		dst.w = src1.w * src2.w;
	}

	// �x�N�g�����X�P�[�����O����
	// dst = fQ * src
	inline void ScaleVector(SVector& dst, const SVector& src, UN_FLOAT fQ)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec4Scale(
			reinterpret_cast<D3DXVECTOR4*>(&dst),
			reinterpret_cast<const D3DXVECTOR4*>(&src),
			fQ);
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = fQ * src.x;
		dst.y = fQ * src.y;
		dst.z = fQ * src.z;
		dst.w = fQ * src.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g���̂w�x�y�����݂̂��X�P�[�����O����
	// �v�����͂��̂܂�
	// dst = (src.x x fQ, src.y x fQ, src.z x fQ, src.w)
	inline void ScaleVectorXYZ(SVector& dst, const SVector& src, UN_FLOAT fQ)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec3Scale(
			reinterpret_cast<D3DXVECTOR3*>(&dst),
			reinterpret_cast<const D3DXVECTOR3*>(&src),
			fQ);
		dst.w = src.w;
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = fQ * src.x;
		dst.y = fQ * src.y;
		dst.z = fQ * src.z;
		dst.w = src.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g�����X�P�[�����O�i���Z�j����
	// dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
	inline void DivVector(SVector& dst, const SVector& src, UN_FLOAT fQ)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec4Scale(
			reinterpret_cast<D3DXVECTOR4*>(&dst),
			reinterpret_cast<const D3DXVECTOR4*>(&src),
			1.0f / fQ);
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = src.x / fQ;
		dst.y = src.y / fQ;
		dst.z = src.z / fQ;
		dst.w = src.w / fQ;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g���̂w�x�y�����݂̂��X�P�[�����O(���Z)����
	// �v�����͂��̂܂�
	// dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w)
	inline void DivVectorXYZ(SVector& dst, const SVector& src, UN_FLOAT fQ)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec3Scale(
			reinterpret_cast<D3DXVECTOR3*>(&dst),
			reinterpret_cast<const D3DXVECTOR3*>(&src),
			1.0f / fQ);
		dst.w = src.w;
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = src.x / fQ;
		dst.y = src.y / fQ;
		dst.z = src.z / fQ;
		dst.w = src.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g���̓��ς��v�Z����
	inline UN_FLOAT DotVector(const SVector& vec1, const SVector& vec2)
	{
#if defined(__USE_D3D_MATH__)
		UN_FLOAT ret = D3DXVec3Dot(
						reinterpret_cast<const D3DXVECTOR3*>(&vec1),
						reinterpret_cast<const D3DXVECTOR3*>(&vec2));
		return ret;
#else	// #if defined(__USE_D3D_MATH__)
		UN_FLOAT ret = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
		return ret;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g���̊O�ς��v�Z����
	inline void CrossVector(SVector& dst, const SVector& src1, const SVector& src2)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec3Cross(
			reinterpret_cast<D3DXVECTOR3*>(&dst),
			reinterpret_cast<const D3DXVECTOR3*>(&src1),
			reinterpret_cast<const D3DXVECTOR3*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = src1.y * src2.z - src1.z * src2.y;
		dst.y = src1.z * src2.x - src1.x * src2.z;
		dst.z = src1.x * src2.y - src1.y * src2.x;
		dst.w = src1.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �x�N�g���̑傫��(��Βl)���v�Z����
	inline UN_FLOAT LengthVector(const SVector& vec)
	{
#if defined(__USE_D3D_MATH__)
		UN_FLOAT ret = D3DXVec3Length(reinterpret_cast<const D3DXVECTOR3*>(&vec));
		return ret;
#else	// #if defined(__USE_D3D_MATH__)
		UN_FLOAT ret = sqrtf(DotVector(vec, vec));
		return ret;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �ʒu�x�N�g���Ԃ̋������v�Z����
	inline UN_FLOAT LengthVector2(const SVector& vec1, const SVector& vec2)
	{
		SVector v;
		SubVector(v, vec1, vec2);
		return LengthVector(v);
	}

	// �x�N�g���𐳋K������
	inline void NormalizeVector(SVector& dst, const SVector& src)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec3Normalize(
			reinterpret_cast<D3DXVECTOR3*>(&dst),
			reinterpret_cast<const D3DXVECTOR3*>(&src));
		dst.w = 0.0f;
#else	// #if defined(__USE_D3D_MATH__)
		DivVectorXYZ(dst, src, LengthVector(src));
		dst.w = 0.0f;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �w���ɑ΂��ăx�N�g������]����
	inline void RotVectorX(SVector& dst, const SVector& src, UN_FLOAT fTheta)
	{
		UN_FLOAT c, s;
		CMath::GetSinCosF(fTheta, &s, &c);

		SetVector(
			dst,
			src.v[0],
			src.v[1] * c - src.v[2] * s,
			src.v[1] * s + src.v[2] * c,
			src.v[3]);
	}

	// �x���ɑ΂��ăx�N�g������]����
	inline void RotVectorY(SVector& dst, const SVector& src, UN_FLOAT fTheta)
	{
		UN_FLOAT c, s;
		CMath::GetSinCosF(fTheta, &s, &c);

		SetVector(
			dst,
			src.v[0] * c + src.v[2] * s,
			src.v[1],
			src.v[2] * c - src.v[0] * s,
			src.v[3]);
	}

	// �y���ɑ΂��ăx�N�g������]����
	inline void RotVectorZ(SVector& dst, const SVector& src, UN_FLOAT fTheta)
	{
		UN_FLOAT c, s;
		CMath::GetSinCosF(fTheta, &s, &c);

		SetVector(
			dst,
			src.v[0] * c - src.v[1] * s,
			src.v[0] * s + src.v[1] * c,
			src.v[2],
			src.v[3]);	
	}

	// �x�N�g���̐��`���
	inline void LerpVector(SVector& dst, const SVector& src0, const SVector& src1, UN_FLOAT f)
	{
#if defined(__USE_D3D_MATH__)
		D3DXVec4Lerp(
			reinterpret_cast<D3DXVECTOR4*>(&dst),
			reinterpret_cast<const D3DXVECTOR4*>(&src0),
			reinterpret_cast<const D3DXVECTOR4*>(&src1),
			f);
#else	// #if defined(__USE_D3D_MATH__)
		UN_FLOAT ff = 1.0f - f;
		dst.x = ff * src0.x + f * src1.x;
		dst.y = ff * src0.y + f * src1.y;
		dst.z = ff * src0.z + f * src1.z;
		dst.w = ff * src0.w + f * src1.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS__MATH_VECTOR_H__)
