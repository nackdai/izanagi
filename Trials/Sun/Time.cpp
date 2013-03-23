#include "Time.h"

#define _FLOOR_F2I_(f)  ((IZ_UINT)::floorf(f))

// Get JD(Julian Day) by UT(Universal Time).
void CTime::GetJDByUT(const SUniversalTime& day, SJulianDay& out)
{
    IZ_UINT year = day.year;
    IZ_UINT month = day.month;

    if (month <= 2)
    {
        year -= 1;
        month += 12;
    }

#if 1
    out.day = _FLOOR_F2I_(365.25f * year) + _FLOOR_F2I_(year / 400.0f) - _FLOOR_F2I_(year / 100.0f)
        + _FLOOR_F2I_(30.59f * (month - 2)) + day.day + 1721088;
    out.time = 0.5f + (day.hour + day.minute / 60 + day.second / 3600.0f) / 24.0f;

    if (out.time >= 1.0f)
    {
        out.day += 1;
        out.time -= 1.0f;
    }
#else
    IZ_UINT a = _FLOOR_F2I_(year / 100.0f);
    IZ_UINT b = 0;

    if (day.year >= 1582
        && day.month >= 10
        && day.day >= 15)
    {
        b = 2.0f - a + _FLOOR_F2I_(a / 4.0f);
    }

    out.day = _FLOOR_F2I_(365.25f * year) + _FLOOR_F2I_(30.6001f * (month + 1.0f)) + day.day + 1720994 + b;
    out.time = 0.5f + day.hour / 24.0f;

    IZ_FLOAT jd = out.day + out.time;
    if (jd < 2299160.5f)
    {
        out.day -= b;
    }
#endif
}

// Get TJD(Truncated Julian Day) by JD(Julian Day).
IZ_FLOAT CTime::GetTJDByJD(const SJulianDay& jd)
{
    IZ_UINT i = jd.day - 2440000;
    IZ_FLOAT f = jd.time - 0.5f;
    IZ_FLOAT tjd = i + f;
    return tjd;
}

// Get TJD(Truncated Julian Day) by UT(Universal Time).
IZ_FLOAT CTime::GetTJDByUT(const SUniversalTime& day)
{
    SJulianDay jd;
    GetJDByUT(day, jd);
    IZ_FLOAT tjd = GetTJDByJD(jd);
    return tjd;
}

// Get Greenwich Sidereal Time by UT(Universal Time).
IZ_FLOAT CTime::GetGreenwichSiderealTimeByUT(const SUniversalTime& day)
{
    IZ_FLOAT tjd = GetTJDByUT(day);
    return GetGreenwichSiderealTimeByTJD(tjd);
}

// Get Greenwich Sidereal Time by JD(Julian Day).
IZ_FLOAT CTime::GetGreenwichSiderealTimeByJD(const SJulianDay& jd)
{
    IZ_FLOAT tjd = GetTJDByJD(jd);
    return GetGreenwichSiderealTimeByTJD(tjd);
}

// Get Greenwich Sidereal Time by TJD(Truncated Julian Day).
IZ_FLOAT CTime::GetGreenwichSiderealTimeByTJD(IZ_FLOAT tjd)
{
    IZ_FLOAT thetaG = 24.0f * (0.671262f + 1.0027379094f * tjd);
    return thetaG;
}

// Get Local Sidereal Time by Greenwich Sidereal Time.
IZ_FLOAT CTime::GetLocalSiderealByGreenwichSiderealTime(IZ_FLOAT time, IZ_FLOAT longitude)
{
    IZ_FLOAT ret = time - longitude / 15.0f;
    if (ret < 0.0f)
    {
        time += 24.0f;
    }
    return ret;
}

// Get Local Sidereal Time by UT(Universal Time).
IZ_FLOAT CTime::GetLocalSiderealTimeByUT(const SUniversalTime& day, IZ_FLOAT longitude)
{
    IZ_FLOAT thetaG = GetGreenwichSiderealTimeByUT(day);
    return GetLocalSiderealByGreenwichSiderealTime(thetaG, longitude);
}

// Get Local Sidereal Time by JD(Julian Day).
IZ_FLOAT CTime::GetLocalSiderealTimeByJD(const SJulianDay& jd, IZ_FLOAT longitude)
{
    IZ_FLOAT thetaG = GetGreenwichSiderealTimeByJD(jd);
    return GetLocalSiderealByGreenwichSiderealTime(thetaG, longitude);
}

// Get Local Sidereal Time by TJD(Truncated Julian Day).
IZ_FLOAT CTime::GetLocalSiderealTimeByTJD(IZ_FLOAT tjd, IZ_FLOAT longitude)
{
    IZ_FLOAT thetaG = GetGreenwichSiderealTimeByTJD(tjd);
    return GetLocalSiderealByGreenwichSiderealTime(thetaG, longitude);
}
