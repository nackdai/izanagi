#include "math/MathRectangle.h"
#include "math/MathRay.h"
#include "math/MathPlane.h"
#include "math/MathCVector.h"

namespace izanagi {
	CRectangle::CRectangle()
	{
		a = b = c = d = padding = 0.0f;

		pt.Set(0.0f, 0.0f, 0.0f);
		v[0].dir.Set(0.0f, 0.0f, 0.0f, 0.0f);
		v[0].dir.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	CRectangle::CRectangle(
		const SVector& point,
		const SVector& dir1,
		const SVector& dir2)
	{
		Set(point, dir1, dir2);
	}

	CRectangle::CRectangle(
		const SVector& point,
		const SVector& dir1, IZ_FLOAT length1,
		const SVector& dir2, IZ_FLOAT length2)
	{
		Set(
			point,
			dir1, length1,
			dir2, length2);
	}

	CRectangle::CRectangle(const CRectangle& rhs)
	{
		*this = rhs;
	}

	const CRectangle& CRectangle::operator=(const CRectangle& rhs)
	{
		SVector::Copy(pt, rhs.pt);

		v[0].length = rhs.v[0].length;
		SVector::Copy(v[0].dir, rhs.v[0].dir);

		v[1].length = rhs.v[1].length;
		SVector::Copy(v[1].dir, rhs.v[1].dir);

		SVector::Copy(nml, rhs.nml);

		d = rhs.d;

		return *this;
	}

	// 矩形を設定.
	void CRectangle::Set(
		const SVector& point,
		const SVector& dir1,
		const SVector& dir2)
	{
		SVector d1;
		SVector::Normalize(d1, dir1);
		IZ_FLOAT length1 = SVector::Length(dir1);

		SVector d2;
		SVector::Normalize(d2, dir2);
		IZ_FLOAT length2 = SVector::Length(dir2);

		Set(
			point,
			d1, length1,
			d2, length2);
	}

	// 矩形を設定.
	void CRectangle::Set(
		const SVector& point,
		const SVector& dir1, IZ_FLOAT length1,
		const SVector& dir2, IZ_FLOAT length2)
	{
		SVector::Copy(pt, point);

		v[0].length = length1;
		SVector::Normalize(v[0].dir, dir1);

		v[1].length = length2;
		SVector::Normalize(v[1].dir, dir2);

		SVector::Cross(nml, v[0].dir, v[1].dir);
		SVector::Normalize(nml, nml);

		d = -(a * pt.x + b * pt.y + c * pt.z);
	}

	// 原点からの距離を取得.
	IZ_FLOAT CRectangle::GetDistance() const
	{
		return ::fabs(d);
	}

	// 矩形上に存在する点かどうか.
	IZ_BOOL CRectangle::IsOnRectangle(const SVector& ptr)
	{
		SVector p;
		SVector::Sub(p, ptr, pt);

		IZ_FLOAT dot1 = SVector::Dot(p, v[0].dir);
		IZ_FLOAT dot2 = SVector::Dot(p, v[1].dir);

		IZ_FLOAT length1 = v[0].length;
		IZ_FLOAT length2 = v[1].length;

		return IS_IN_BOUND(dot1 / length1, 0.0f, 1.0f) && IS_IN_BOUND(dot2 / length2, 0.0f, 1.0f);
	}

	// 4x4行列による変換.
	void CRectangle::Transform(const SMatrix& mtx)
	{
		SMatrix::Apply(pt, pt, mtx);

		for (IZ_UINT i = 0; i < COUNTOF(v); i++)
		{
			SVector tmp;
			SVector::Scale(tmp, v[i].dir, v[i].length);

			SMatrix::ApplyXYZ(v[i].dir, tmp, mtx);

			v[i].length = SVector::Length(v[i].dir);
			SVector::Normalize(v[i].dir, v[i].dir);
		}

		SVector::Cross(nml, v[0].dir, v[1].dir);
		SVector::Normalize(nml, nml);

		d = -(a * pt.x + b * pt.y + c * pt.z);
	}

	// レイと交差する点を取得
	IZ_BOOL CRectangle::GetCrossPoint(
		const SRay& ray,
		SVector& refPtr)
	{
		CVector ptr;

		IZ_BOOL isCross = CPlane::GetCrossPoint(
			CPlane(nml, pt),
			ray, ptr);

		if (isCross) {
			isCross = IsOnRectangle(ptr);
			if (isCross) {
				SVector::Copy(refPtr, ptr);
			}
		}

		return isCross;
	}

	// レイと交差するかどうか
	IZ_BOOL CRectangle::IsCross(const SRay& ray)
	{
		CVector ptr;

		IZ_BOOL isCross = GetCrossPoint(ray, ptr);

		return isCross;
	}
}	// namespace izanagi
