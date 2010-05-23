#if !defined(__URANUS__MATH_QUATERNION_H__)
#define __URANUS__MATH_QUATERNION_H__

#include <math.h>
#include <memory.h>
#include "../unDefs.h"

namespace uranus {
	struct SVector;
	struct SMatrix;

	struct SQuat {
		union {
			UN_FLOAT v[4];
			struct {
				UN_FLOAT x, y, z, w;
			};
		};

		void Set(UN_FLOAT _x, UN_FLOAT _y, UN_FLOAT _z, UN_FLOAT _w)
		{
			x = _x; y = _y; z = _z; w = _w;
		}

		void Set(const SQuat& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}
	};

	// �N�I�[�^�j�I����ݒ肷��
	inline void SetQuat(SQuat& quat, UN_FLOAT x, UN_FLOAT y, UN_FLOAT z, UN_FLOAT w)
	{
		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;
	}

	// �p�x�Ǝ�����N�I�[�^�j�I����ݒ肷��
	extern void SetQuatFromRadAxis(SQuat& quat, UN_FLOAT rad, UN_FLOAT fAxisX, UN_FLOAT fAxisY, UN_FLOAT fAxisZ);
	extern void SetQuatFromRadAxis(SQuat& quat, UN_FLOAT rad, const SVector& vAxis);

	// �S�����O�̃N�I�[�^�j�I����ݒ肷��
	inline void SetZeroQuat(SQuat& quat)
	{
		quat.x = 0.0f;
		quat.y = 0.0f;
		quat.z = 0.0f;
		quat.w = 0.0f;
	}

	inline void SetQuatIdentity(SQuat& quat)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionIdentity(reinterpret_cast<D3DXQUATERNION*>(&quat));
#else	// #if defined(__USE_D3D_MATH__)
		quat.x = 0.0f;
		quat.y = 0.0f;
		quat.z = 0.0f;
		quat.w = 1.0f;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �N�I�[�^�j�I�����R�s�[
	inline void CopyQuat(SQuat& dst, const SQuat& src)
	{
		memcpy(&dst, &src, sizeof(SQuat));
	}

	// �Q�̃N�I�[�^�j�I�����X���b�v����B
	inline void SwapQuat(SQuat& q1, SQuat& q2)
	{
		SQuat q;
		CopyQuat(q, q1);
		CopyQuat(q1, q2);
		CopyQuat(q2, q);
	}

	// �N�I�[�^�j�I�������Z����
	// dst = src1 + src2
	inline void AddQuat(SQuat& dst, const SQuat& src1, const SQuat& src2)
	{
		dst.x = src1.x + src2.x;
		dst.y = src1.y + src2.y;
		dst.z = src1.z + src2.z;
		dst.w = src1.w + src2.w;
	}

	// �N�I�[�^�j�I�������Z����
	// dst = src1 - src2
	inline void SubQuat(SQuat& dst, const SQuat& src1, const SQuat& src2)
	{
		dst.x = src1.x - src2.x;
		dst.y = src1.y - src2.y;
		dst.z = src1.z - src2.z;
		dst.w = src1.w - src2.w;
	}

	// �N�I�[�^�j�I������Z����
	// dst = src1 * src2
	inline void MulQuat(SQuat& dst, const SQuat& src1, const SQuat& src2)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionMultiply(
			reinterpret_cast<D3DXQUATERNION*>(&dst),
			reinterpret_cast<const D3DXQUATERNION*>(&src1),
			reinterpret_cast<const D3DXQUATERNION*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = (src1.w * src2.x + src2.w * src1.x) + (src1.y * src2.z - src1.z * src2.y);
		dst.y = (src1.w * src2.y + src2.w * src1.y) + (src1.z * src2.x - src1.x * src2.z);
		dst.z = (src1.w * src2.z + src2.w * src1.z) + (src1.x * src2.y - src1.y * src2.x);

		dst.w = src1.w * src2.w - (src1.x * src2.x + src1.y * src2.y + src1.z * src2.z);
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �N�I�[�^�j�I�����X�P�[�����O����
	// dst = fQ * src
	inline void ScaleQuat(SQuat& dst, const SQuat& src, UN_FLOAT fQ)
	{
		dst.x = fQ * src.x;
		dst.y = fQ * src.y;
		dst.z = fQ * src.z;
		dst.w = fQ * src.w;
	}

	// �N�I�[�^�j�I�����X�P�[�����O�i���Z�j����
	// dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
	inline void DivQuat(SQuat& dst, const SQuat& src, UN_FLOAT fQ)
	{
#if 0
		dst.x = src.x / fQ;
		dst.y = src.y / fQ;
		dst.z = src.z / fQ;
		dst.w = src.w / fQ;
#else
		ScaleQuat(dst, src, 1.0f / fQ);
#endif
	}

	// �N�I�[�^�j�I���̑傫��(��Βl)���v�Z����
	inline UN_FLOAT LengthQuat(const SQuat& quat)
	{
#if defined(__USE_D3D_MATH__)
		UN_FLOAT ret = D3DXQuaternionLength(reinterpret_cast<const D3DXQUATERNION*>(&quat));
		return ret;
#else	// #if defined(__USE_D3D_MATH__)
		UN_FLOAT q = quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
		UN_FLOAT ret = sqrtf(q);
		return ret;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �N�I�[�^�j�I���𐳋K������
	inline void NormalizeQuat(SQuat& dst, const SQuat& src)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionNormalize(
			reinterpret_cast<D3DXQUATERNION*>(&dst),
			reinterpret_cast<const D3DXQUATERNION*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
		DivQuat(dst, src, LengthQuat(src));
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �����N�I�[�^�j�I�������߂�
	inline void ConjugateQuat(SQuat& dst, const SQuat& src)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionConjugate(
			reinterpret_cast<D3DXQUATERNION*>(&dst),
			reinterpret_cast<const D3DXQUATERNION*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
		dst.x = -src.x;
		dst.y = -src.y;
		dst.z = -src.z;
		dst.w = src.w;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// �t�N�I�[�^�j�I�������߂�
	inline void InverseQuat(SQuat& dst, const SQuat& src)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionInverse(
			reinterpret_cast<D3DXQUATERNION*>(&dst),
			reinterpret_cast<const D3DXQUATERNION*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
		// |q|^2
		UN_FLOAT s = src.x * src.x + src.y * src.y + src.z * src.z + src.w * src.w;

		// conjugate(q)
		ConjugateQuat(dst, src);

		// q^-1 = conjugate(q) / |q|^2
		DivQuat(dst, dst, s);
#endif	// #if defined(__USE_D3D_MATH__)
	}

#if 0
	// �x�N�g���ɃN�I�[�^�j�I����K�p����
	extern void ApplyQuat(SVector& dst, const SVector& src, const SQuat& q)
#endif

	// �N�I�[�^�j�I������s����v�Z����
	extern void MatrixFromQuat(SMatrix& mtx, const SQuat& quat);

	// �s�񂩂�N�I�[�^�j�I�����v�Z����
	extern void QuatFromMatrix(SQuat& quat, const SMatrix& mtx);

	// �I�C���[�p����N�I�[�^�j�I�����v�Z����
	extern void QuatFromEuler(SQuat& quat, UN_FLOAT fYaw, UN_FLOAT fPitch, UN_FLOAT fRoll);

	// ��̃x�N�g��v0,v1���^����ꂽ�Ƃ���
	// q  * v0 == v1 �ƂȂ�N�I�[�^�j�I��q���v�Z����
	extern void RotateionArc(SQuat& quat, const SVector& from, const SVector& to);
}	// namespace uranus

#endif	// #if !defined(__URANUS__MATH_QUATERNION_H__)
