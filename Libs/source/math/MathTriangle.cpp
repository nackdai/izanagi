#include "math/MathTriangle.h"
#include "math/MathRectangle.h"
#include "math/MathPlane.h"
#include "math/MathCVector.h"

namespace izanagi {
	CTriangle::CTriangle()
	{
		a = b = c = d = padding = 0.0f;

		pt[0].Set(0.0f, 0.0f, 0.0f);
		pt[1].Set(0.0f, 0.0f, 0.0f);
		pt[2].Set(0.0f, 0.0f, 0.0f);
	}

	CTriangle::CTriangle(const SVector point[3])
	{
		Set(point);
	}

	CTriangle::CTriangle(const CTriangle& rhs)
	{
		*this = rhs;
	}

	const CTriangle& CTriangle::operator=(const CTriangle& rhs)
	{
		memcpy(pt, rhs.pt, sizeof(pt));
		SVector::Copy(nml, rhs.nml);
		d = rhs.d;

		return *this;
	}

	void CTriangle::Set(const SVector point[3])
	{
		Set(point[0], point[1], point[2]);
	}

	void CTriangle::Set(
		const SVector& point0,
		const SVector& point1,
		const SVector& point2)
	{
		pt[0].Set(point0.x, point0.y, point0.z);
		pt[1].Set(point1.x, point1.y, point1.z);
		pt[2].Set(point2.x, point2.y, point2.z);

		izanagi::CVector dir0(pt[1], pt[0], CVector::INIT_SUB);
		izanagi::CVector dir1(pt[2], pt[0], CVector::INIT_SUB);

		SVector::Cross(nml, dir0, dir1);
		SVector::Normalize(nml, nml);

		d = -(a * pt[0].x + b * pt[0].y + c * pt[0].z);
	}

	// 4x4行列による変換.
	void CTriangle::Transform(const SMatrix& mtx)
	{
		SVector tmp[COUNTOF(pt)];

		for (IZ_UINT i = 0; i < COUNTOF(pt); i++)
		{
			SMatrix::Apply(tmp[i], pt[i], mtx);
		}

		Set(tmp);
	}

	// 三角形上に存在する点かどうか.
	IZ_BOOL CTriangle::IsOnTriangle(const SVector& ptr) const
	{
		for (IZ_UINT i = 0; i < 3; i++)
		{
			IZ_UINT idx = (i + 1) % 3;
			CVector e(
				pt[idx].x - pt[i].x,
				pt[idx].y - pt[i].y,
				pt[idx].z - pt[i].z,
				0.0f);

			idx = (i + 2) % 3;
			CVector f(
				pt[idx].x - pt[i].x,
				pt[idx].y - pt[i].y,
				pt[idx].z - pt[i].z,
				0.0f);

			CVector g(
				ptr.x - pt[i].x,
				ptr.y - pt[i].y,
				ptr.z - pt[i].z,
				0.0f);

			SVector normalOfE;
			{
				// ベクトルeに垂直なベクトルを計算する
				SVector cross;
				
				SVector::Cross(cross, e, f);
				SVector::Cross(normalOfE, cross, e);

				// 三角形の内側にあるか計算する
				IZ_FLOAT dot0 = SVector::Dot(f, normalOfE);
				IZ_FLOAT dot1 = SVector::Dot(g, normalOfE);
				if (dot0 * dot1 < 0.0f)
				{
					return IZ_FALSE;
				}

#if 0
				// 逆向きで計算してみる
				SVector::Cross(normalOfE, e, cross);

				// 三角形の内側にあるか計算する
				dot0 = SVector::Dot(f, normalOfE);
				dot1 = SVector::Dot(g, normalOfE);
				if (dot0 * dot1 >= 0.0f)
				{
					return IZ_TRUE;
				}
#endif
			}
		}

		return IZ_TRUE;
	}

	// レイと交差する点を取得
	IZ_BOOL CTriangle::GetCrossPoint(
		const SRay& ray,
		SVector& refPtr) const
	{
		CPlane plane(nml, pt[0]);

		IZ_BOOL isCross = plane.GetCrossPoint(ray, refPtr);

		if (isCross)
		{
			isCross = IsOnTriangle(refPtr);
		}

		return isCross;
	}

	// レイと交差するかどうか
	IZ_BOOL CTriangle::IsCross(const SRay& ray)
	{
		SVector tmp;

		IZ_BOOL isCross = GetCrossPoint(ray, tmp);
		return isCross;
	}
}	// namespace izanagi
