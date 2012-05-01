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

		v[0].dir.Set(0.0f, 0.0f, 0.0f, 0.0f);
		v[1].dir.Set(0.0f, 0.0f, 0.0f, 0.0f);
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

		v[0].length = rhs.v[0].length;
		SVector::Copy(v[0].dir, rhs.v[0].dir);

		v[1].length = rhs.v[1].length;
		SVector::Copy(v[1].dir, rhs.v[1].dir);

		SVector::Copy(nml, rhs.nml);

		d = rhs.d;

		return *this;
	}

	void CTriangle::Set(const SVector point[3])
	{
		for (IZ_UINT i = 0; i < COUNTOF(pt); i++)
		{
			pt[i].Set(point[i].x, point[i].y, point[i].z);
		}

		SVector::Sub(v[0].dir, pt[1], pt[0]);
		v[0].length = SVector::Length(v[0].dir);
		SVector::Normalize(v[0].dir, v[0].dir);

		SVector::Sub(v[1].dir, pt[2], pt[0]);
		v[1].length = SVector::Length(v[1].dir);
		SVector::Normalize(v[1].dir, v[1].dir);

		SVector::Cross(nml, v[0].dir, v[1].dir);
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
		CRectangle rc(
			pt[0],
			v[0].dir, v[0].length,
			v[1].dir, v[1].length);

		IZ_BOOL isOn = rc.IsOnRectangle(ptr);

		if (isOn)
		{
			for (IZ_UINT i = 0; i < 3; i++)
			{
				IZ_UINT idx = (i + 1) % 3;
				CVector e(
					pt[idx].x - pt[i].x,
					pt[idx].y - pt[i].y,
					pt[idx].z - pt[i].z);

				idx = (i + 1) % 3;
				CVector f(
					pt[idx].x - pt[i].x,
					pt[idx].y - pt[i].y,
					pt[idx].z - pt[i].z);

				CVector g(
					ptr.x - pt[i].x,
					ptr.y - pt[i].y,
					ptr.z - pt[i].z);

				SVector normalOfE;
				{
					// ベクトルeに垂直なベクトルを計算する
					SVector cross;
					
					SVector::Cross(cross, e, f);
					SVector::Cross(normalOfE, cross, e);

					// 三角形の内側にあるか計算する
					IZ_FLOAT dot0 = SVector::Dot(f, normalOfE);
					IZ_FLOAT dot1 = SVector::Dot(g, normalOfE);
					if (dot0 * dot1 >= 0.0f)
					{
						return IZ_TRUE;
					}

					// 逆向きで計算してみる
					SVector::Cross(normalOfE, e, cross);

					// 三角形の内側にあるか計算する
					dot0 = SVector::Dot(f, normalOfE);
					dot1 = SVector::Dot(g, normalOfE);
					if (dot0 * dot1 >= 0.0f)
					{
						return IZ_TRUE;
					}
				}
			}
		}

		return IZ_FALSE;
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

	// レイと交差する点を裏表の両面について取得.
	IZ_BOOL CTriangle::GetBilateralCrossPoint(
		const SRay& ray,
		SVector& refPtr) const
	{
		CPlane plane(nml, pt[0]);

		IZ_BOOL isCross = plane.GetBilateralCrossPoint(ray, refPtr);

		if (isCross)
		{
			isCross = IsOnTriangle(refPtr);
		}

		return isCross;
	}

	// 裏表の両面についてレイと交差するかどうか.
	IZ_BOOL CTriangle::IsBilateralCross(const SRay& ray) const
	{
		SVector tmp;

		IZ_BOOL isCross = GetBilateralCrossPoint(ray, tmp);
		return isCross;
	}

	// 三角形の基準点からのベクトルに対する割合を取得.
	IZ_BOOL CTriangle::GetFraction(
		const SVector& point,
		IZ_FLOAT f[2]) const
	{
		IZ_BOOL isOn = IsOnTriangle(point);
		if (!isOn)
		{
			// そもそも三角形上にないので何もしない
			return IZ_FALSE;
		}

		// それぞれに対するなす角を計算する
	}
}	// namespace izanagi
