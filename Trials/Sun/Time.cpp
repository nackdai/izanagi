#include "Time.h"
#include "izMath.h"

#define _FLOOR_F2I_(f)  ((IZ_UINT)::floorf(f))

// Get JD(Julian Day) by UT(Universal Time).
void CTime::GetJDByUT(const SUniversalTime& ut, SLongTime& out)
{
    IZ_UINT year = ut.year;
    IZ_UINT month = ut.month;

    if (month <= 2)
    {
        year -= 1;
        month += 12;
    }

#if 1
    out.integer = _FLOOR_F2I_(365.25f * year) + _FLOOR_F2I_(year / 400.0f) - _FLOOR_F2I_(year / 100.0f)
        + _FLOOR_F2I_(30.59f * (month - 2)) + ut.day + 1721088;
    out.frac = 0.5f + (ut.hour + ut.minute / 60 + ut.second / 3600.0f) / 24.0f;

    if (out.frac >= 1.0f)
    {
        out.integer += 1;
        out.frac -= 1.0f;
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
    out.frac = 0.5f + day.hour / 24.0f;

    IZ_FLOAT jd = out.day + out.frac;
    if (jd < 2299160.5f)
    {
        out.frac -= b;
    }
#endif
}

// Get TJD(Truncated Julian Day) by JD(Julian Day).
IZ_FLOAT CTime::GetTJDByJD(const SLongTime& jd)
{
    IZ_UINT i = jd.integer - 2440000;
    IZ_FLOAT f = jd.frac - 0.5f;
    IZ_FLOAT tjd = i + f;
    return tjd;
}

// Get TJD(Truncated Julian Day) by UT(Universal Time).
IZ_FLOAT CTime::GetTJDByUT(const SUniversalTime& ut)
{
    SLongTime jd;
    GetJDByUT(ut, jd);
    IZ_FLOAT tjd = GetTJDByJD(jd);
    return tjd;
}

// Get Greenwich Sidereal Time by UT(Universal Time).
IZ_DOUBLE CTime::GetGSTByUT(const SUniversalTime& ut)
{
#if 1
    // NOTE
    // JD は必ず0時UTでなければいけない

    SUniversalTime tmp = ut;
    tmp.hour = 0;
    tmp.minute = 0;
    tmp.second = 0;

    SLongTime jd;
    GetJDByUT(tmp, jd);

    IZ_DOUBLE JD = jd.integer + jd.frac;
    IZ_DOUBLE T = (JD - 2451545.0) / 36525.0;

#if 1
    // 秒単位
    IZ_DOUBLE t = ut.hour * 3600.0 + ut.minute * 60 + ut.second;

    IZ_DOUBLE gst = 24110.54841 + 8640184.812866 * T + 0.093104 * T * T - 0.0000062 * T * T * T;
    gst += 1.00273790935 * t;
#else
    // 時間単位
    IZ_DOUBLE t = ut.hour + (IZ_DOUBLE)ut.minute / 24.0 + (IZ_DOUBLE)ut.second / (24.0 * 60.0);
    IZ_DOUBLE gst = 6.697374558 + 1.0027379093 * t + 2400.051336 * T + 0.000025862 * T * T;

    if (gst > 24.0)
    {
        gst = gst - ((IZ_INT)(gst / 24.0f)) * 24.0;
    }
#endif
#else
    SLongTime jd;
    GetJDByUT(ut, jd);

    IZ_DOUBLE JD = jd.integer + jd.frac;
    IZ_DOUBLE T = (JD - 2451545.0) / 36525.0;

    IZ_DOUBLE gst = 280.46061837 + 360.98564736629 * (JD - 2451545.0) + 0.0002879337 * T * T - T * T * T / 38710000;
#endif
    
    return gst;
}

// Get Greenwich Sidereal Time by UT(Universal Time).
IZ_FLOAT CTime::GetGSTByUTAsFloat(const SUniversalTime& ut)
{
    IZ_DOUBLE gst = GetGSTByUT(ut);
    return (IZ_FLOAT)gst;
}

// Get Local Sidereal Time by UT(Universal Time).
void CTime::GetLSTByUT(
    const SUniversalTime& ut,
    IZ_FLOAT longitude,
    SUniversalTime& lst)
{
    // NOTE
    // 東経が+、西経が-

    IZ_DOUBLE gst = GetGSTByUT(ut);

#if 0
    IZ_DOUBLE t = longitude / 15.0;
    gst += t;

    IZ_DOUBLE hour = gst;
    IZ_DOUBLE frac = izanagi::math::CMath::GetFrac(hour);

    IZ_DOUBLE minute = frac * 60.0;
    frac = izanagi::math::CMath::GetFrac(minute);

    IZ_DOUBLE second = frac * 60.0;
#else
    IZ_DOUBLE t = longitude / 15.0 * 3600.0;
    gst += t;

    t = gst / (24.0 * 60.0 * 60.0);
    IZ_DOUBLE frac = izanagi::math::CMath::GetFrac(t);

    IZ_DOUBLE hour = frac * 24.0;
    frac = izanagi::math::CMath::GetFrac(hour);

    IZ_DOUBLE minute = frac * 60.0;
    frac = izanagi::math::CMath::GetFrac(minute);

    IZ_DOUBLE second = frac * 60.0;
#endif

    lst.hour = (IZ_UINT8)hour;
    lst.minute = (IZ_UINT8)minute;
    lst.second = (IZ_UINT8)second;
}
