#include "Sun.h"

const IZ_FLOAT CSun::DEFAULT_SOLAR_YEAR = 365.242194f;

IZ_FLOAT CSun::g_SloarYear = DEFAULT_SOLAR_YEAR;
IZ_FLOAT CSun::g_AnglePerSolarYear = 360.0f / DEFAULT_SOLAR_YEAR;

IZ_FLOAT CSun::GetSolarYear()
{
    return g_SloarYear;
}

void CSun::SetSolarYear(IZ_FLOAT day)
{
    g_SloarYear = day;
    if (g_SloarYear <= 0.0f)
    {
        g_SloarYear = DEFAULT_SOLAR_YEAR;
    }

    g_AnglePerSolarYear = IZ_MATH_PI2 / g_SloarYear;
}

IZ_FLOAT CSun::GetAnglePerSolarYear()
{
    return g_AnglePerSolarYear;
}

// 世界時から黄経を取得.
IZ_FLOAT CSun::GetElipticLongitudeByUniversalTime(
    const SUniversalTime& day,
    const SUniversalTime& veDay)
{
    SUniversalTime base;
    {
        base.year = veDay.year;
        base.month = veDay.month;
        base.day = veDay.day;
        base.hour = veDay.hour;
        base.minute = veDay.minute;
        base.second = veDay.second;
    }

    SLongTime baseJD;
    CTime::GetJDByUT(base, baseJD);

    SLongTime targetJD;
    CTime::GetJDByUT(day, targetJD);

    SLongTime elapsed;
    {
        elapsed.integer = targetJD.integer - baseJD.integer;
        elapsed.frac = targetJD.frac - baseJD.frac;

        if (elapsed.frac < 0.0f)
        {
            elapsed.integer -= 1;
            elapsed.frac += 1.0f;
        }
    }

    IZ_FLOAT longitude = GetElipticLongitudeByElapsedDay(elapsed);

    return longitude;
}

// 世界時から黄道座標を取得.
void CSun::GetElipticByUniversalTime(
    SPolarCoord& eliptic,
    const SUniversalTime& day,
    const SUniversalTime& veDay)
{
    IZ_FLOAT longitude = GetElipticLongitudeByUniversalTime(day, veDay);

    eliptic.radius = 1.0f;
    eliptic.latitude = 0.0f;
    eliptic.longitude = longitude;
}

// 世界時から黄道座標を取得.
void CSun::GetElipticByUniversalTime(
    izanagi::math::SVector& eliptic,
    const SUniversalTime& day,
    const SUniversalTime& veDay)
{
    SPolarCoord polar;
    GetElipticByUniversalTime(polar, day, veDay);

    CEphemeris::ConvertPolarToRectangular(polar, eliptic);
}

// その年における春分点からの経過日数から黄経を取得.
IZ_FLOAT CSun::GetElipticLongitudeByElapsedDay(const SLongTime& day)
{
    IZ_FLOAT anglePerSolarYear = GetAnglePerSolarYear();

    IZ_FLOAT angle = anglePerSolarYear * day.integer;
    angle += anglePerSolarYear * day.frac;

    return angle;
}

// その年における春分点からの経過日数から黄道座標を取得.
void CSun::GetElipticByElapsedDay(
    SPolarCoord& eliptic,
    const SLongTime& day)
{
    IZ_FLOAT longitude = GetElipticLongitudeByElapsedDay(day);

    eliptic.radius = 1.0f;
    eliptic.latitude = 0.0f;
    eliptic.longitude = longitude;
}

// その年における春分点からの経過日数から黄道座標を取得.
void CSun::GetElipticByElapsedDay(
    izanagi::math::SVector& eliptic,
    const SLongTime& day)
{
    SPolarCoord polar;
    GetElipticByElapsedDay(polar, day);

    CEphemeris::ConvertPolarToRectangular(polar, eliptic);
}
