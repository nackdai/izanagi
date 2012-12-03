#if !defined(__IZANAGI_MATH_CQUATERNION_H__)
#define __IZANAGI_MATH_CQUATERNION_H__

#include "MathQuaternion.h"

namespace izanagi
{
namespace math
{
	// うーん・・・
	class CQuat : public SQuat {
	public:
		CQuat()
		{
			x = y = z = w = 0.0f;
		}

		~CQuat() {}

		CQuat(const SQuat& rhs)
		{
			x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		}

		CQuat(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w = 1.0f)
		{
			x = _x; y = _y; z = _z; w = _w;
		}

		operator IZ_FLOAT*()
		{
			return v;
		}

		operator const IZ_FLOAT* () const
		{
			return v;
		}

		const SQuat& operator=(const SQuat& rhs)
		{
			SQuat::Copy(*this, rhs);
			return *this;
		}

		SQuat& operator +=(const SQuat& rhs)
		{
			SQuat::Add(*this, *this, rhs);
			return *this;
		}

		SQuat& operator -=(const SQuat& rhs)
		{
			SQuat::Sub(*this, *this, rhs);
			return *this;
		}

		SQuat& operator *=(const SQuat& rhs)
		{
			SQuat::Mul(*this, *this, rhs);
			return *this;
		}

		SQuat& operator *=(IZ_FLOAT rhs)
		{
			SQuat::Scale(*this, *this, rhs);
			return *this;
		}

		SQuat& operator /= (IZ_FLOAT rhs)
		{
			rhs = 1.0f / rhs;
			SQuat::Scale(*this, *this, rhs);
			return *this;
		}

		SQuat operator +() const
		{
			return *this;
		}

		SQuat operator -() const
		{
			SQuat ret;
			ret.Set(-x, -y, -z, -w);
			return ret;
		}

		SQuat operator +(const SQuat& rhs) const
		{
			SQuat ret;
			SQuat::Add(ret, *this, rhs);
			return ret;
		}

		SQuat operator -(const SQuat& rhs) const
		{
			SQuat ret;
			SQuat::Sub(ret, *this, rhs);
			return ret;
		}

		SQuat operator *(const SQuat& rhs) const
		{
			SQuat ret;
			SQuat::Mul(ret, *this, rhs);
			return ret;
		}

		SQuat operator *(IZ_FLOAT rhs) const
		{
			SQuat ret;
			SQuat::Scale(ret, *this, rhs);
			return ret;
		}

		SQuat operator /(IZ_FLOAT rhs) const
		{
			rhs = 1.0f / rhs;

			SQuat ret;
			SQuat::Scale(ret, *this, rhs);
			return ret;
		}

		friend SQuat operator *(IZ_FLOAT f, const SQuat& v)
		{
			SQuat ret;
			SQuat::Scale(ret, v, f);
			return ret;
		}

	public:
		IZ_FLOAT Length() const
		{
			IZ_FLOAT ret = SQuat::Length(*this);
			return ret;
		}

		void Normalize()
		{
			SQuat::Normalize(*this, *this);
		}

		void Conjugate()
		{
			SQuat::Conjugate(*this, *this);
		}

		void Conjugate(SQuat& ret)
		{
			SQuat::Conjugate(ret, *this);
		}

		void Inverse()
		{
			SQuat::Inverse(*this, *this);
		}
	};
}   // namespace math
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI__MATH_CVECTOR_H__)
