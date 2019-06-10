#if !defined(__IZANAGI_MATH_CMATRIX44_H__)
#define __IZANAGI_MATH_CMATRIX44_H__

#include "MathMatrix44.h"

namespace izanagi
{
namespace math
{
    class CMatrix44 : public SMatrix44 {
    public:
        CMatrix44()
        {
            SetUnit();
        }
        ~CMatrix44() {}

        CMatrix44(const SMatrix44& rhs)
        {
            SMatrix44::Copy(*this, rhs);
        }

        CMatrix44(IZ_FLOAT* rhs)
        {
            memcpy(a, rhs, sizeof(SMatrix44));
        }

        CMatrix44(
            const SVector4& v0,
            const SVector4& v1,
            const SVector4& v2,
            const SVector4& v3)
        {
            SVector4::Copy(v[0], v0);
            SVector4::Copy(v[1], v1);
            SVector4::Copy(v[2], v2);
            SVector4::Copy(v[3], v3);
        }

        CMatrix44(
            IZ_FLOAT _00, IZ_FLOAT _01, IZ_FLOAT _02, IZ_FLOAT _03,
            IZ_FLOAT _10, IZ_FLOAT _11, IZ_FLOAT _12, IZ_FLOAT _13,
            IZ_FLOAT _20, IZ_FLOAT _21, IZ_FLOAT _22, IZ_FLOAT _23,
            IZ_FLOAT _30, IZ_FLOAT _31, IZ_FLOAT _32, IZ_FLOAT _33)
        {
            this->_00 = _00; this->_01 = _01; this->_02 = _02; this->_03 = _03;
            this->_10 = _10; this->_11 = _11; this->_12 = _12; this->_13 = _13;
            this->_20 = _20; this->_21 = _21; this->_22 = _22; this->_23 = _23;
            this->_30 = _30; this->_31 = _31; this->_32 = _32; this->_33 = _33;
        }

        operator IZ_FLOAT*()
        {
            return a;
        }

        operator const IZ_FLOAT*() const
        {
            return a;
        }

        SMatrix44& operator *=(const SMatrix44& rhs)
        {
            SMatrix44::Mul(*this, *this, rhs);
            return *this;
        }

        SMatrix44& operator +=(const SMatrix44& rhs)
        {
            SMatrix44::Add(*this, *this, rhs);
            return *this;
        }

        SMatrix44& operator -=(const SMatrix44& rhs)
        {
            SMatrix44::Sub(*this, *this, rhs);
            return *this;
        }

        SMatrix44& operator *=(IZ_FLOAT rhs)
        {
            SMatrix44::MulScalar(*this, *this, rhs);
        }

        SMatrix44& operator /=(IZ_FLOAT rhs)
        {
            SMatrix44::MulScalar(*this, *this, 1.0f / rhs);
        }

        SMatrix44 operator +() const
        {
            return *this;
        }

        SMatrix44 operator - () const
        {
            SMatrix44 ret;

            ret._00 = -_00; ret._01 = -_01; ret._02 = -_02; ret._03 = -_03;
            ret._10 = -_10; ret._11 = -_11; ret._12 = -_12; ret._13 = -_13;
            ret._20 = -_20; ret._21 = -_21; ret._22 = -_22; ret._23 = -_23;
            ret._30 = -_30; ret._31 = -_31; ret._32 = -_32; ret._33 = -_33;

            return ret;
        }

        SMatrix44 operator *(const SMatrix44& rhs) const
        {
            SMatrix44 ret;
            SMatrix44::Mul(ret, *this, rhs);
            return ret;
        }

		SVector4 operator *(const SVector4& rhs) const
		{
			SVector4 ret;
			Apply(ret, rhs, *this);
			return ret;
		}

        SMatrix44 operator +(const SMatrix44& rhs) const
        {
            SMatrix44 ret;
            SMatrix44::Add(ret, *this, rhs);
            return ret;
        }

        SMatrix44 operator -(const SMatrix44& rhs) const
        {
            SMatrix44 ret;
            SMatrix44::Sub(ret, *this, rhs);
            return ret;
        }

        SMatrix44 operator *(IZ_FLOAT rhs) const
        {
            SMatrix44 ret;
            SMatrix44::MulScalar(ret, *this, rhs);
            return ret;
        }

        SMatrix44 operator /(IZ_FLOAT rhs) const
        {
            SMatrix44 ret;
            SMatrix44::MulScalar(ret, *this, 1.0f / rhs);
            return ret;
        }

        friend SMatrix44 operator *(IZ_FLOAT f, const SMatrix44& mtx)
        {
            SMatrix44 ret;
            SMatrix44::MulScalar(ret, mtx, f);
            return ret;
        }

    public:
        void SetUnit()
        {
            SMatrix44::SetUnit(*this);
        }

        void Transpose()
        {
            SMatrix44::Transpose(*this, *this);
        }

        void Inverse()
        {
            SMatrix44::Inverse(*this, *this);
        }

        IZ_FLOAT Determinant()
        {
            IZ_FLOAT ret = SMatrix44::Determinant(*this);
            return ret;
        }

        void SetTrans(IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            SVector4 trans;
            trans.Set(x, y, z);

            SMatrix44::GetTrans(*this, trans);
        }

        void SetTrans(const SVector4& trans)
        {
            SMatrix44::GetTrans(*this, trans);
        }

        void Trans(IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            SMatrix44::Trans(
                *this, *this,
                x, y, z);
        }

        void SetRotByX(IZ_FLOAT angle)
        {
            SMatrix44::GetRotByX(*this, angle);
        }

        void RotByX(IZ_FLOAT angle)
        {
            SMatrix44::RotByX(*this, *this, angle);
        }

        void SetRotByY(IZ_FLOAT angle)
        {
            SMatrix44::GetRotByY(*this, angle);
        }

        void RotByY(IZ_FLOAT angle)
        {
            SMatrix44::RotByY(*this, *this, angle);
        }

        void SetRotByZ(IZ_FLOAT angle)
        {
            SMatrix44::GetRotByZ(*this, angle);
        }

        void RotByZ(IZ_FLOAT angle)
        {
            SMatrix44::RotByZ(*this, *this, angle);
        }

        void SetRot(
            IZ_FLOAT angle,
            IZ_FLOAT axisX, IZ_FLOAT axisY, IZ_FLOAT axisZ)
        {
            SMatrix44::GetRot(
                *this,
                angle,
                axisX, axisY, axisZ);
        }

        void Rot(
            IZ_FLOAT angle,
            IZ_FLOAT axisX, IZ_FLOAT axisY, IZ_FLOAT axisZ)
        {
            SMatrix44::Rot(
                *this, *this,
                angle,
                axisX, axisY, axisZ);
        }

        void SetScale(IZ_FLOAT scaleX, IZ_FLOAT scaleY, IZ_FLOAT scaleZ)
        {
            SMatrix44::SetScale(
                *this,
                scaleX, scaleY, scaleZ);
        }

        void Scale(IZ_FLOAT scaleX, IZ_FLOAT scaleY, IZ_FLOAT scaleZ)
        {
            SMatrix44::Scale(
                *this, *this,
                scaleX, scaleY, scaleZ);
        }

        void Dump()
        {
            SMatrix44::Dump(*this);
        }
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_CMATRIX44_H__)

