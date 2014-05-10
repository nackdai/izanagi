#include "math/MathCurve.h"
#include "math/MathMatrix.h"

namespace izanagi {
namespace math {
    /** ベジェ曲線.
     */
    IZ_FLOAT CMathCurve::ComputeBezier(
        IZ_FLOAT t,
        IZ_FLOAT x0,
        IZ_FLOAT x1,
        IZ_FLOAT x2,
        IZ_FLOAT x3)
    {
        IZ_FLOAT s = 1.0f - t;
        IZ_FLOAT s2 = s * s;
        IZ_FLOAT s3 = s2 * s;

        IZ_FLOAT t2 = t * t;
        IZ_FLOAT t3 = t2 * t;

        // NOTE
        // P(t) = (1-t)^3 * B0 + 3t(1-t)^2 * B1 + 3t^2(1-t) * B2 + t^3 * B3

        IZ_FLOAT ret = s3 * x0
            + 3.0f * t * s2 * x1
            + 3.0f * t2 * s * x2
            + t3 * x3;

        return ret;
    }

    /** ベジェ曲線.
     */
    IZ_FLOAT CMathCurve::ComputeBezier(
        IZ_FLOAT t,
        const SVector& p)
    {
        return ComputeBezier(t, p.x, p.y, p.z, p.w);
    }

    /** Hermite曲線.
     */
    IZ_FLOAT CMathCurve::ComputeHermite(
        IZ_FLOAT t,
        IZ_FLOAT x0,
        IZ_FLOAT x1,
        IZ_FLOAT v0,
        IZ_FLOAT v1)
    {
        return ComputeHermite(t, CVector(x0, x1, v0, v1));
    }

    /** Hermite曲線.
     */
    IZ_FLOAT CMathCurve::ComputeHermite(
        IZ_FLOAT t,
        const SVector& p)
    {
        CVector tv(t * t * t, t * t, t, 1.0f);

        static const SMatrix mtxHermite = {
             2.0f, -2.0f,  1.0f,  1.0f,
            -3.0f,  3.0f, -2.0f, -1.0f,
             0.0f,  0.0f,  1.0f,  0.0f,
             1.0f,  0.0f,  0.0f,  0.0f,
        };

        // NOTE
        //                         | 2 -2  1  1 | |x0|
        //                         |-3  3 -2 -1 | |x1|
        // P(t) = [t^3, t^2, t, 1] | 0  0  1  0 | |v0|
        //                         | 1  0  0  0 | |v1|

        SMatrix::Apply(tv, tv, mtxHermite);
        IZ_FLOAT ret = SVector::Dot(tv, p);

        return ret;
    }

    /** Catmull-Rom曲線.
     */
    IZ_FLOAT CMathCurve::ComputeCatmullRom(
        IZ_FLOAT t,
        IZ_FLOAT x0,
        IZ_FLOAT x1,
        IZ_FLOAT x2,
        IZ_FLOAT x3)
    {
        IZ_FLOAT v0 = 0.5f * (x2 - x0);
        IZ_FLOAT v1 = 0.5f * (x3 - x1);

        return ComputeHermite(t, x0, x1, v0, v1);
    }

    /** Catmull-Rom曲線.
     */
    IZ_FLOAT CMathCurve::ComputeCatmullRom(
        IZ_FLOAT t,
        const SVector& p)
    {
        return ComputeCatmullRom(t, p.x, p.y, p.z, p.w);
    }
}   // namespace math {
}   // namespace izanagi
