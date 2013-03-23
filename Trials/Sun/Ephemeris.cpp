#include "Ephemeris.h"

void CEphemeris::ConvertPolarToRectangular(
    const SPolarCoord& polar,
    izanagi::math::SVector& ortho)
{
    IZ_FLOAT sinLat, cosLat;
#if 0
    izanagi::math::CMath::GetSinCosF(
        polar.latitude,
        sinLat, cosLat);
#else
    sinLat = izanagi::math::CMath::SinF(polar.latitude);
    cosLat = izanagi::math::CMath::CosF(polar.latitude);
#endif

    IZ_FLOAT sinLong, cosLong;
#if 0
    izanagi::math::CMath::GetSinCosF(
        polar.longitude,
        sinLong, cosLong);
#else
    sinLong = izanagi::math::CMath::SinF(polar.longitude);
    cosLong = izanagi::math::CMath::CosF(polar.longitude);
#endif

    ortho.x = polar.radius * cosLat * cosLong;
    ortho.y = polar.radius * sinLat;
    ortho.z = polar.radius * cosLat * sinLong;
    ortho.w = 0.0f;
}

void CEphemeris::ConvertRectangularToPolar(
    const izanagi::math::SVector& ortho,
    SPolarCoord& polar)
{
    // TODO
    // ゼロチェック

    polar.longitude = izanagi::math::CMath::ArcTan2F(ortho.z, ortho.x);

    IZ_FLOAT sinLong = izanagi::math::CMath::SinF(polar.longitude);

    IZ_FLOAT z = ortho.z / sinLong;
    polar.latitude = izanagi::math::CMath::ArcTan2F(ortho.y, z);

    IZ_FLOAT sinLat = izanagi::math::CMath::SinF(polar.latitude);

    polar.radius = ortho.y / sinLat;
}

void CEphemeris::ConvertPolarToMatrix(
    const SPolarCoord& polar,
    izanagi::math::SMatrix& mtx)
{
    izanagi::math::SMatrix tmp;
    izanagi::math::SMatrix::GetRotByZ(tmp, polar.latitude);

    izanagi::math::SMatrix::GetRotByY(mtx, polar.longitude);

    izanagi::math::SMatrix::Mul(mtx, tmp, mtx);
}

void CEphemeris::ConvertElipticToEquatorial(
    const SPolarCoord& eliptic, 
    izanagi::math::SVector& equatorial)
{
    izanagi::math::SVector coord;
    ConvertPolarToRectangular(eliptic, coord);

    ConvertElipticToEquatorial(coord, equatorial);
}

void CEphemeris::ConvertElipticToEquatorial(
    const izanagi::math::SVector& eliptic, 
    izanagi::math::SVector& equatorial)
{
    izanagi::math::SMatrix mtx;
    izanagi::math::SMatrix::GetRotByX(mtx, -IZ_DEG2RAD(23.43929f));

    izanagi::math::SMatrix::ApplyXYZ(
        equatorial,
        eliptic,
        mtx);
}

void CEphemeris::ConvertEquatorialToHorizontal(
    const SPolarCoord& equatorial, 
    IZ_FLOAT latitude,
    IZ_FLOAT hourAngle,
    izanagi::math::SVector& horizontal)
{
    izanagi::math::SVector coord;
    ConvertPolarToRectangular(equatorial, coord);

    ConvertEquatorialToHorizontal(
        coord, 
        latitude, hourAngle,
        horizontal);
}

void CEphemeris::ConvertEquatorialToHorizontal(
    const izanagi::math::SVector& equatorial, 
    IZ_FLOAT latitude,
    IZ_FLOAT hourAngle,
    izanagi::math::SVector& horizontal)
{
    izanagi::math::SMatrix mtxLong;
    izanagi::math::SMatrix::GetRotByY(mtxLong, -hourAngle);

    izanagi::math::SMatrix mtxLat;
    izanagi::math::SMatrix::GetRotByY(mtxLong, IZ_MATH_PI1_2 - latitude);

    izanagi::math::SMatrix::ApplyXYZ(horizontal, equatorial, mtxLong);
    izanagi::math::SMatrix::ApplyXYZ(horizontal, horizontal, mtxLat);
}

void CEphemeris::ConvertElipticToHorizontal(
    const SPolarCoord& eliptic,
    IZ_FLOAT latitude,
    IZ_FLOAT hourAngle,
    izanagi::math::SVector& horizontal)
{
    izanagi::math::SVector equatorial;
    ConvertElipticToEquatorial(eliptic, equatorial);

    ConvertEquatorialToHorizontal(
        equatorial, 
        latitude, hourAngle,
        horizontal);
}

void CEphemeris::ConvertElipticToHorizontal(
    const izanagi::math::SVector& eliptic, 
    IZ_FLOAT latitude,
    IZ_FLOAT hourAngle,
    izanagi::math::SVector& horizontal)
{
    izanagi::math::SVector equatorial;
    ConvertElipticToEquatorial(eliptic, equatorial);

    ConvertEquatorialToHorizontal(
        equatorial, 
        latitude, hourAngle,
        horizontal);
}
