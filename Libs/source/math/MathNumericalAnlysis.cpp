#include "math/MathNumericalAnlysis.h"
#include "math/MathUtil.h"

namespace izanagi
{
namespace math
{
	// 二分法.
	IZ_FLOAT CNumericalAnalysis::ComputeBisectionMethod(
		CNumericalAnalysis::Func& func,
		IZ_FLOAT a,
		IZ_FLOAT b,
		IZ_FLOAT threshold)
	{
		IZ_FLOAT fa = func(a);
		IZ_FLOAT fb = func(b);

		// 許さない.
		VRETURN_VAL(fa * fb > 0.0f, 0.0f);

		if (b - a < 0.0f)
		{
			IZ_FLOAT tmp = b;
			b = a;
			a = tmp;
		}

		// c = (a + b) / 2
		IZ_FLOAT c = (a + b) * 0.5f;

		for (;;)
		{
			if (b - a <= threshold)
			{
				return c;
			}

			// c = (a + b) / 2
			c = (a + b) * 0.5f;

			IZ_FLOAT fc = func(c);
			IZ_FLOAT fa = func(a);

			if (fc * fa < 0.0f)
			{
				b = c;
			}
			else
			{
				a = c;
			}
		}
	}

	// ニュートン法.
	IZ_FLOAT CNumericalAnalysis::ComputeNewtonMethod(
		Func& func,
		Func& funcd,    // funcを微分した関数
		IZ_FLOAT init,
		IZ_FLOAT threshold,
		IZ_UINT loopCnt)
	{
		// さすがに１回以上はまわしてほしい
		IZ_ASSERT(loopCnt >= 1);

		IZ_FLOAT x = init;
		IZ_FLOAT newX = x;

        // NOTE
        // ニュートン法
        // Xn+1 = Xn - f(Xn) / f'(Xn)

		for (IZ_UINT i = 0; i < loopCnt; i++)
		{
			IZ_FLOAT f = func(x);
			IZ_FLOAT fd = funcd(x);

			if (CMath::IsNearyEqualZero(fd))
			{
				return newX;
			}

			newX = x - f / fd;

			if (CMath::IsNearyEqualZero(x))
			{
				return newX;
			}

			if (CMath::Absf((newX - x) / x) <= threshold)
			{
				return newX;
			}

			x = newX;
		}

		// 収束しなかったときにここにくる

		// 一応
		//IZ_ASSERT(IZ_FALSE);
		return newX;
	}
}   // namespace math
}	// namespace izanagi
