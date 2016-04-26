#if !defined(__IZANAGI__MATH_QUATERNION_H__)
#define __IZANAGI__MATH_QUATERNION_H__

#include <math.h>
#include <memory.h>
#include "izDefs.h"
#include "MathVector4.h"
#include "MathMatrix44.h"

namespace izanagi
{
namespace math
{
    struct SMatrix44;

    struct SQuat : public SVector4 {
        void Set(IZ_FLOAT _x, IZ_FLOAT _y, IZ_FLOAT _z, IZ_FLOAT _w = 1.0f)
        {
            x = _x; y = _y; z = _z; w = _w;
        }

        void Set(const SVector4& rhs)
        {
            x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
        }

        // クオータニオンを設定する
        static void Set(SQuat& quat, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT w)
        {
            quat.x = x;
            quat.y = y;
            quat.z = z;
            quat.w = w;
        }

        // 全成分０のクオータニオンを設定する
        static void SetZero(SQuat& quat)
        {
            quat.x = 0.0f;
            quat.y = 0.0f;
            quat.z = 0.0f;
            quat.w = 0.0f;
        }

        static inline void SetIdentity(SQuat& quat);

        // クオータニオンをコピー
        static void Copy(SQuat& dst, const SQuat& src)
        {
            dst.x = src.x;
            dst.y = src.y;
            dst.z = src.z;
            dst.w = src.w;
        }

        static void Copy(SVector4& dst, const SQuat& src)
        {
            dst.x = src.x;
            dst.y = src.y;
            dst.z = src.z;
            dst.w = src.w;
        }

        // ２つのクオータニオンをスワップする。
        static void Swap(SQuat& q1, SQuat& q2)
        {
            SQuat q;
            Copy(q, q1);
            Copy(q1, q2);
            Copy(q2, q);
        }

        // クオータニオンを加算する
        // dst = src1 + src2
        static void Add(SQuat& dst, const SQuat& src1, const SQuat& src2)
        {
            dst.x = src1.x + src2.x;
            dst.y = src1.y + src2.y;
            dst.z = src1.z + src2.z;
            dst.w = src1.w + src2.w;
        }

        // クオータニオンを減算する
        // dst = src1 - src2
        static void Sub(SQuat& dst, const SQuat& src1, const SQuat& src2)
        {
            dst.x = src1.x - src2.x;
            dst.y = src1.y - src2.y;
            dst.z = src1.z - src2.z;
            dst.w = src1.w - src2.w;
        }

        // クオータニオンを乗算する
        // dst = src1 * src2
        static inline void Mul(SQuat& dst, const SQuat& src1, const SQuat& src2);

        // クオータニオンをスケーリングする
        // dst = fQ * src
        static void Scale(SQuat& dst, const SQuat& src, IZ_FLOAT fQ)
        {
            dst.x = fQ * src.x;
            dst.y = fQ * src.y;
            dst.z = fQ * src.z;
            dst.w = fQ * src.w;
        }

        // クオータニオンをスケーリング（除算）する
        // dst = (src.x / fQ, src.y / fQ, src.z / fQ, src.w / fQ)
        static void Div(SQuat& dst, const SQuat& src, IZ_FLOAT fQ)
        {
#if 0
            dst.x = src.x / fQ;
            dst.y = src.y / fQ;
            dst.z = src.z / fQ;
            dst.w = src.w / fQ;
#else
            Scale(dst, src, 1.0f / fQ);
#endif
        }

        // クオータニオンの大きさ(絶対値)を計算する
        static inline IZ_FLOAT Length(const SQuat& quat);

        // クオータニオンを正規化する
        static inline void Normalize(SQuat& dst, const SQuat& src);

        // 共役クオータニオンを求める
        static inline void Conjugate(SQuat& dst, const SQuat& src);

        // 逆クオータニオンを求める
        static inline void Inverse(SQuat& dst, const SQuat& src);

        // 球面線形補間
        static inline void Slerp(SQuat& dst, const SQuat& quat1, const SQuat& quat2, IZ_FLOAT t);

        static void Slerp(SVector4& dst, const SQuat& quat1, const SQuat& quat2, IZ_FLOAT t)
        {
            Slerp(
                *(reinterpret_cast<SQuat*>(&dst)),
                quat1, quat2,
                t);
        }

        // 角度と軸からクオータニオンを設定する
        static void SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, IZ_FLOAT fAxisX, IZ_FLOAT fAxisY, IZ_FLOAT fAxisZ)
        {
            SVector4 vAxis;
            vAxis.Set(fAxisX, fAxisY, fAxisZ, 0.0f);

            SetQuatFromRadAxis(quat, rad, vAxis);
        }

        // 角度と軸からクオータニオンを設定する
        static inline void SetQuatFromRadAxis(SQuat& quat, IZ_FLOAT rad, const SVector4& vAxis);

        // クオータニオンから行列を計算する
        static inline void MatrixFromQuat(SMatrix44& mtx, const SQuat& quat);

        // 行列からクオータニオンを計算する
        static inline void QuatFromMatrix(SQuat& quat, const SMatrix44& mtx);

        // オイラー角からクオータニオンを計算する
        static inline void QuatFromEuler(SQuat& quat, IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z);

        // 二つのベクトルv0,v1が与えられたときに
        // q  * v0 == v1 となるクオータニオンqを計算する
        static void RotationArc(SQuat& quat, const SVector4& from, const SVector4& to)
        {
            // 念のため
            SVector4 v0, v1;
            SVector4::Normalize(v0, from);
            SVector4::Normalize(v1, to);

#if 0
            SVector4 c;
            SVector4::Cross(c, v0, v1);

            IZ_FLOAT d = SVector4::Dot(v0, v1);
            IZ_FLOAT s = sqrtf(2.0f * (1.0f + d));

            quat.x = c.x / s;
            quat.y = c.y / s;
            quat.z = c.z / s;

            quat.w = s * 0.5f;
#else
            // http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm

            // angle = arcos(v1•v2 / | v1 || v2 | )
            // axis = norm(v1 x v2)
            // s = sin(angle / 2)
            // x = axis.x *s
            // y = axis.y *s
            // z = axis.z *s
            // w = cos(angle / 2)

            SVector4 axis;
            SVector4::Cross(axis, v0, v1);

            IZ_FLOAT cosine = SVector4::Dot(v0, v1);
            IZ_FLOAT angle = acosf(cosine);

            IZ_FLOAT s = sinf(angle * 0.5f);

            quat.x = axis.x * s;
            quat.y = axis.y * s;
            quat.z = axis.z * s;

            quat.w = cosf(angle * 0.5f);
#endif
        }

        // クオータニオンからオイラー角を計算する
        static void GetEuler(SVector4& angle, const SQuat& quat)
        {
            SMatrix44 mtx;
            MatrixFromQuat(mtx, quat);

            // TODO
            // MatrixFromQuatの結果はZXYの回転順番であることを常に保障する？
            SMatrix44::GetEulerFromMatrix(angle, mtx, E_MATH_ROTATION_ORDER_ZXY);
        }

        // オイラー角からクオータニオンを計算する
        static void QuatFromEuler(SQuat& quat, const SVector4& angle)
        {
            QuatFromEuler(
                quat,
                angle.x, angle.y, angle.z);
        }
    };
}   // namespace math
}    // namespace izanagi

#ifdef __USE_D3D_MATH__
    #include "math/dx9/MathQuaternion_DX9.h"
#else   // #ifdef __USE_D3D_MATH__
    #include "math/noplatform/MathQuaternion_NoPlatform.h"
#endif

#endif    // #if !defined(__IZANAGI__MATH_QUATERNION_H__)
