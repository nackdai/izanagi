#if !defined(__URANUS__MATH_VECTOR_EX_H__)
#define __URANUS__MATH_VECTOR_EX_H__

#include "MathVector.h"

namespace uranus {
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

		CVector(UN_FLOAT _x, UN_FLOAT _y, UN_FLOAT _z, UN_FLOAT _w = 1.0f)
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

		const CVector operator*(UN_FLOAT f) const
		{
			CVector ret(*this);
			ScaleVector(ret, ret, f);
			return ret;
		}

		const CVector& operator*=(UN_FLOAT f)
		{
			ScaleVector(*this, *this, f);
			return *this;
		}

		const CVector operator/(UN_FLOAT f) const
		{
			CVector ret(*this);
			ScaleVector(ret, ret, 1.0f / f);
			return ret;
		}

		const CVector& operator/=(UN_FLOAT f)
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

		UN_FLOAT dot(const SVector& rhs) const
		{
			UN_FLOAT ret = DotVector(*this, rhs);
			return ret;
		}

		UN_FLOAT dot(const CVector& rhs) const
		{
			UN_FLOAT ret = DotVector(*this, rhs);
			return ret;
		}

	public:
		friend void operator*(UN_FLOAT f, CVector& rhs)
		{
			ScaleVector(rhs, rhs, f);
		}
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS__MATH_VECTOR_EX_H__)
