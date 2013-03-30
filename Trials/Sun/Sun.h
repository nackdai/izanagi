#if !defined(__SUN_H__)
#define __SUN_H__

#include "izMath.h"
#include "Ephemeris.h"

class CSun
{
public:
    static const IZ_FLOAT DEFAULT_SOLAR_YEAR;

private:
    static IZ_FLOAT g_SloarYear;
    static IZ_FLOAT g_AnglePerSolarYear;

private:
    CSun();
    ~CSun();

public:
    static IZ_FLOAT GetSolarYear();
    static void SetSolarYear(IZ_FLOAT day);

private:
    static IZ_FLOAT GetAnglePerSolarYear();

public:
    /** 世界時から黄経を取得.
     */
    static IZ_FLOAT GetElipticLongitudeByUniversalTime(
        const SUniversalTime& day,
        const SUniversalTime& veDay);

    /** 世界時から黄道座標を取得.
     */
    static void GetElipticByUniversalTime(
        SPolarCoord& eliptic,
        const SUniversalTime& day,
        const SUniversalTime& veDay);

    /** 世界時から黄道座標を取得.
     */
    static void GetElipticByUniversalTime(
        izanagi::math::SVector& eliptic,
        const SUniversalTime& day,
        const SUniversalTime& veDay);

    /** その年における春分点からの経過日数から黄経を取得.
     *
     * 春分点より前の場合は前年の春分点の経過日数.
     */
    static IZ_FLOAT GetElipticLongitudeByElapsedDay(const SLongTime& day);

    /** その年における春分点からの経過日数から黄道座標を取得.
     */
    static void GetElipticByElapsedDay(
        SPolarCoord& eliptic,
        const SLongTime& day);

    /** その年における春分点からの経過日数から黄道座標を取得.
     */
    static void GetElipticByElapsedDay(
        izanagi::math::SVector& eliptic,
        const SLongTime& day);
};

#endif    // #if !defined(__EPHEMERIS_H__)