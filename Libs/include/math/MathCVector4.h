#if !defined(__IZANAGI_MATH_CVECTOR4_H__)
#define __IZANAGI_MATH_CVECTOR4_H__

#include "math/MathVector4.h"
#include "math/MathCMatrix44.h"

namespace izanagi
{
namespace math
{
    // うーん・・・
    class CVector4 : public SVector4 {
    public:
        enum INIT_TYPE
        {
            INIT_ADD,
            INIT_SUB,
        };

    public:
        CVector4()
        {
            x = y = z = w = 0.0f;
        }

        ~CVector4() {}

        CVector4(const SVector4& rhs)
        {
            x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
        }

        CVector4(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w = 1.0f)
        {
            x = _x; y = _y; z = _z; w = _w;
        }

        CVector4(const SVector4& v0, const SVector4& v1, INIT_TYPE type)
        {
            switch (type)
            {
            case INIT_ADD:
                SVector4::Add(*this, v0, v1);
                break;
            case INIT_SUB:
                SVector4::Sub(*this, v0, v1);
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

        const CVector4& operator=(const SVector4& rhs)
        {
            SVector4::Copy(*this, rhs);
            return *this;
        }

        CVector4& operator +=(const SVector4& rhs)
        {
            SVector4::Add(*this, *this, rhs);
            return *this;
        }

        CVector4& operator +=(const SVector3& rhs)
        {
            this->x += rhs.x;
            this->y += rhs.y;
            this->z += rhs.z;
            return *this;
        }

        CVector4& operator -=(const SVector4& rhs)
        {
            SVector4::Sub(*this, *this, rhs);
            return *this;
        }

        CVector4& operator -=(const SVector3& rhs)
        {
            this->x -= rhs.x;
            this->y -= rhs.y;
            this->z -= rhs.z;
            return *this;
        }

        CVector4& operator *=(IZ_FLOAT rhs)
        {
            SVector4::Scale(*this, *this, rhs);
            return *this;
        }

        CVector4& operator *=(const CMatrix44& rhs)
        {
            SMatrix44::Apply(*this, *this, rhs);
            return *this;
        }

        CVector4& operator /=(IZ_FLOAT rhs)
        {
            rhs = 1.0f / rhs;
            SVector4::Scale(*this, *this, rhs);
            return *this;
        }

        CVector4 operator +() const
        {
            return *this;
        }

        CVector4 operator -() const
        {
            return CVector4(-x, -y, -z, -w);
        }

        CVector4 operator +(const SVector4& rhs) const
        {
            return CVector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        CVector4 operator +(const SVector3& rhs) const
        {
            return CVector4(x + rhs.x, y + rhs.y, z + rhs.z, w);
        }

        CVector4 operator -(const SVector4& rhs) const
        {
            return CVector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        CVector4 operator -(const SVector3& rhs) const
        {
            return CVector4(x - rhs.x, y - rhs.y, z - rhs.z, w);
        }

        CVector4 operator *(IZ_FLOAT rhs) const
        {
            return CVector4(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        CVector4 operator *(const CMatrix44& rhs) const
        {
            CVector4 ret(*this);
            SMatrix44::Apply(ret, ret, rhs);
            return std::move(ret);
        }

        CVector4 operator /(IZ_FLOAT rhs) const
        {
            rhs = 1.0f / rhs;
            return CVector4(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        friend CVector4 operator *(IZ_FLOAT f, const SVector4& v)
        {
            return CVector4(f * v.x, f * v.y, f * v.z, f * v.w);
        }

    public:
        IZ_FLOAT Length() const
        {
            IZ_FLOAT ret = SVector4::Length(*this);
            return ret;
        }

        void Normalize()
        {
            SVector4::Normalize(*this, *this);
        }

        IZ_FLOAT Dot(const SVector4& rhs) const
        {
            IZ_FLOAT ret = SVector4::Dot(*this, rhs);
            return ret;
        }

        void Cross(const SVector4& rhs)
        {
            SVector4::Cross(*this, *this, rhs);
        }

        void Add(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z)
        {
            x += _x; y += _y; z += _z;
        }

        void Dump() const
        {
            SVector4::Dump(*this);
        }
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_CVECTOR4_H__)
