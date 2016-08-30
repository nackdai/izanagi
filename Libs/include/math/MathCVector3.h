#if !defined(__IZANAGI_MATH_CVECTOR3_H__)
#define __IZANAGI_MATH_CVECTOR3_H__

#include "MathVector3.h"

namespace izanagi
{
namespace math
{
    // うーん・・・
    class CVector3 : public SVector3 {
    public:
        enum INIT_TYPE
        {
            INIT_ADD,
            INIT_SUB,
        };

        static CVector3 yup()
        {
            static CVector3 up(0, 1, 0);
            return std::move(up);
        }

    public:
        CVector3()
        {
            x = y = z = 0.0f;
        }

        ~CVector3() {}

        CVector3(const SVector3& rhs)
        {
            x = rhs.x; y = rhs.y; z = rhs.z;
        }

        CVector3(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z)
        {
            x = _x; y = _y; z = _z;
        }

        CVector3(const SVector3& v0, const SVector3& v1, INIT_TYPE type)
        {
            switch (type)
            {
            case INIT_ADD:
                SVector3::Add(*this, v0, v1);
                break;
            case INIT_SUB:
                SVector3::Sub(*this, v0, v1);
                break;
            }
        }

        operator IZ_FLOAT*()
        {
            return v;
        }

        operator const IZ_FLOAT*() const
        {
            return v;
        }

        const CVector3& operator=(const SVector3& rhs)
        {
            SVector3::Copy(*this, rhs);
            return *this;
        }

        CVector3& operator +=(const SVector3& rhs)
        {
            SVector3::Add(*this, *this, rhs);
            return *this;
        }

        CVector3& operator -=(const SVector3& rhs)
        {
            SVector3::Sub(*this, *this, rhs);
            return *this;
        }

        CVector3& operator *=(IZ_FLOAT rhs)
        {
            SVector3::Scale(*this, *this, rhs);
            return *this;
        }

        CVector3& operator /=(IZ_FLOAT rhs)
        {
            rhs = 1.0f / rhs;
            SVector3::Scale(*this, *this, rhs);
            return *this;
        }

        CVector3 operator +() const
        {
            return *this;
        }

        CVector3 operator -() const
        {
            return CVector3(-x, -y, -z);
        }

        CVector3 operator +(const SVector3& rhs) const
        {
            return CVector3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        CVector3 operator -(const SVector3& rhs) const
        {
            return CVector3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        CVector3 operator *(IZ_FLOAT rhs) const
        {
            return CVector3(x * rhs, y * rhs, z * rhs);
        }

        CVector3 operator /(IZ_FLOAT rhs) const
        {
            rhs = 1.0f / rhs;
            return CVector3(x * rhs, y * rhs, z * rhs);
        }

        friend CVector3 operator *(IZ_FLOAT f, const SVector3& v)
        {
            return CVector3(f * v.x, f * v.y, f * v.z);
        }

        IZ_BOOL operator==(const CVector3& rhs)
        {
            IZ_FLOAT dot = Dot(rhs) - 1.0f;
            return (-IZ_MATH_EPSILON < dot && dot < IZ_MATH_EPSILON);
        }

    public:
        IZ_BOOL Equals(const CVector3& rhs)
        {
            return *this == rhs;
        }

        IZ_FLOAT Length() const
        {
            IZ_FLOAT ret = SVector3::Length(*this);
            return ret;
        }

        void Normalize()
        {
            SVector3::Normalize(*this, *this);
        }

        IZ_FLOAT Dot(const SVector3& rhs) const
        {
            IZ_FLOAT ret = SVector3::Dot(*this, rhs);
            return ret;
        }

        void Cross(const SVector3& rhs)
        {
            SVector3::Cross(*this, *this, rhs);
        }
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_CVECTOR3_H__)
