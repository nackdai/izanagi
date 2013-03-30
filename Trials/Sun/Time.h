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

struct SLongTime
{
    IZ_UINT integer;
    IZ_FLOAT frac;
};

class CTime
{
private:
    CTime();
    ~CTime();

public:
    /** Get JD(Julian Day) by UT(Universal Time).
     */
    static void GetJDByUT(const SUniversalTime& ut, SLongTime& out);

    /** Get TJD(Truncated Julian Day) by JD(Julian Day).
     */
    static IZ_FLOAT GetTJDByJD(const SLongTime& jd);

    /** Get TJD(Truncated Julian Day) by UT(Universal Time).
     */
    static IZ_FLOAT GetTJDByUT(const SUniversalTime& day);

    /** Get Greenwich Sidereal Time by UT(Universal Time).
     */
    static IZ_DOUBLE GetGSTByUT(const SUniversalTime& ut);

    /** Get Greenwich Sidereal Time by UT(Universal Time).
     */
    static IZ_FLOAT GetGSTByUTAsFloat(const SUniversalTime& ut);

    /** Get Local Sidereal Time by UT(Universal Time).
     */
    static void GetLSTByUT(
        const SUniversalTime& ut,
        IZ_FLOAT longitude,
        SUniversalTime& lst);
};

#endif    // #if !defined(__SUN_H__)