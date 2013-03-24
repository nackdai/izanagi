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
    polar.longitude = izanagi::math::CMath::ArcTan2F(ortho.z, ortho.x);

    IZ_FLOAT sinLong = izanagi::math::CMath::SinF(polar.longitude);

    if (sinLong != 0.0f)
    {
        IZ_FLOAT z = ortho.z / sinLong;
        polar.latitude = izanagi::math::CMath::ArcTan2F(ortho.y, z);
    }
    else
    {
        IZ_FLOAT cosLong = izanagi::math::CMath::CosF(polar.longitude);
        IZ_FLOAT x = ortho.x / cosLong;
        polar.latitude = izanagi::math::CMath::ArcTan2F(ortho.y, x);
    }

    if (ortho.y != 0.0f)
    {
        IZ_FLOAT sinLat = izanagi::math::CMath::SinF(polar.latitude);
        polar.radius = ortho.y / sinLat;
    }
    else
    {
        IZ_FLOAT cosLat = izanagi::math::CMath::CosF(polar.latitude);

        if (ortho.z != 0.0f)
        {
            polar.radius = ortho.z / (cosLat * sinLong);
        }
        else
        {
            IZ_FLOAT cosLong = izanagi::math::CMath::CosF(polar.longitude);
            polar.radius = ortho.x / (cosLat * cosLong);
        }
    }
}

void CEphemeris::ConvertPolarToMatrix(
    const SPolarCoord& polar,
    izanagi::math::SMatrix& mtx)
{
    izanagi::math::SMatrix tmp;
    izanagi::math::SMatrix::GetRotByZ(tmp, polar.latitude);

    // NOTE
    // 真上(y軸+方向）から見た場合 => x-z平面
    // z
    // |
    // |
    // +---->x 
    //
    // 極座標を考える場合は、x-z平面は数学的に２次元で考えられて、すると、それはよくある x-y平面 とみなすことができる
    // すると、回転の方向は x->y に向けて、つまり反時計まわりとなる。
    // しかし、右手座標系の回転は z->x に向けて、つまり時計まわりとなるため
    // 回転の方向に矛盾が生じる。
    // そこで、正規化をする必要がある。

    // TODO
    // 本来は、極座標を右手座標系に合わせる必要がある

    IZ_FLOAT longitude = IZ_MATH_PI2 - polar.longitude;

    izanagi::math::SMatrix::GetRotByY(mtx, longitude);

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
