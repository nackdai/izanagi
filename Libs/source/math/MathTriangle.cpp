#include "math/MathTriangle.h"

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

	// 4x4s—ñ‚É‚æ‚é•ÏŠ·.
	void CTriangle::Transform(const SMatrix& mtx)
	{
		SVector tmp[COUNTOF(pt)];

		for (IZ_UINT i = 0; i < COUNTOF(pt); i++)
		{
			SMatrix::Apply(tmp[i], pt[i], mtx);
		}

		Set(tmp);
	}
}	// namespace izanagi
