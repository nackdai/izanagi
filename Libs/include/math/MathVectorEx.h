#if !defined(__IZANAGI__MATH_VECTOR_EX_H__)
#define __IZANAGI__MATH_VECTOR_EX_H__

#include "MathVector.h"

namespace izanagi {
	// うーん・・・
	class CVector : public SVector {
	public:
		CVector()
		{
			x = y = z = w = 0.0f;
		}

		~CVector() {}

		CVector(const CVector& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}

		CVector(const SVector& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}

		CVector(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w = 1.0f)
		{
			x = _x; y = _y; z = _z; w = _w;
		}

		const CVector& operator=(const CVector& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
			return *this;
		}

		const CVector& operator=(const SVector& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
			return *this;
		}

		operator SVector&()
		{
			return *(SVector*)this;
		}

		const CVector operator*(IZ_FLOAT f) const
		{
			CVector ret(*this);
			ScaleVector(ret, ret, f);
			return ret;
		}

		const CVector& operator*=(IZ_FLOAT f)
		{
			ScaleVector(*this, *this, f);
			return *this;
		}

		const CVector operator/(IZ_FLOAT f) const
		{
			CVector ret(*this);
			ScaleVector(ret, ret, 1.0f / f);
			return ret;
		}

		const CVector& operator/=(IZ_FLOAT f)
		{
			ScaleVector(*this, *this, 1.0f / f);
			return *this;
		}

		const CVector operator+(const SVector& rhs) const
		{
			CVector ret(*this);
			AddVector(ret, ret, rhs);
			return ret;
		}

		const CVector operator+(const CVector& rhs) const
		{
			CVector ret(*this);
			AddVector(ret, ret, rhs);
			return ret;
		}

		const CVector& operator+=(const SVector& rhs)
		{
			AddVector(*this, *this, rhs);
			return *this;
		}

		const CVector& operator+=(const CVector& rhs)
		{
			AddVector(*this, *this, rhs);
			return *this;
		}

		const CVector operator-(const SVector& rhs) const
		{
			CVector ret(*this);
			SubVector(ret, ret, rhs);
			return ret;
		}

		const CVector operator-(const CVector& rhs) const
		{
			CVector ret(*this);
			SubVector(ret, ret, rhs);
			return ret;
		}

		const CVector& operator-=(const SVector& rhs)
		{
			SubVector(*this, *this, rhs);
			return *this;
		}

		const CVector& operator-=(const CVector& rhs)
		{
			SubVector(*this, *this, rhs);
			return *this;
		}

		IZ_FLOAT dot(const SVector& rhs) const
		{
			IZ_FLOAT ret = DotVector(*this, rhs);
			return ret;
		}

		IZ_FLOAT dot(const CVector& rhs) const
		{
			IZ_FLOAT ret = DotVector(*this, rhs);
			return ret;
		}

	public:
		friend void operator*(IZ_FLOAT f, CVector& rhs)
		{
			ScaleVector(rhs, rhs, f);
		}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_VECTOR_EX_H__)
