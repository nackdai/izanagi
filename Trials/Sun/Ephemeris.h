#if !defined(__EPHEMERIS_H__)
#define __EPHEMERIS_H__

#include "izMath.h"
#include "Time.h"

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
    /** �ɍ��W���璼�s���W�ɕϊ�.
     */
    static void ConvertPolarToRectangular(
        const SPolarCoord& polar,
        izanagi::math::SVector4& ortho);

    /** ���s���W����ɍ��W�ɕϊ�. 
     */
    static void ConvertRectangularToPolar(
        const izanagi::math::SVector4& ortho,
        SPolarCoord& polar);

    /** �ɍ��W�����]�s��ɕϊ�.
     */
    static void ConvertPolarToMatrix(
        const SPolarCoord& polar,
        izanagi::math::SMatrix44& mtx);

    /** �������W����ԓ����W�ɕϊ�.
     */
    static void ConvertElipticToEquatorial(
        const SPolarCoord& eliptic, 
        izanagi::math::SVector4& equatorial);

    /** �������W����ԓ����W�ɕϊ�.
     */
    static void ConvertElipticToEquatorial(
        const izanagi::math::SVector4& eliptic, 
        izanagi::math::SVector4& equatorial);

    /** �ԓ����W����n�����W�ɕϊ�.
     */
    static void ConvertEquatorialToHorizontal(
        const SPolarCoord& equatorial, 
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector4& horizontal);

    /** �ԓ����W����n�����W�ɕϊ�.
     */
    static void ConvertEquatorialToHorizontal(
        const izanagi::math::SVector4& equatorial,
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector4& horizontal);

    /** �������W����n�����W�ɕϊ�.
     */
    static void ConvertElipticToHorizontal(
        const SPolarCoord& eliptic, 
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector4& horizontal);

    /** �������W����n�����W�ɕϊ�.
     */
    static void ConvertElipticToHorizontal(
        const izanagi::math::SVector4& eliptic,
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector4& horizontal);

public:
    /** ���p���擾.
     */
    static IZ_FLOAT GetHourAngle(
        const SUniversalTime& ut,
        IZ_FLOAT longitude,
        IZ_FLOAT rightAscension);

    /** �������W���玞�p���擾.
     */
    static IZ_FLOAT GetHourAngleByEliptic(
        const SUniversalTime& ut,
        IZ_FLOAT longitude,
        const SPolarCoord& eliptic);

    /** �������W����n�����W�ɕϊ�.
     */
    static void ConvertElipticToHorizontal(
        const SPolarCoord& eliptic,
        const SUniversalTime& ut,
        const SPolarCoord& polar,
        izanagi::math::SVector4& horizontal);
};

#endif    // #if !defined(__EPHEMERIS_H__)