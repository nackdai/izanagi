#if !defined(__IZANAGI_MATH_CMATRIX_H__)
#define __IZANAGI_MATH_CMATRIX_H__

#include "MathMatrix.h"

namespace izanagi
{
namespace math
{
    // うーん・・・
    class CMatrix : public SMatrix {
    public:
        CMatrix();
        ~CMatrix();

        CMatrix(const SMatrix& rhs)
        {
            SMatrix::Copy(*this, rhs);
        }

        CMatrix(IZ_FLOAT* rhs)
        {
            memcpy(a, rhs, sizeof(SMatrix));
        }

        CMatrix(
            const SVector& v0,
            const SVector& v1,
            const SVector& v2,
            const SVector& v3)
        {
            SVector::Copy(v[0], v0);
            SVector::Copy(v[1], v1);
            SVector::Copy(v[2], v2);
            SVector::Copy(v[3], v3);
        }

        CMatrix(
            IZ_FLOAT _00, IZ_FLOAT _01, IZ_FLOAT _02, IZ_FLOAT _03,
            IZ_FLOAT _10, IZ_FLOAT _11, IZ_FLOAT _12, IZ_FLOAT _23,
            IZ_FLOAT _20, IZ_FLOAT _21, IZ_FLOAT _22, IZ_FLOAT _33,
            IZ_FLOAT _30, IZ_FLOAT _31, IZ_FLOAT _32, IZ_FLOAT _43)
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

        SMatrix& operator *=(const SMatrix& rhs)
        {
            SMatrix::Mul(*this, *this, rhs);
            return *this;
        }

        SMatrix& operator +=(const SMatrix& rhs)
        {
            SMatrix::Add(*this, *this, rhs);
            return *this;
        }

        SMatrix& operator -=(const SMatrix& rhs)
        {
            SMatrix::Sub(*this, *this, rhs);
            return *this;
        }

        SMatrix& operator *=(IZ_FLOAT rhs)
        {
            SMatrix::MulScalar(*this, *this, rhs);
        }

        SMatrix& operator /=(IZ_FLOAT rhs)
        {
            SMatrix::MulScalar(*this, *this, 1.0f / rhs);
        }

        SMatrix operator +() const
        {
            return *this;
        }

        SMatrix operator - () const
        {
            SMatrix ret;

            ret._00 = -_00; ret._01 = -_01; ret._02 = -_02; ret._03 = -_03;
            ret._10 = -_10; ret._11 = -_11; ret._12 = -_12; ret._13 = -_13;
            ret._20 = -_20; ret._21 = -_21; ret._22 = -_22; ret._23 = -_23;
            ret._30 = -_30; ret._31 = -_31; ret._32 = -_32; ret._33 = -_33;

            return ret;
        }

        SMatrix operator *(const SMatrix& rhs) const
        {
            SMatrix ret;
            SMatrix::Mul(ret, *this, rhs);
            return ret;
        }

        SMatrix operator +(const SMatrix& rhs) const
        {
            SMatrix ret;
            SMatrix::Add(ret, *this, rhs);
            return ret;
        }

        SMatrix operator -(const SMatrix& rhs) const
        {
            SMatrix ret;
            SMatrix::Sub(ret, *this, rhs);
            return ret;
        }

        SMatrix operator *(IZ_FLOAT rhs) const
        {
            SMatrix ret;
            SMatrix::MulScalar(ret, *this, rhs);
            return ret;
        }

        SMatrix operator /(IZ_FLOAT rhs) const
        {
            SMatrix ret;
            SMatrix::MulScalar(ret, *this, 1.0f / rhs);
            return ret;
        }

        friend SMatrix operator *(IZ_FLOAT f, const SMatrix& mtx)
        {
            SMatrix ret;
            SMatrix::MulScalar(ret, mtx, f);
            return ret;
        }

    public:
        void SetUnit()
        {
            SMatrix::SetUnit(*this);
        }

        void Transpose()
        {
            SMatrix::Transpose(*this, *this);
        }

        void Inverse()
        {
            SMatrix::Inverse(*this, *this);
        }

        IZ_FLOAT Determinant()
        {
            IZ_FLOAT ret = SMatrix::Determinant(*this);
            return ret;
        }

        void SetTrans(IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            SVector trans;
            trans.Set(x, y, z);

            SMatrix::GetTrans(*this, trans);
        }

        void Trans(IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z)
        {
            SMatrix::Trans(
                *this, *this,
                x, y, z);
        }

        void SetRotByX(IZ_FLOAT angle)
        {
            SMatrix::GetRotByX(*this, angle);
        }

        void RotByX(IZ_FLOAT angle)
        {
            SMatrix::RotByX(*this, *this, angle);
        }

        void SetRotByY(IZ_FLOAT angle)
        {
            SMatrix::GetRotByY(*this, angle);
        }

        void RotByY(IZ_FLOAT angle)
        {
            SMatrix::RotByY(*this, *this, angle);
        }

        void SetRotByZ(IZ_FLOAT angle)
        {
            SMatrix::GetRotByZ(*this, angle);
        }

        void RotByZ(IZ_FLOAT angle)
        {
            SMatrix::RotByZ(*this, *this, angle);
        }

        void SetRot(
            IZ_FLOAT angle,
            IZ_FLOAT axisX, IZ_FLOAT axisY, IZ_FLOAT axisZ)
        {
            SMatrix::GetRot(
                *this,
                angle,
                axisX, axisY, axisZ);
        }

        void Rot(
            IZ_FLOAT angle,
            IZ_FLOAT axisX, IZ_FLOAT axisY, IZ_FLOAT axisZ)
        {
            SMatrix::Rot(
                *this, *this,
                angle,
                axisX, axisY, axisZ);
        }

        void SetScale(IZ_FLOAT scaleX, IZ_FLOAT scaleY, IZ_FLOAT scaleZ)
        {
            SMatrix::SetScale(
                *this,
                scaleX, scaleY, scaleZ);
        }

        void Scale(IZ_FLOAT scaleX, IZ_FLOAT scaleY, IZ_FLOAT scaleZ)
        {
            SMatrix::Scale(
                *this, *this,
                scaleX, scaleY, scaleZ);
        }

        void Dump()
        {
            SMatrix::Dump(*this);
        }
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI__MATH_CMATRIX_H__)

