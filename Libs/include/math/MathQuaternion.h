#if !defined(__IZANAGI__MATH_QUATERNION_H__)
#define __IZANAGI__MATH_QUATERNION_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"
#include "MathVector.h"

namespace izanagi {
	struct SMatrix;

	struct SQuat : public SVector {
		void Set(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w = 1.0f)
		{
			x = _x; y = _y; z = _z; w = _w;
		}

		void Set(const SVector& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}

		// クオータニオンを設定する
		static void Set(SQuat& quat, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT w)
		{
			quat.x = x;
			quat.y = y;
			quat.z = z;
			quat.w = w;
		}

		// 全成分０のクオータニオンを設定する
		static void SetZero(SQuat& quat)
		{
			quat.x = 0.0f;
			quat.y = 0.0f;
			quat.z = 0.0f;
			quat.w = 0.0f;
		}

		static void SetIdentity(SQuat& quat)
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
		static void Copy(SQuat& dst, const SQuat& src)
		{
			dst.x = src.x;
			dst.y = src.y;
			dst.z = src.z;
			dst.w = src.w;
		}

		static void Copy(SVector& dst, const SQuat& src)
		{
			dst.x = src.x;
			dst.y = src.y;
			dst.z = src.z;
			dst.w = src.w;
		}

		// ２つのクオータニオンをスワップする。
		static void Swap(SQuat& q1, SQuat& q2)
		{
			SQuat q;
			Copy(q, q1);
			Copy(q1, q2);
			Copy(q2, q);
		}

		// クオータニオンを加算する
		// dst = src1 + src2
		static void Add(SQuat& dst, const SQuat& src1, const SQuat& src2)
		{
			dst.x = src1.x + src2.x;
			dst.y = src1.y + src2.y;
			dst.z = src1.z + src2.z;
			dst.w = src1.w + src2.w;
		}

		// クオータニオンを減算する
		// dst = src1 - src2
		static void Sub(SQuat& dst, const SQuat& src1, const SQuat& src2)
		{
			dst.x = src1.x - src2.x;
			dst.y = src1.y - src2.y;
			dst.z = src1.z - src2.z;
			dst.w = src1.w - src2.w;
		}

		// クオータニオンを乗算する
		// dst = src1 * src2
		static void Mul(SQuat& dst, const SQuat& src1, const SQuat& src2)
		{
#if defined(__USE_D3D_MATH__)
			D3DXQuaternionMultiply(
				reinterpret_cast<D3DXQUATERNION*>(&dst),
				reinterpret_cast<const D3DXQUATERNION*>(&src1),
				reinterpret_cast<const D3DXQUATERNION*>(&src2));
#else	// #if defined(__USE_D3D_MATH__)
			dst.x = src1.w * src2.x + src2.w * src1.x + src1.y * src2.z - src1.z * src2.y;
			dst.y = src1.w * src2.y + src2.w * src1.y + src1.z * src2.x - src1.x * src2.z;
			dst.z = src1.w * src2.z + src2.w * src1.z + src1.x * src2.y - src1.y * src2.x;

			dst.w = src1.w * src2.w - src1.x * src2.x - src1.y * src2.y - src1.z * src2.z;
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// クオータニオンをスケーリングする
		// dst = fQ * src
		static void Scale(SQuat& dst, const SQuat& src, IZ_FLOAT fQ)
		{
			dst.x = fQ * src.x;
			dst.y = fQ * src.y;
			dst.z = fQ * src.z;
			dst.w = fQ * src.w;
		}

		// クオータニオンをスケーリング（除算）する
		// dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
		static void Div(SQuat& dst, const SQuat& src, IZ_FLOAT fQ)
		{
#if 0
			dst.x = src.x / fQ;
			dst.y = src.y / fQ;
			dst.z = src.z / fQ;
			dst.w = src.w / fQ;
#else
			Scale(dst, src, 1.0f / fQ);
#endif
		}

		// クオータニオンの大きさ(絶対値)を計算する
		static IZ_FLOAT Length(const SQuat& quat)
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
		static void Normalize(SQuat& dst, const SQuat& src)
		{
#if defined(__USE_D3D_MATH__)
			D3DXQuaternionNormalize(
				reinterpret_cast<D3DXQUATERNION*>(&dst),
				reinterpret_cast<const D3DXQUATERNION*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
			Div(dst, src, Length(src));
#endif	// #if defined(__USE_D3D_MATH__)
		}

		// 共役クオータニオンを求める
		static void Conjugate(SQuat& dst, const SQuat& src)
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
		static void Inverse(SQuat& dst, const SQuat& src)
		{
#if defined(__USE_D3D_MATH__)
			D3DXQuaternionInverse(
				reinterpret_cast<D3DXQUATERNION*>(&dst),
				reinterpret_cast<const D3DXQUATERNION*>(&src));
#else	// #if defined(__USE_D3D_MATH__)
			// |q|^2
			IZ_FLOAT s = src.x * src.x + src.y * src.y + src.z * src.z + src.w * src.w;

			// conjugate(q)
			Conjugate(dst, src);

			// q^-1 = conjugate(q) / |q|^2
			Div(dst, dst, s);
#endif	// #if defined(__USE_D3D_MATH__)
		}

#if 0
		// ベクトルにクオータニオンを適用する
		void Apply(SVector& dst, const SVector& src, const SQuat& q);
#endif

		// 球面線形補間
		static void Slerp(SQuat& dst, const SQuat& quat1, const SQuat& quat2, IZ_FLOAT t)
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

		static void Slerp(SVector& dst, const SQuat& quat1, const SQuat& quat2, IZ_FLOAT t)
		{
			Slerp(
				*(reinterpret_cast<SQuat*>(&dst)),
				quat1, quat2,
				t);
		}

		// 角度と軸からクオータニオンを設定する
		static void SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, IZ_FLOAT fAxisX, IZ_FLOAT fAxisY, IZ_FLOAT fAxisZ);
		static void SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, const SVector& vAxis);

		// クオータニオンから行列を計算する
		static void MatrixFromQuat(SMatrix& mtx, const SQuat& quat);

		// 行列からクオータニオンを計算する
		static void QuatFromMatrix(SQuat& quat, const SMatrix& mtx);

		// オイラー角からクオータニオンを計算する
		static void QuatFromEuler(SQuat& quat, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z);

		// 二つのベクトルv0,v1が与えられたときに
		// q  * v0 == v1 となるクオータニオンqを計算する
		static void RotationArc(SQuat& quat, const SVector& from, const SVector& to);

		// クオータニオンからオイラー角を計算する
		static void GetEuler(SVector& angle, const SQuat& quat);

		// オイラー角からクオータニオンを計算する
		static void QuatFromEuler(SQuat& quat, const SVector& angle);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_QUATERNION_H__)
