#if !defined(__EPHEMERIS_H__)
#define __EPHEMERIS_H__

#include "izMath.h"

struct SPolarCoord
{
    IZ_FLOAT radius;
    IZ_FLOAT longitude;
    IZ_FLOAT latitude;
};

class CEphemeris
{
private:
    CEphemeris();
    ~CEphemeris();

public:
    static void ConvertPolarToRectangular(
        const SPolarCoord& polar,
        izanagi::math::SVector& ortho);

    static void ConvertRectangularToPolar(
        const izanagi::math::SVector& ortho,
        SPolarCoord& polar);

    static void ConvertPolarToMatrix(
        const SPolarCoord& polar,
        izanagi::math::SMatrix& mtx);

    static void ConvertElipticToEquatorial(
        const SPolarCoord& eliptic, 
        izanagi::math::SVector& equatorial);

    static void ConvertElipticToEquatorial(
        const izanagi::math::SVector& eliptic, 
        izanagi::math::SVector& equatorial);

    static void ConvertEquatorialToHorizontal(
        const SPolarCoord& equatorial, 
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector& horizontal);

    static void ConvertEquatorialToHorizontal(
        const izanagi::math::SVector& equatorial,
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector& horizontal);

    static void ConvertElipticToHorizontal(
        const SPolarCoord& eliptic, 
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector& horizontal);

    static void ConvertElipticToHorizontal(
        const izanagi::math::SVector& eliptic, 
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector& horizontal);
};

#endif    // #if !defined(__EPHEMERIS_H__)