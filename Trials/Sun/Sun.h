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
    /** ���E�����物�o���擾.
     */
    static IZ_FLOAT GetElipticLongitudeByUniversalTime(
        const SUniversalTime& day,
        const SUniversalTime& veDay);

    /** ���E�����物�����W���擾.
     */
    static void GetElipticByUniversalTime(
        SPolarCoord& eliptic,
        const SUniversalTime& day,
        const SUniversalTime& veDay);

    /** ���E�����物�����W���擾.
     */
    static void GetElipticByUniversalTime(
        izanagi::math::SVector& eliptic,
        const SUniversalTime& day,
        const SUniversalTime& veDay);

    /** ���̔N�ɂ�����t���_����̌o�ߓ������物�o���擾.
     *
     * �t���_���O�̏ꍇ�͑O�N�̏t���_�̌o�ߓ���.
     */
    static IZ_FLOAT GetElipticLongitudeByElapsedDay(const SLongTime& day);

    /** ���̔N�ɂ�����t���_����̌o�ߓ������物�����W���擾.
     */
    static void GetElipticByElapsedDay(
        SPolarCoord& eliptic,
        const SLongTime& day);

    /** ���̔N�ɂ�����t���_����̌o�ߓ������物�����W���擾.
     */
    static void GetElipticByElapsedDay(
        izanagi::math::SVector& eliptic,
        const SLongTime& day);
};

#endif    // #if !defined(__EPHEMERIS_H__)