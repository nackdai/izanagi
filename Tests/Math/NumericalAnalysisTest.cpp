#include "NumericalAnalysisTest.h"
#include "izMath.h"

struct Func : public izanagi::CNumericalAnalysis::Func
{
	IZ_FLOAT operator()(IZ_FLOAT t)
	{
		return t * t - 2;
	}
};

struct FuncD : public izanagi::CNumericalAnalysis::Func
{
	IZ_FLOAT operator()(IZ_FLOAT t)
	{
		return 2 * t;
	}
};

void NumericalAnalysisTest()
{
	IZ_FLOAT t = izanagi::CNumericalAnalysis::ComputeNewtonMethod(
		Func(),
		FuncD(),
		10.0f,
		0.00001f,
		10);

	IZ_FLOAT x = izanagi::CNumericalAnalysis::ComputeBisectionMethod(
		Func(),
		-10.0f, 10.0f,
		0.00001f);
}
