#include "izMath.h"
#include "izSystem.h"

void SimpleTest()
{
    IZ_FLOAT zero = 0.0f;
    IZ_FLOAT one = 1.0f;

    izanagi::math::CFloat16 zeroF16(zero);
    izanagi::math::CFloat16 oneF16(one);

    IZ_FLOAT tmp_0 = zeroF16;
    IZ_FLOAT tmp_1= oneF16;

    IZ_ASSERT(tmp_0 == zero);
    IZ_ASSERT(tmp_1 == one);
}

void FloatTest()
{
    IZ_FLOAT f0 = 0.5f;
    IZ_FLOAT f1 = 5.25f;
    IZ_FLOAT f2 = 19.75f;

    izanagi::math::CFloat16 h0(f0);
    izanagi::math::CFloat16 h1(f1);
    izanagi::math::CFloat16 h2(f2);

    IZ_FLOAT t0 = h0;
    IZ_FLOAT t1 = h1;
    IZ_FLOAT t2 = h2;

    IZ_ASSERT(t0 == f0);
    IZ_ASSERT(t1 == f1);
    IZ_ASSERT(t2 == f2);
}

void RandomTest()
{
    for (IZ_UINT i = 0; i < 10; i++)
    {
        IZ_FLOAT value = izanagi::math::CMathRand::GetRandFloat();

        izanagi::math::CFloat16 f16(value);

        IZ_FLOAT tmp = f16;

        IZ_PRINTF("(%d)=======\n", i);
        IZ_PRINTF("[%f] / [%f]\n", value, tmp);

        f16 = tmp;
        value  = f16;
        IZ_PRINTF("[%f] / [%f]\n", tmp, value);

        IZ_ASSERT(value == tmp);
    }
}

void HalfFloatTest()
{
    izanagi::sys::CTimer timer;
    izanagi::math::CMathRand::Init(timer.GetCurTime());

    SimpleTest();
    FloatTest();
    RandomTest();
}
