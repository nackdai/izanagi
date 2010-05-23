#include "math/MathQuaternion.h"
#include "math/MathVector.h"
#include "math/MathMatrix.h"

namespace uranus {
	// 角度と軸からクオータニオンを設定する
	void SetQuatFromRadAxis(SQuat& quat, UN_FLOAT rad, UN_FLOAT fAxisX, UN_FLOAT fAxisY, UN_FLOAT fAxisZ)
	{
		SVector vAxis;
		vAxis.Set(fAxisX, fAxisY, fAxisZ, 0.0f);

		SetQuatFromRadAxis(quat, rad, vAxis);
	}

	void SetQuatFromRadAxis(SQuat& quat, UN_FLOAT rad, const SVector& vAxis)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionRotationAxis(
			reinterpret_cast<D3DXQUATERNION*>(&quat),
			reinterpret_cast<const D3DXVECTOR3*>(&vAxis),
			rad);
#else	// #if defined(__USE_D3D_MATH__)
		// 念のため
		SVector v;
		NormalizeVector(v, vAxis);

		UN_FLOAT c = cosf(rad * 0.5f);
		UN_FLOAT s = sinf(rad * 0.5f);

		quat.x = s * v.x;
		quat.y = s * v.y;
		quat.z = s * v.z;
		quat.w = c;
#endif	// #if defined(__USE_D3D_MATH__)
	}

#if 0
	// ベクトルにクオータニオンを適用する
	void ApplyQuat(SVector& dst, const SVector& src, const SQuat& q)
	{
		dst.Set(src.x, src.y, src.z, 0.0f);

		SVector v;
		v.Set(q.x, q.y, q.z, 0.0f);

		UN_FLOAT ww = q.w * q.w;
		UN_FLOAT vv = InnerProduct(v, v);
		UN_FLOAT vd = InnerProduct(v, dst);
		
		SVector cross;
		OuterProduct(cross, v, dst);

		ScaleVector(dst, dst, ww - vv);
		ScaleVector(v, v, 2.0f * vd);
		ScaleVector(cross, cross, 2.0f * q.w);

		AddVector(dst, dst, v);
		AddVector(dst, dst, cross);
	}
#endif

	// クオータニオンから行列を計算する
	void MatrixFromQuat(SMatrix& mtx, const SQuat& quat)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixRotationQuaternion(
			reinterpret_cast<D3DXMATRIX*>(&mtx),
			reinterpret_cast<const D3DXQUATERNION*>(&quat));
#else	// #if defined(__USE_D3D_MATH__)
		const UN_FLOAT& x = quat.x;
		const UN_FLOAT& y = quat.y;
		const UN_FLOAT& z = quat.z;
		const UN_FLOAT& w = quat.w;

#if 0
		// ベタに計算するとこうなる
		const UN_FLOAT xx2 = 2.0f * x * x;
		const UN_FLOAT yy2 = 2.0f * y * y;
		const UN_FLOAT zz2 = 2.0f * z * z;
		const UN_FLOAT xy2 = 2.0f * x * y;
		const UN_FLOAT xz2 = 2.0f * x * z;
		const UN_FLOAT wz2 = 2.0f * w * z;
		const UN_FLOAT wy2 = 2.0f * w * y;
		const UN_FLOAT yz2 = 2.0f * y * z;
		const UN_FLOAT wx2 = 2.0f * w * x;

		mtx.m[0][0] = 1.0f - yy2 - zz2;
		mtx.m[0][1] = xy2 + wz2;
		mtx.m[0][2] = xz2 - wy2;
		mtx.m[0][3] = 0.0f;

		mtx.m[1][0] = xy2 - wz2;
		mtx.m[1][1] = 1.0f - xx2 - zz2;
		mtx.m[1][2] = yz2 + wx2;
		mtx.m[1][3] = 0.0f;

		mtx.m[2][0] = xz2 + wy2;
		mtx.m[2][1] = yz2 - wx2;
		mtx.m[2][2] = 1.0f - xx2 - yy2;
		mtx.m[2][3] = 0.0f;
#else
		// マトリクスの乗算に直すとこうなる
		SMatrix m1, m2;
		
		m1.m[0][0] =  w; m1.m[0][1] =  x; m1.m[0][2] = -y; m1.m[0][3] =  z;
		m1.m[1][0] = -z; m1.m[1][1] =  y; m1.m[1][2] =  x; m1.m[1][3] =  w;
		m1.m[2][0] =  y; m1.m[2][1] =  z; m1.m[2][2] =  w; m1.m[2][3] = -x;
		m1.m[3][0] =     m1.m[3][1] =     m1.m[3][2] =     m1.m[3][3] = 0.0f;
		
		m2.m[0][0] =  w; m2.m[0][1] =  z; m2.m[0][2] = -y; m2.m[0][3] = 0.0f;
		m2.m[1][0] =  x; m2.m[1][1] =  y; m2.m[1][2] =  z; m2.m[1][3] = 0.0f;
		m2.m[2][0] =  y; m2.m[2][1] = -x; m2.m[2][2] =  w; m2.m[2][3] = 0.0f;
		m2.m[3][0] = -z; m2.m[3][1] =  w; m2.m[3][2] =  x; m2.m[3][3] = 0.0f;

		MulMatrix(mtx, m1, m2);
#endif
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// 行列からクオータニオンを計算する
	void QuatFromMatrix(SQuat& quat, const SMatrix& mtx)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionRotationMatrix(
			reinterpret_cast<D3DXQUATERNION*>(&quat),
			reinterpret_cast<const D3DXMATRIX*>(&mtx));
#else	// #if defined(__USE_D3D_MATH__)
		UN_FLOAT ww = 0.25f * (1.0f + mtx.m[0][0] + mtx.m[1][1] + mtx.m[2][2]);

		if (ww > 0.0f) {
			quat.w = sqrtf(ww);

			UN_FLOAT div = 1.0f / (4.0f * quat.w);
			quat.x = (mtx.m[1][2] - mtx.m[2][1]) * div;
			quat.y = (mtx.m[2][0] - mtx.m[0][2]) * div;
			quat.z = (mtx.m[0][1] - mtx.m[1][0]) * div;
		}
		else {
			quat.w = 0.0f;

			UN_FLOAT xx = -0.5f * (mtx.m[1][1] + mtx.m[2][2]);

			if (xx > 0.0f) {
				quat.x = sqrtf(xx);

				UN_FLOAT div = 1.0f / (2.0f * quat.x);
				quat.y = mtx.m[0][1] * div;
				quat.z = mtx.m[0][2] * div;
			}
			else {
				quat.x = 0.0f;

				UN_FLOAT yy = 0.5f * (1.0f - mtx.m[2][2]);

				if (yy > 0.0f) {
					quat.y = sqrtf(yy);
					quat.z = mtx.m[1][2] / (2.0f * quat.y);
				}
				else {
					quat.y = 0.0f;
					quat.z = 1.0f;
				}
			}
		}
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// オイラー角からクオータニオンを計算する
	void QuatFromEuler(SQuat& quat, UN_FLOAT fYaw, UN_FLOAT fPitch, UN_FLOAT fRoll)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionRotationYawPitchRoll(
			reinterpret_cast<D3DXQUATERNION*>(&quat),
			fYaw,
			fPitch,
			fRoll);
#else	// #if defined(__USE_D3D_MATH__)
		// NOTE
		// XYZ軸で回転するクオータニオンを順に乗算する

		UN_FLOAT cosY = cosf(fYaw * 0.5f);
		UN_FLOAT sinY = sinf(fYaw * 0.5f);

		UN_FLOAT cosP = cosf(fPitch * 0.5f);
		UN_FLOAT sinP = sinf(fPitch * 0.5f);

		UN_FLOAT cosR = cosf(fRoll * 0.5f);
		UN_FLOAT sinR = sinf(fRoll * 0.5f);

		quat.Set(
			cosR * sinP * cosY + sinR * cosP * sinY,
			cosR * cosP * sinY - sinR * sinP * cosY,
			sinR * cosP * cosY - cosR * sinP * sinY,
			cosR * cosP * cosY + sinR * sinP * sinY);
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// 二つのベクトルv0,v1が与えられたときに
	// q  * v0 == v1 となるクオータニオンqを計算する
	void RotateionArc(SQuat& quat, const SVector& from, const SVector& to)
	{
		// 念のため
		SVector v0, v1;
		NormalizeVector(v0, from);
		NormalizeVector(v1, to);

		SVector c;
		CrossVector(c, v0, v1);

		UN_FLOAT d = DotVector(v0, v1);
		UN_FLOAT s = sqrtf(2.0f * (1.0f + d));

		quat.x = c.x / s;
		quat.y = c.y / s;
		quat.z = c.z / s;

		quat.w = s * 0.5f;
	}
}	// namespace uranus
