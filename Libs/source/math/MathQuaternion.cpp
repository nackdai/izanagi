#include "math/MathQuaternion.h"
#include "math/MathVector.h"
#include "math/MathMatrix.h"

namespace izanagi {
	// 角度と軸からクオータニオンを設定する
	void SQuat::SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, IZ_FLOAT fAxisX, IZ_FLOAT fAxisY, IZ_FLOAT fAxisZ)
	{
		SVector vAxis;
		vAxis.Set(fAxisX, fAxisY, fAxisZ, 0.0f);

		SetQuatFromRadAxis(quat, rad, vAxis);
	}

	void SQuat::SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, const SVector& vAxis)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionRotationAxis(
			reinterpret_cast<D3DXQUATERNION*>(&quat),
			reinterpret_cast<const D3DXVECTOR3*>(&vAxis),
			rad);
#else	// #if defined(__USE_D3D_MATH__)
		// 念のため
		SVector v;
		SVector::Normalize(v, vAxis);

		IZ_FLOAT c = cosf(rad * 0.5f);
		IZ_FLOAT s = sinf(rad * 0.5f);

		quat.x = s * v.x;
		quat.y = s * v.y;
		quat.z = s * v.z;
		quat.w = c;
#endif	// #if defined(__USE_D3D_MATH__)
	}

#if 0
	// ベクトルにクオータニオンを適用する
	void SQuat::ApplyQuat(SVector& dst, const SVector& src, const SQuat& q)
	{
		dst.Set(src.x, src.y, src.z, 0.0f);

		SVector v;
		v.Set(q.x, q.y, q.z, 0.0f);

		IZ_FLOAT ww = q.w * q.w;
		IZ_FLOAT vv = InnerProduct(v, v);
		IZ_FLOAT vd = InnerProduct(v, dst);
		
		SVector cross;
		OuterProduct(cross, v, dst);

		SVector::Scale(dst, dst, ww - vv);
		SVector::Scale(v, v, 2.0f * vd);
		SVector::Scale(cross, cross, 2.0f * q.w);

		SVector::Add(dst, dst, v);
		SVector::Add(dst, dst, cross);
	}
#endif

	// クオータニオンから行列を計算する
	void SQuat::MatrixFromQuat(SMatrix& mtx, const SQuat& quat)
	{
#if defined(__USE_D3D_MATH__)
		D3DXMatrixRotationQuaternion(
			reinterpret_cast<D3DXMATRIX*>(&mtx),
			reinterpret_cast<const D3DXQUATERNION*>(&quat));
#else	// #if defined(__USE_D3D_MATH__)
		const IZ_FLOAT& x = quat.x;
		const IZ_FLOAT& y = quat.y;
		const IZ_FLOAT& z = quat.z;
		const IZ_FLOAT& w = quat.w;

#if 1
		// ベタに計算するとこうなる
		const IZ_FLOAT xx2 = 2.0f * x * x;
		const IZ_FLOAT yy2 = 2.0f * y * y;
		const IZ_FLOAT zz2 = 2.0f * z * z;
		const IZ_FLOAT xy2 = 2.0f * x * y;
		const IZ_FLOAT xz2 = 2.0f * x * z;
		const IZ_FLOAT wz2 = 2.0f * w * z;
		const IZ_FLOAT wy2 = 2.0f * w * y;
		const IZ_FLOAT yz2 = 2.0f * y * z;
		const IZ_FLOAT wx2 = 2.0f * w * x;

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

		SMatrix::Mul(mtx, m1, m2);
		mtx.m[3][3] = 1.0f;
#endif
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// 行列からクオータニオンを計算する
	void SQuat::QuatFromMatrix(SQuat& quat, const SMatrix& mtx)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionRotationMatrix(
			reinterpret_cast<D3DXQUATERNION*>(&quat),
			reinterpret_cast<const D3DXMATRIX*>(&mtx));
#else	// #if defined(__USE_D3D_MATH__)
		// 最大値を探す
		IZ_FLOAT value[4] = {
			 mtx._00 - mtx._11 - mtx._22 + 1.0f,
			-mtx._00 + mtx._11 - mtx._22 + 1.0f,
			-mtx._00 - mtx._11 + mtx._22 + 1.0f,
			 mtx._00 + mtx._11 + mtx._22 + 1.0f,
		};

		IZ_UINT nMaxValIdx = 0;
		for (IZ_UINT i = 0; i < 4; ++i) {
			if (value[i] > value[nMaxValIdx]) {
				nMaxValIdx = i;
			}
		}

		IZ_ASSERT(value[nMaxValIdx] > 0.0f);

		IZ_FLOAT v = sqrtf(value[nMaxValIdx]) * 0.5f;

		// NOTE
		// 1 / (4 * v)
		IZ_FLOAT div = 0.25f / v;

		switch (nMaxValIdx) {
		case 0:	// x
			quat.x = v;
			quat.y = (mtx._01 + mtx._10) * div;
			quat.z = (mtx._20 + mtx._02) * div;
			quat.w = (mtx._12 - mtx._21) * div;
			break;
		case 1:	// y
			quat.x = (mtx._01 + mtx._10) * div;
			quat.y = v;
			quat.z = (mtx._12 + mtx._21) * div;
			quat.w = (mtx._20 - mtx._02) * div;
			break;
		case 2:	// z
			quat.x = (mtx._20 + mtx._02) * div;
			quat.y = (mtx._12 + mtx._21) * div;
			quat.z = v;
			quat.w = (mtx._01 - mtx._10) * div;
			break;
		case 3:	// w
			quat.x = (mtx._12 - mtx._21) * div;
			quat.y = (mtx._20 - mtx._02) * div;
			quat.z = (mtx._01 - mtx._10) * div;
			quat.w = v;
			break;
		}
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// オイラー角からクオータニオンを計算する
	void SQuat::QuatFromEuler(SQuat& quat, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
	{
#if defined(__USE_D3D_MATH__)
		D3DXQuaternionRotationYawPitchRoll(
			reinterpret_cast<D3DXQUATERNION*>(&quat),
			y,	// Yaw
			x,	// Pitch
			z);	// Roll
#else	// #if defined(__USE_D3D_MATH__)
        // Q1 = (x1, y1, z1, w1) = x1i + y1j + z1k + w1
        // Q2 = (x2, y2, z2, w2) = x2i + y2j + z2k + w2
        //
        // ii = jj = kk = -1
        // ij = -ji = k
        // jk = -kj = i
        // ki = -ik = j
        //
        // Q1Q2 = (w1w2 - x1x2 - y1y2 - z1-z2)
        //        + (w1x2 + x1w2 + y1z2 - z1y2)i
        //        + (w1y2 - x1z2 + y1w2 + z1x2)j
        //        + (w1z2 + x1y2 - y1x2 + z1w2)k

        // Qyaw   = (0,            sin(yaw/2), 0,           cos(yaw/2))
        // Qpitch = (sin(pitch/2), 0,          0,           cos(pitch/2))
        // Qroll  = (0,            0,          sin(roll/2), cos(roll/2))
        //
        // Q = QyawQpitchQroll -> Roll Pitch Yaw の順番
        //  (qPq^-1 = QyawQpitchQrollPq^-1 からRoll Pitch Yaw の順番でかけることになる）
        //
        // Cy = cos(yaw/2), Cp = cos(pitch/2), Cr = cos(roll/2)
        // Sy = sin(yaw/2), Sp = sin(pitch/2), Sr = sin(roll/2)
        //
        // QpitchQroll = (CpCr) + (SpCr)i + (-SpSR)j + (CpSr)k
        // QyawQpirchQroll = (CyCpCr + SySpSr) 
        //                      + (CySpCr + SyCpCr)i
        //                      + (-CySpSr + SyCpCr)j
        //                      + (CyCpSr - SySpCr)k

		// Yaw
		IZ_FLOAT cosY = cosf(y * 0.5f);
		IZ_FLOAT sinY = sinf(y * 0.5f);

		// Pitch
		IZ_FLOAT cosX = cosf(x * 0.5f);
		IZ_FLOAT sinX = sinf(x * 0.5f);

		// Roll
		IZ_FLOAT cosZ = cosf(z * 0.5f);
		IZ_FLOAT sinZ = sinf(z * 0.5f);

		quat.Set(
			cosZ * sinX * cosY + sinZ * cosX * sinY,
			cosZ * cosX * sinY - sinZ * sinX * cosY,
			sinZ * cosX * cosY - cosZ * sinX * sinY,
			cosZ * cosX * cosY + sinZ * sinX * sinY);
#endif	// #if defined(__USE_D3D_MATH__)
	}

	// 二つのベクトルv0,v1が与えられたときに
	// q  * v0 == v1 となるクオータニオンqを計算する
	void SQuat::RotationArc(SQuat& quat, const SVector& from, const SVector& to)
	{
		// 念のため
		SVector v0, v1;
		SVector::Normalize(v0, from);
		SVector::Normalize(v1, to);

		SVector c;
		SVector::Cross(c, v0, v1);

		IZ_FLOAT d = SVector::Dot(v0, v1);
		IZ_FLOAT s = sqrtf(2.0f * (1.0f + d));

		quat.x = c.x / s;
		quat.y = c.y / s;
		quat.z = c.z / s;

		quat.w = s * 0.5f;
	}

	// クオータニオンからオイラー角を計算する
	void SQuat::GetEuler(SVector& angle, const SQuat& quat)
	{
		izanagi::SMatrix mtx;
		MatrixFromQuat(mtx, quat);

		// TODO
		// MatrixFromQuatの結果はZXYの回転順番であることを常に保障する？
		SMatrix::GetEulerFromMatrix(angle, mtx, E_MATH_ROTATION_ORDER_ZXY);
	}

	// オイラー角からクオータニオンを計算する
	void SQuat::QuatFromEuler(SQuat& quat, const SVector& angle)
	{
		QuatFromEuler(
			quat,
			angle.x, angle.y, angle.z);
	}
}	// namespace izanagi
