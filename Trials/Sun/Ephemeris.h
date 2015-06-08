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
    /** 極座標から直行座標に変換.
     */
    static void ConvertPolarToRectangular(
        const SPolarCoord& polar,
        izanagi::math::SVector4& ortho);

    /** 直行座標から極座標に変換. 
     */
    static void ConvertRectangularToPolar(
        const izanagi::math::SVector4& ortho,
        SPolarCoord& polar);

    /** 極座標から回転行列に変換.
     */
    static void ConvertPolarToMatrix(
        const SPolarCoord& polar,
        izanagi::math::SMatrix44& mtx);

    /** 黄道座標から赤道座標に変換.
     */
    static void ConvertElipticToEquatorial(
        const SPolarCoord& eliptic, 
        izanagi::math::SVector4& equatorial);

    /** 黄道座標から赤道座標に変換.
     */
    static void ConvertElipticToEquatorial(
        const izanagi::math::SVector4& eliptic, 
        izanagi::math::SVector4& equatorial);

    /** 赤道座標から地平座標に変換.
     */
    static void ConvertEquatorialToHorizontal(
        const SPolarCoord& equatorial, 
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector4& horizontal);

    /** 赤道座標から地平座標に変換.
     */
    static void ConvertEquatorialToHorizontal(
        const izanagi::math::SVector4& equatorial,
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector4& horizontal);

    /** 黄道座標から地平座標に変換.
     */
    static void ConvertElipticToHorizontal(
        const SPolarCoord& eliptic, 
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector4& horizontal);

    /** 黄道座標から地平座標に変換.
     */
    static void ConvertElipticToHorizontal(
        const izanagi::math::SVector4& eliptic,
        IZ_FLOAT latitude,
        IZ_FLOAT hourAngle,
        izanagi::math::SVector4& horizontal);

public:
    /** 時角を取得.
     */
    static IZ_FLOAT GetHourAngle(
        const SUniversalTime& ut,
        IZ_FLOAT longitude,
        IZ_FLOAT rightAscension);

    /** 黄道座標から時角を取得.
     */
    static IZ_FLOAT GetHourAngleByEliptic(
        const SUniversalTime& ut,
        IZ_FLOAT longitude,
        const SPolarCoord& eliptic);

    /** 黄道座標から地平座標に変換.
     */
    static void ConvertElipticToHorizontal(
        const SPolarCoord& eliptic,
        const SUniversalTime& ut,
        const SPolarCoord& polar,
        izanagi::math::SVector4& horizontal);
};

#endif    // #if !defined(__EPHEMERIS_H__)