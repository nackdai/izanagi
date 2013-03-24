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
    // �^��(y��+�����j���猩���ꍇ => x-z����
    // z
    // |
    // |
    // +---->x 
    //
    // �ɍ��W���l����ꍇ�́Ax-z���ʂ͐��w�I�ɂQ�����ōl�����āA����ƁA����͂悭���� x-y���� �Ƃ݂Ȃ����Ƃ��ł���
    // ����ƁA��]�̕����� x->y �Ɍ����āA�܂蔽���v�܂��ƂȂ�B
    // �������A�E����W�n�̉�]�� z->x �Ɍ����āA�܂莞�v�܂��ƂȂ邽��
    // ��]�̕����ɖ�����������B
    // �����ŁA���K��������K�v������B

    // TODO
    // �{���́A�ɍ��W���E����W�n�ɍ��킹��K�v������

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
