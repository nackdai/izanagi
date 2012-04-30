#include "math/MathPlane.h"
#include "math/MathRay.h"
#include "math/MathCVector.h"

namespace izanagi {
	// 平面上でレイと交差する点を取得
	IZ_BOOL CPlane::GetCrossPoint(
		const CPlane& plane,
		const SRay& ray,
		SVector& refPtr)
	{
		return plane.GetCrossPoint(ray, refPtr);
	}

	// レイと交差する点を裏表の両面について取得.
	IZ_BOOL CPlane::GetBilateralCrossPoint(
		const CPlane& plane,
		const SRay& ray,
		SVector& refPtr)
	{
		return plane.GetBilateralCrossPoint(ray, refPtr);
	}

	CPlane::CPlane()
	{
		a = b = c = d = padding = 0.0f;
	}

	CPlane::CPlane(const SVector& normal, const SVector& ptr)
	{
		Set(normal, ptr);
	}

	CPlane::CPlane(const SPlane& rhs)
	{
		*this = rhs;
	}

	CPlane::CPlane(IZ_FLOAT _a, IZ_FLOAT _b, IZ_FLOAT _c, IZ_FLOAT _d)
	{
		a = _a;
		b = _b;
		c = _c;
		d = _d;
	}

	const CPlane& CPlane::operator=(const SPlane& rhs)
	{
		Set(rhs.nml, rhs.pt);
		return *this;
	}

	// 平面を設定.
	void CPlane::Set(const SVector& normal, const SVector& ptr)
	{
		SVector::Copy(nml, normal);

		nml.w = 0.0f;
		SVector::Normalize(nml, nml);

		d = -(a * ptr.x + b * ptr.y + c * ptr.z);

		pt.Set(ptr.x, ptr.y, ptr.z);
	}

	// 原点からの距離を取得.
	IZ_FLOAT CPlane::GetDistance() const
	{
		return ::fabs(d);
	}

	// 平面上に存在する点かどうか.
	IZ_BOOL CPlane::IsOnPlane(const SVector& ptr)
	{
		IZ_FLOAT tmp = a * ptr.x + b * ptr.y + c * ptr.z;
		return CMath::IsNearyEqualZero(tmp + d);
	}

	// 4x4行列による変換.
	void CPlane::Transform(const SMatrix& mtx)
	{
		// TODO
		// Matrixにスケールが含まれていた場合
		// 一度Normalizeする？

		SMatrix::ApplyXYZ(nml, nml, mtx);
	}

	// 平面をあらわすベクトルを取得
	void CPlane::GetPlaneVector(SVector& plane)
	{
		plane.Set(a, b, c, d);
	}

	// レイと交差する点を取得
	IZ_BOOL CPlane::GetCrossPoint(
		const SRay& ray,
		SVector& refPtr) const
	{
		// NOTE
		// Plane : L = <N, D>
		// Ray   : P(t) = Q + Vt
		//
		// N・P(t) + D = 0
		// <=> N・Q + (N・V)t + D = 0
		// <=> t = -(N・Q + D) / N・V
		// <=> t = -L・Q / L・V
		//
		// Q = (Qx, Qy, Qz, 1) => 位置ベクトル
		// V = (Vx, Vy, Vz, 0) => 方向ベクトル
		// L = (Nx, Ny, Nz, D)
		// なので、
		// N・Q + D = Nx * Qx + Ny * Qy + Nz * Qz + D * 1 = L・Q
		// N・V + 0 = Nx * Vx + Ny * Vy + Nz * Vz + D * 0 = L・V

		CVector plane(a, b, c, d);

		// L・V
		IZ_FLOAT d = SVector::Dot(plane, ray.v);
		if (d >= 0.0f) {

			// NOTE
			//   | /       |
			//   |/        |
			//---+----  ---+---
			//            /
			//           /
			//  NG        OK
			//
			// 面の法線となす角が90度より大きければ面と交わる
			// => 内積の値がマイナスであれば面と交わる

			return IZ_FALSE;
		}

		// L・Q
		IZ_FLOAT t = SVector::Dot(plane, ray.p);

		// t = -L・Q / L・V 
		t = -t / d;

		refPtr.x = ray.p.x + t * ray.v.x;
		refPtr.y = ray.p.y + t * ray.v.y;
		refPtr.z = ray.p.z + t * ray.v.z;
		refPtr.w = 1.0f;

		return IZ_TRUE;
	}

	// レイと交差するかどうか
	IZ_BOOL CPlane::IsCross(const SRay& ray) const
	{
		CVector plane(a, b, c, d);

		IZ_FLOAT d = SVector::Dot(plane, ray.v);

		// NOTE
		// 面の法線となす角が90度より大きければ面と交わる
		// => 内積の値がマイナスであれば面と交わる

		return (d < 0.0f);
	}

	// レイと交差する点を裏表の両面について取得.
	IZ_BOOL CPlane::GetBilateralCrossPoint(
		const SRay& ray,
		SVector& refPtr) const
	{
		// 表
		if (CPlane::GetCrossPoint(*this, ray, refPtr))
		{
			return IZ_TRUE;
		}

		// 裏
		CPlane planeBack(-a, -b, -c, d);
		if (CPlane::GetCrossPoint(planeBack, ray, refPtr))
		{
			return IZ_TRUE;
		}

		return IZ_FALSE;
	}

	// 裏表の両面についてレイと交差するかどうか.
	IZ_BOOL CPlane::IsBilateralCross(const SRay& ray) const
	{
		// 表
		if (IsCross(ray))
		{
			return IZ_TRUE;
		}

		// 裏
		CPlane planeBack(-a, -b, -c, d);
		return planeBack.IsCross(ray);
	}

	// 面の正側（法線の向き側）に点があるかどうか.
	IZ_BOOL CPlane::IsPositive(const SVector& ptr) const
	{
		// 基準点からの方向ベクトル
		CVector dir(
			ptr.x - pt.x,
			ptr.y - pt.y,
			ptr.z - pt.z,
			0.0f);

		// NOTE
		//   | /       |
		//   |/        |
		//---+----  ---+---
		//            /
		//           /
		//
		// 面の法線となす角が90度より小さければ面の正の側
		// => 内積の値がプラスであれば法線の向き側に点がある

		IZ_FLOAT dot = SVector::Dot(nml, dir);

		return (dot >= 0.0f);
	}
}	// namespace izanagi
