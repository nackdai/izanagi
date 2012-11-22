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
		v[1].dir.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	CRectangle::CRectangle(
		const SVector& point,
		IZ_FLOAT lengthX,
		IZ_FLOAT lengthZ)
	{
		Set(
			point,
			lengthX, lengthZ);
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

	static const izanagi::CVector DirX(1.0f, 0.0f,  0.0f, 0.0f);
	static const izanagi::CVector DirZ(0.0f, 0.0f, -1.0f, 0.0f);

	// 矩形を設定.
	void CRectangle::Set(
		const SVector& point,
		IZ_FLOAT lengthX,
		IZ_FLOAT lengthZ)
	{
		SVector::Copy(pt, point);

		v[0].length = lengthX;
		v[0].dir.Set(DirX);

		v[1].length = lengthZ;
		v[1].dir.Set(DirZ);

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
	IZ_BOOL CRectangle::IsOnRectangle(const SVector& ptr) const
	{
		SVector p;
		SVector::Sub(p, ptr, pt);

		IZ_FLOAT dot1 = SVector::Dot(p, v[0].dir);
		IZ_FLOAT dot2 = SVector::Dot(p, v[1].dir);

		IZ_FLOAT length1 = v[0].length;
		IZ_FLOAT length2 = v[1].length;

#if 0
		return IS_IN_BOUND(dot1 / length1, 0.0f, 1.0f) && IS_IN_BOUND(dot2 / length2, 0.0f, 1.0f);
#else
		// ptは矩形の中心点なので
		// 矩形を構成する点は二つのベクトルの -0.5 - 0.5 の範囲に存在する
		return IS_IN_BOUND(dot1 / length1, -0.5f, 0.5f)
			&& IS_IN_BOUND(dot2 / length2, -0.5f, 0.5f);
#endif
	}

	// 4x4行列による変換.
	void CRectangle::Transform(const SMatrix& mtx)
	{
		CVector p(0.0f, 0.0f, 0.0f);
		SMatrix::Apply(p, p, mtx);

		SVector::AddXYZ(pt, pt, p);

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

		// 計算誤算を丸める
		nml.x = izanagi::CMath::IsNearyEqualZero(nml.x) ? 0.0f : nml.x;
		nml.y = izanagi::CMath::IsNearyEqualZero(nml.y) ? 0.0f : nml.y;
		nml.z = izanagi::CMath::IsNearyEqualZero(nml.z) ? 0.0f : nml.z;
		SVector::Normalize(nml, nml);

		d = -(a * pt.x + b * pt.y + c * pt.z);
	}

	// レイと交差する点を取得
	IZ_BOOL CRectangle::GetCrossPoint(
		const SRay& ray,
		SVector& refPtr) const
	{
		IZ_BOOL isCross = GetCrossPoint(
			CPlane::GetCrossPoint,
			ray,
			refPtr);

		return isCross;
	}

	// レイと交差するかどうか
	IZ_BOOL CRectangle::IsCross(const SRay& ray)
	{
		CVector ptr;

		IZ_BOOL isCross = GetCrossPoint(ray, ptr);

		return isCross;
	}

	// 平面を取得.
	void CRectangle::GetPlane(SPlane& plane) const
	{
		plane.pt.Set(
			pt.x,
			pt.y,
			pt.z);

		plane.nml.Set(a, b, c, 0.0f);
		plane.d = d;
	}

	// レイと交差する点を取得
	IZ_BOOL CRectangle::GetCrossPoint(
		CRectangle::GetCrossPointFunc func,
		const SRay& ray,
		SVector& refPtr) const
	{
		CVector ptr;

		IZ_BOOL isCross = (*func)(
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

	// X方向のベクトルを取得.
	const SVector& CRectangle::GetX() const
	{
		return v[0].dir;
	}

	// Y方向のベクトルを取得.
	const SVector& CRectangle::GetY() const
	{
		return v[1].dir;
	}

	// 幅を取得.
	IZ_FLOAT CRectangle::GetWidth() const
	{
		return v[0].length;
	}

	// 高さを取得.
	IZ_FLOAT CRectangle::GetHeight() const
	{
		return v[1].length;
	}
}	// namespace izanagi
