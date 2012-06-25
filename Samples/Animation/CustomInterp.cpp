#include "CustomInterp.h"

struct Func : public izanagi::CNumericalAnalysis::Func
{
	IZ_FLOAT x0;
	IZ_FLOAT x1;
	IZ_FLOAT x2;
	IZ_FLOAT x3;

	IZ_FLOAT x;

	IZ_FLOAT operator()(IZ_FLOAT t)
	{
		IZ_FLOAT s = 1.0f - t;
		IZ_FLOAT s2 = s * s;
		IZ_FLOAT s3 = s2 * s;

		IZ_FLOAT t2 = t * t;
		IZ_FLOAT t3 = t2 * t;

		//IZ_FLOAT ret = s3 * x0 + 3.0f * s2 * t * x1 + 3.0f * s * t2 * x2 + t3 * x3;
		IZ_FLOAT ret = ((1 + 3 * x1 - 3 * x2) * t * t * t + (3 * x2 - 6 * x1) * t * t + 3 * x1 * t);
		ret -= x;

		return ret;
	}
};

struct FuncD : public izanagi::CNumericalAnalysis::Func
{
	IZ_FLOAT x0;
	IZ_FLOAT x1;
	IZ_FLOAT x2;
	IZ_FLOAT x3;

	IZ_FLOAT operator()(IZ_FLOAT t)
	{
		IZ_FLOAT s3_dt = -3.0f * (1.0f - t);
		IZ_FLOAT s2_t_dt = 1.0f - 4.0f * t + 3.0f * t * t;
		IZ_FLOAT s_t2_dt = 1.0f - 2.0f * t;
		IZ_FLOAT t3_dt = 3.0f * t * t;

		//IZ_FLOAT ret = s3_dt * x0 + 3.0f * s2_t_dt * x1 + 3.0f * s_t2_dt * x2 + t3_dt * x3;
		IZ_FLOAT ret = ((3 + 9 * x1 - 9 * x2) * t * t + (6 * x2 - 12 * x1) * t + 3 * x1);

		return ret;
	}
};

IZ_FLOAT CCustomInterp::InterpScalar(
	IZ_FLOAT time,
	IZ_UINT keyNum,
	IZ_UINT paramPos,
	const izanagi::S_ANM_KEY** keys)
{
#if 1
	IZ_UINT pos = paramPos * 9;

	IZ_INT prev = 0;
	IZ_INT next = -1;

	IZ_FLOAT normTime = izanagi::CAnimationUtil::ComputeNomralizedTime(
							time,
							prev, next,
							keyNum,
							keys);

	if (normTime == 0.0f)
	{
		return keys[prev]->params[pos];
	}
	else if (normTime == 1.0f)
	{
		return keys[next]->params[pos];
	}

	Func func;
	{
		func.x0 = keys[next]->params[pos + 1];
		func.x1 = keys[next]->params[pos + 2];
		func.x2 = keys[next]->params[pos + 3];
		func.x3 = keys[next]->params[pos + 4];

		func.x = normTime;
	}

	FuncD funcd;
	{
		funcd.x0 = func.x0;
		funcd.x1 = func.x1;
		funcd.x2 = func.x2;
		funcd.x3 = func.x3;
	}

	IZ_FLOAT t = izanagi::CNumericalAnalysis::ComputeNewtonMethod(
		func, funcd,
		normTime,
		0.0001f,
		10);

	Func valueFunc;
	{
		valueFunc.x0 = keys[next]->params[pos + 5];
		valueFunc.x1 = keys[next]->params[pos + 6];
		valueFunc.x2 = keys[next]->params[pos + 7];
		valueFunc.x3 = keys[next]->params[pos + 8];

		valueFunc.x = 0.0f;
	}

	IZ_FLOAT interpParam = valueFunc(t);

	// 線形補間
	IZ_FLOAT p0 = keys[prev]->params[pos];
	IZ_FLOAT p1 = keys[next]->params[pos];
	IZ_FLOAT ret = p0 * (1.0f - interpParam) + p1 * interpParam;

	return ret;
#endif
}

void CCustomInterp::InterpVector(
	izanagi::SVector& ret,
	IZ_FLOAT time,
	IZ_UINT keyNum,
	IZ_UINT paramPos,
	const izanagi::S_ANM_KEY** keys)
{
#if 1
	IZ_UINT pos = paramPos;

	IZ_INT prev = 0;
	IZ_INT next = -1;

	IZ_FLOAT normTime = izanagi::CAnimationUtil::ComputeNomralizedTime(
							time,
							prev, next,
							keyNum,
							keys);

	if (normTime == 0.0f)
	{
		ret.Set(
			keys[prev]->params[pos + 0],
			keys[prev]->params[pos + 1],
			keys[prev]->params[pos + 2],
			keys[prev]->params[pos + 3]);
		return;
	}
	else if (normTime == 1.0f)
	{
		ret.Set(
			keys[next]->params[pos + 0],
			keys[next]->params[pos + 1],
			keys[next]->params[pos + 2],
			keys[next]->params[pos + 3]);
		return;
	}

	Func func;
	{
		func.x0 = keys[next]->params[pos + 4];
		func.x1 = keys[next]->params[pos + 5];
		func.x2 = keys[next]->params[pos + 6];
		func.x3 = keys[next]->params[pos + 7];

		func.x = normTime;
	}

	FuncD funcd;
	{
		funcd.x0 = func.x0;
		funcd.x1 = func.x1;
		funcd.x2 = func.x2;
		funcd.x3 = func.x3;
	}

	IZ_FLOAT t = izanagi::CNumericalAnalysis::ComputeNewtonMethod(
		func, funcd,
		normTime,
		0.0001f,
		10);

	Func valueFunc;
	{
		valueFunc.x0 = keys[next]->params[pos + 8];
		valueFunc.x1 = keys[next]->params[pos + 9];
		valueFunc.x2 = keys[next]->params[pos + 10];
		valueFunc.x3 = keys[next]->params[pos + 11];

		valueFunc.x = 0.0f;
	}

	IZ_FLOAT interpParam = valueFunc(t);

	// SLERP
	{
		izanagi::SQuat quat1;
		quat1.Set(
			keys[prev]->params[pos + 0],
			keys[prev]->params[pos + 1],
			keys[prev]->params[pos + 2],
			keys[prev]->params[pos + 3]);

		izanagi::SQuat quat2;
		quat2.Set(
			keys[next]->params[pos + 0],
			keys[next]->params[pos + 1],
			keys[next]->params[pos + 2],
			keys[next]->params[pos + 3]);

		// Slerp
		izanagi::SQuat::Slerp(
			ret,
			quat1, quat2,
			t);
	}
#endif
}
