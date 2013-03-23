#if !defined(__TIME_H__)
#define __TIME_H__

#include "izDefs.h"

struct SUniversalTime
{
    IZ_UINT year;
    IZ_UINT8 month;
    IZ_UINT8 day;
    IZ_UINT8 hour;
    IZ_UINT8 minute;
    IZ_UINT8 second;
    IZ_UINT8 padding[3];
};

struct SJulianDay
{
    IZ_UINT day;
    IZ_FLOAT time;
};

class CTime
{
private:
    CTime();
    ~CTime();

public:
    /** Get JD(Julian Day) by UT(Universal Time).
     */
    static void GetJDByUT(const SUniversalTime& day, SJulianDay& out);

    /** Get TJD(Truncated Julian Day) by JD(Julian Day).
     */
    static IZ_FLOAT GetTJDByJD(const SJulianDay& jd);

    /** Get TJD(Truncated Julian Day) by UT(Universal Time).
     */
    static IZ_FLOAT GetTJDByUT(const SUniversalTime& day);

    /** Get Greenwich Sidereal Time by UT(Universal Time).
     */
    static IZ_FLOAT GetGreenwichSiderealTimeByUT(const SUniversalTime& day);

    /** Get Greenwich Sidereal Time by JD(Julian Day).
     */
    static IZ_FLOAT GetGreenwichSiderealTimeByJD(const SJulianDay& jd);

    /** Get Greenwich Sidereal Time by TJD(Truncated Julian Day).
     */
    static IZ_FLOAT GetGreenwichSiderealTimeByTJD(IZ_FLOAT tjd);

    /** Get Local Sidereal Time by Greenwich Sidereal Time.
     */
    static IZ_FLOAT GetLocalSiderealByGreenwichSiderealTime(IZ_FLOAT time, IZ_FLOAT longitude);

    /** Get Local Sidereal Time by UT(Universal Time).
     */
    static IZ_FLOAT GetLocalSiderealTimeByUT(const SUniversalTime& day, IZ_FLOAT longitude);

    /** Get Local Sidereal Time by JD(Julian Day).
     */
    static IZ_FLOAT GetLocalSiderealTimeByJD(const SJulianDay& jd, IZ_FLOAT longitude);

    /** Get Local Sidereal Time by TJD(Truncated Julian Day).
     */
    static IZ_FLOAT GetLocalSiderealTimeByTJD(IZ_FLOAT tjd, IZ_FLOAT longitude);
};

#endif    // #if !defined(__SUN_H__)