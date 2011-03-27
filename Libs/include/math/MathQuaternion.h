#if !defined(__IZANAGI__MATH_QUATERNION_H__)
#define __IZANAGI__MATH_QUATERNION_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"

namespace izanagi {
#if 0
	struct SVector;
	struct SMatrix;

	struct SQuat {
		union {
			IZ_FLOAT v[4];
			struct {
				IZ_FLOAT x, y, z, w;
			};
		};

		void Set(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w)
		{
			x = _x; y = _y; z = _z; w = _w;
		}

		void Set(const SQuat& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}
	};
#else
}	// namepsace izanagi

#include "MathVector.h"

namespace izanagi {
	struct SMatrix;

	#define SQuat SVector
#endif

	// クオータニオンを設定する
	inline void SetQuat(SQuat& quat, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT w)
	{
		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;
	}

	// 全成分０のクオータニオンを設定する
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

	// クオータニオンをコピー
	inline void CopyQuat(SQuat& dst, const SQuat& src)
	{
		memcpy(&dst, &src, sizeof(SQuat));
	}

	// ２つのクオータニオンをスワップする。
	inline void SwapQuat(SQuat& q1, SQuat& q2)
	{
		SQuat q;
		CopyQuat(q, q1);
		CopyQuat(q1, q2);
		CopyQuat(q2, q);
	}

	// クオータニオンを加算する
	// dst = src1 + src2
	inline void AddQuat(SQuat& dst, const SQuat& src1, const SQuat& src2)
	{
		dst.x = src1.x + src2.x;
		dst.y = src1.y + src2.y;
		dst.z = src1.z + src2.z;
		dst.w = src1.w + src2.w;
	}

	// クオータニオンを減算する
	// dst = src1 - src2
	inline void SubQuat(SQuat& dst, const SQuat& src1, const SQuat& src2)
	{
		dst.x = src1.x - src2.x;
		dst.y = src1.y - src2.y;
		dst.z = src1.z - src2.z;
		dst.w = src1.w - src2.w;
	}

	// クオータニオンを乗算する
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

	// クオータニオンをスケーリングする
	// dst = fQ * src
	inline void ScaleQuat(SQuat& dst, const SQuat& src, IZ_FLOAT fQ)
	{
		dst.x = fQ * src.x;
		dst.y = fQ * src.y;
		dst.z = fQ * src.z;
		dst.w = fQ * src.w;
	}

	// クオータニオンをスケーリング（除算）する
	// dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
	inline void DivQuat(SQuat& dst, const SQuat& src, IZ_FLOAT fQ)
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

	// クオータニオンの大きさ(絶対値)を計算する
	inline IZ_FLOAT LengthQuat(const SQuat& quat)
	{
#if defined(__USE_D3D_MATH__)
		IZ_FLOAT ret = D3DXQuaternionLength(reinterpret_cast<const D3DXQUATERNION*>(&quat));
		return ret;
#else	// #if defined(__USE_D3D_MATH__)
		IZ_FLOAT q = quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
		IZ_FLOAT ret = sqrtf(q);
		return ret;
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// クオータニオンを正規化する
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

	// 共役クオータニオンを求める
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

	// 逆クオータニオンを求める
	inline void InverseQuat(SQuat& dst, const SQuat& src)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionInverse(
			reinterpret_cast<D3DXQUATERNION*>(&dst),
			reinterpret_cast<const D3DXQUATERNION*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
		// |q|^2
		IZ_FLOAT s = src.x * src.x + src.y * src.y + src.z * src.z + src.w * src.w;

		// conjugate(q)
		ConjugateQuat(dst, src);

		// q^-1 = conjugate(q) / |q|^2
		DivQuat(dst, dst, s);
#endif	// #if defined(__USE_D3D_MATH__)
	}

#if 0
	// ベクトルにクオータニオンを適用する
	extern void ApplyQuat(SVector& dst, const SVector& src, const SQuat& q);
#endif

	// 球面線形補間
	inline void SlerpQuat(SQuat& dst, const SQuat& quat1, const SQuat& quat2, IZ_FLOAT t)
	{
		// 念のため
		t = CLAMP(t, 0.0f, 1.0f);

#if defined(__USE_D3D_MATH__)
		D3DXQuaternionSlerp(
			reinterpret_cast<D3DXQUATERNION*>(&dst),
			reinterpret_cast<const D3DXQUATERNION*>(&quat1),
			reinterpret_cast<const D3DXQUATERNION*>(&quat2),
			t);
#else	// #if defined(__USE_D3D_MATH__)
		// TODO
		IZ_ASSERT(IZ_FALSE);
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// 角度と軸からクオータニオンを設定する
	extern void SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, IZ_FLOAT fAxisX, IZ_FLOAT fAxisY, IZ_FLOAT fAxisZ);
	extern void SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, const SVector& vAxis);

	// クオータニオンから行列を計算する
	extern void MatrixFromQuat(SMatrix& mtx, const SQuat& quat);

	// 行列からクオータニオンを計算する
	extern void QuatFromMatrix(SQuat& quat, const SMatrix& mtx);

	// オイラー角からクオータニオンを計算する
	extern void QuatFromEuler(SQuat& quat, IZ_FLOAT fYaw, IZ_FLOAT fPitch, IZ_FLOAT fRoll);

	// 二つのベクトルv0,v1が与えられたときに
	// q  * v0 == v1 となるクオータニオンqを計算する
	extern void RotateionArc(SQuat& quat, const SVector& from, const SVector& to);
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_QUATERNION_H__)
