#include "Ephemeris.h"
#include "Time.h"

// �ɍ��W���璼�s���W�ɕϊ�.
void CEphemeris::ConvertPolarToRectangular(
    const SPolarCoord& polar,
    izanagi::math::SVector4& ortho)
{
    IZ_FLOAT latitude  = IZ_DEG2RAD(polar.latitude);
    IZ_FLOAT longitude = IZ_DEG2RAD(polar.longitude);

    IZ_FLOAT sinLat, cosLat;
    sinLat = izanagi::math::CMath::SinF(latitude);
    cosLat = izanagi::math::CMath::CosF(latitude);

    IZ_FLOAT sinLong, cosLong;
    sinLong = izanagi::math::CMath::SinF(longitude);
    cosLong = izanagi::math::CMath::CosF(longitude);

    ortho.x = polar.radius * cosLat * cosLong;
    ortho.y = polar.radius * sinLat;
    ortho.z = polar.radius * cosLat * sinLong;
    ortho.w = 0.0f;
}

// ���s���W����ɍ��W�ɕϊ�. 
void CEphemeris::ConvertRectangularToPolar(
    const izanagi::math::SVector4& ortho,
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

    polar.latitude  = IZ_RAD2DEG(polar.latitude);
    polar.longitude = IZ_RAD2DEG(polar.longitude);
}

// �ɍ��W�����]�s��ɕϊ�.
void CEphemeris::ConvertPolarToMatrix(
    const SPolarCoord& polar,
    izanagi::math::SMatrix44& mtx)
{
    IZ_FLOAT latitude  = IZ_DEG2RAD(polar.latitude);
    IZ_FLOAT longitude = IZ_DEG2RAD(polar.longitude);

    izanagi::math::SMatrix44 tmp;
    izanagi::math::SMatrix44::GetRotByZ(tmp, latitude);

    // NOTE
    // �^��(y��+�����j���猩���ꍇ => x-z����
    // z
    // |
    // |
    // +---->x 
    //
    // �ɍ��W���l����ꍇ�́Ax-z���ʂ͐��w�I�ɂQ�����ōl�����āA����ƁA����͂悭���� x-y���� �Ƃ݂Ȃ����Ƃ��ł���
    // ����ƁA��]�̕����� x->y �Ɍ����āA�܂蔽���v�܂��ƂȂ�B
    // �������A������W�n�̉�]�� z->x �Ɍ����āA�܂莞�v�܂��ƂȂ邽��
    // ��]�̕����ɖ�����������B
    // �����ŁA���K��������K�v������B

    // TODO
    // �{���́A�ɍ��W���E����W�n�ɍ��킹��K�v������

    izanagi::math::SMatrix44::GetRotByY(mtx, IZ_MATH_PI2 - longitude);

    izanagi::math::SMatrix44::Mul(mtx, tmp, mtx);
}

// �������W����ԓ����W�ɕϊ�.
void CEphemeris::ConvertElipticToEquatorial(
    const SPolarCoord& eliptic, 
    izanagi::math::SVector4& equatorial)
{
    izanagi::math::SVector4 coord;
    ConvertPolarToRectangular(eliptic, coord);

    ConvertElipticToEquatorial(coord, equatorial);
}

// �������W����ԓ����W�ɕϊ�.
void CEphemeris::ConvertElipticToEquatorial(
    const izanagi::math::SVector4& eliptic, 
    izanagi::math::SVector4& equatorial)
{
    izanagi::math::SMatrix44 mtx;
    izanagi::math::SMatrix44::GetRotByX(mtx, -IZ_DEG2RAD(23.43929f));

    izanagi::math::SMatrix44::ApplyXYZ(
        equatorial,
        eliptic,
        mtx);
}

// �ԓ����W����n�����W�ɕϊ�.
void CEphemeris::ConvertEquatorialToHorizontal(
    const SPolarCoord& equatorial, 
    IZ_FLOAT latitude,
    IZ_FLOAT hourAngle,
    izanagi::math::SVector4& horizontal)
{
    izanagi::math::SVector4 coord;
    ConvertPolarToRectangular(equatorial, coord);

    ConvertEquatorialToHorizontal(
        coord, 
        latitude, hourAngle,
        horizontal);
}

// �ԓ����W����n�����W�ɕϊ�.
void CEphemeris::ConvertEquatorialToHorizontal(
    const izanagi::math::SVector4& equatorial, 
    IZ_FLOAT latitude,
    IZ_FLOAT hourAngle,
    izanagi::math::SVector4& horizontal)
{
    IZ_FLOAT lat = IZ_DEG2RAD(latitude);
    IZ_FLOAT hAngle = IZ_DEG2RAD(hourAngle);

    izanagi::math::SMatrix44 mtxLong;
    izanagi::math::SMatrix44::GetRotByY(mtxLong, -hAngle);

    izanagi::math::SMatrix44 mtxLat;
    izanagi::math::SMatrix44::GetRotByY(mtxLong, IZ_MATH_PI1_2 - lat);

    izanagi::math::SMatrix44::ApplyXYZ(horizontal, equatorial, mtxLong);
    izanagi::math::SMatrix44::ApplyXYZ(horizontal, horizontal, mtxLat);
}

// �������W����n�����W�ɕϊ�.
void CEphemeris::ConvertElipticToHorizontal(
    const SPolarCoord& eliptic,
    IZ_FLOAT latitude,
    IZ_FLOAT hourAngle,
    izanagi::math::SVector4& horizontal)
{
    izanagi::math::SVector4 equatorial;
    ConvertElipticToEquatorial(eliptic, equatorial);

    ConvertEquatorialToHorizontal(
        equatorial, 
        latitude, hourAngle,
        horizontal);
}

// �������W����n�����W�ɕϊ�.
void CEphemeris::ConvertElipticToHorizontal(
    const izanagi::math::SVector4& eliptic, 
    IZ_FLOAT latitude,
    IZ_FLOAT hourAngle,
    izanagi::math::SVector4& horizontal)
{
    izanagi::math::SVector4 equatorial;
    ConvertElipticToEquatorial(eliptic, equatorial);

    ConvertEquatorialToHorizontal(
        equatorial, 
        latitude, hourAngle,
        horizontal);
}

// ���p���擾.
IZ_FLOAT CEphemeris::GetHourAngle(
    const SUniversalTime& ut,
    IZ_FLOAT longitude,
    IZ_FLOAT rightAscension)
{
    SUniversalTime lst;
    CTime::GetLSTByUT(ut, longitude, lst);

    IZ_FLOAT hour = lst.hour + lst.minute / 60.0f + lst.second / 3600.0f;
    IZ_FLOAT angle = hour * 15.0f;

    angle -= rightAscension;

    return angle;
}

// �������W���玞�p���擾.
IZ_FLOAT CEphemeris::GetHourAngleByEliptic(
    const SUniversalTime& ut,
    IZ_FLOAT longitude,
    const SPolarCoord& eliptic)
{
    izanagi::math::SVector4 equatorial;
    ConvertElipticToEquatorial(eliptic, equatorial);

    SPolarCoord polarEquatorial;
    ConvertRectangularToPolar(equatorial, polarEquatorial);

    IZ_FLOAT hourAngle = GetHourAngle(ut, longitude, polarEquatorial.longitude);
    return hourAngle;
}

// �������W����n�����W�ɕϊ�.
void CEphemeris::ConvertElipticToHorizontal(
    const SPolarCoord& eliptic,
    const SUniversalTime& ut,
    const SPolarCoord& polar,
    izanagi::math::SVector4& horizontal)
{
    IZ_FLOAT hourAngle = GetHourAngleByEliptic(ut, polar.longitude, eliptic);

    ConvertElipticToHorizontal(
        eliptic,
        polar.latitude,
        hourAngle,
        horizontal);
}
