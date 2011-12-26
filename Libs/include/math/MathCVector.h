#if !defined(__IZANAGI_MATH_CVECTOR_H__)
#define __IZANAGI_MATH_CVECTOR_H__

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

		CVector(const SVector& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}

		CVector(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w = 1.0f)
		{
			x = _x; y = _y; z = _z; w = _w;
		}

		operator IZ_FLOAT*()
		{
			return v;
		}

		operator const IZ_FLOAT*() const
		{
			return v;
		}

		const CVector& operator=(const SVector& rhs)
		{
			SVector::Copy(*this, rhs);
			return *this;
		}

		CVector& operator +=(const SVector& rhs)
		{
			SVector::Add(*this, *this, rhs);
			return *this;
		}

		CVector& operator -=(const SVector& rhs)
		{
			SVector::Sub(*this, *this, rhs);
			return *this;
		}

		CVector& operator *=(IZ_FLOAT rhs)
		{
			SVector::Scale(*this, *this, rhs);
			return *this;
		}

		CVector& operator /=(IZ_FLOAT rhs)
		{
			rhs = 1.0f / rhs;
			SVector::Scale(*this, *this, rhs);
			return *this;
		}

		CVector operator +() const
		{
			return *this;
		}

		CVector operator -() const
		{
			CVector ret;
			ret.Set(-x, -y, -z, -w);
			return ret;
		}

		CVector operator +(const SVector& rhs) const
		{
			CVector ret;
			SVector::Add(ret, *this, rhs);
			return ret;
		}

		CVector operator -(const SVector& rhs) const
		{
			CVector ret;
			SVector::Sub(ret, *this, rhs);
			return ret;
		}

		CVector operator *(IZ_FLOAT rhs) const
		{
			CVector ret;
			SVector::Scale(ret, *this, rhs);
			return ret;
		}

		CVector operator /(IZ_FLOAT rhs) const
		{
			rhs = 1.0f / rhs;

			CVector ret;
			SVector::Scale(ret, *this, rhs);
			return ret;
		}

		friend CVector operator *(IZ_FLOAT f, const SVector& v)
		{
			CVector ret;
			SVector::Scale(ret, v, f);
			return ret;
		}

	public:
		IZ_FLOAT Length() const
		{
			IZ_FLOAT ret = SVector::Length(*this);
			return ret;
		}

		void Normalize()
		{
			SVector::Normalize(*this, *this);
		}

		IZ_FLOAT Dot(const SVector& rhs) const
		{
			IZ_FLOAT ret = SVector::Dot(*this, rhs);
			return ret;
		}

		void Cross(const SVector& rhs)
		{
			SVector::Cross(*this, *this, rhs);
		}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_CVECTOR_H__)
