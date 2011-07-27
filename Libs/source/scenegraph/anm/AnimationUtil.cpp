#include "scenegraph/anm/AnimationUtil.h"

using namespace izanagi;

IZ_BOOL CAnimationUtil::IsScalarInterp(IZ_UINT type)
{
	IZ_BOOL ret = (type == E_ANM_INTERP_TYPE_LINEAR)
					|| (type == E_ANM_INTERP_TYPE_BEZIER)
					|| (type == E_ANM_INTERP_TYPE_HERMITE);
	return ret;
}

IZ_FLOAT CAnimationUtil::ComputeInterp(
	E_ANM_INTERP_TYPE nInterp,
	IZ_FLOAT fTime,
	IZ_UINT nKeyNum,
	IZ_UINT nPos,
	S_ANM_KEY** const pKeys)
{
	typedef IZ_FLOAT (*funcInterp)(IZ_FLOAT, IZ_UINT, IZ_UINT, S_ANM_KEY** const);

	static funcInterp tblFuncInterp[] = {
		&CAnimationUtil::ComputeLinear,
		&CAnimationUtil::ComputeBezier,
		&CAnimationUtil::ComputeHermite,
		IZ_NULL,
	};
	IZ_C_ASSERT(COUNTOF(tblFuncInterp) == E_ANM_INTERP_TYPE_NUM);

	IZ_ASSERT(nInterp < E_ANM_INTERP_TYPE_NUM);
	IZ_ASSERT(tblFuncInterp[nInterp] != IZ_NULL);

	IZ_FLOAT ret = (*tblFuncInterp[nInterp])(fTime, nKeyNum, nPos, pKeys);
	return ret;
}

void CAnimationUtil::ComputeInterp(
	SVector& vRef,
	E_ANM_INTERP_TYPE nInterp,
	IZ_FLOAT fTime,
	IZ_UINT nKeyNum,
	IZ_UINT nPos,
	S_ANM_KEY** const pKeys)
{
	typedef void (*funcInterp)(SVector&, IZ_FLOAT, IZ_UINT, IZ_UINT, S_ANM_KEY** const);

	static funcInterp tblFuncInterp[] = {
		IZ_NULL,
		IZ_NULL,
		IZ_NULL,
		&CAnimationUtil::ComputeSlerp,
	};
	IZ_C_ASSERT(COUNTOF(tblFuncInterp) == E_ANM_INTERP_TYPE_NUM);

	IZ_ASSERT(nInterp < E_ANM_INTERP_TYPE_NUM);
	IZ_ASSERT(tblFuncInterp[nInterp] != IZ_NULL);

	(*tblFuncInterp[nInterp])(vRef, fTime, nKeyNum, nPos, pKeys);
}

IZ_FLOAT CAnimationUtil::ComputeNomralizedTime(
	IZ_FLOAT fTime,
	IZ_INT& nPrev,
	IZ_INT& nNext,
	IZ_UINT nKeyNum,
	S_ANM_KEY** const pKeys)
{
	nPrev = 0;
	nNext = -1;

	IZ_FLOAT fPrevTime = pKeys[0]->keyTime;

	for (IZ_UINT i = 1; i < nKeyNum; ++i) {
		IZ_FLOAT fNextTime = pKeys[i]->keyTime;
		if ((fPrevTime <= fTime) && (fTime <= fNextTime)) {
			nPrev = i - 1;
			nNext = i;
			break;
		}
		fPrevTime = fNextTime;
	}

	IZ_ASSERT(nNext > nPrev);

	// Normalize time 0 to 1.
	const IZ_FLOAT fStartTime = pKeys[nPrev]->keyTime;
	const IZ_FLOAT fEndTime = pKeys[nNext]->keyTime;
	IZ_ASSERT(fStartTime < fEndTime);

	IZ_FLOAT fNormTime = (fTime - fStartTime) / (fEndTime - fStartTime);
	fNormTime = CMath::Clamp(fNormTime, 0.0f, 1.0f);

	return fNormTime;
}

IZ_FLOAT CAnimationUtil::ComputeLinear(
	IZ_FLOAT fTime,
	IZ_UINT nKeyNum,
	IZ_UINT nPos,
	S_ANM_KEY** const pKeys)
{
	IZ_INT nPrev = 0;
	IZ_INT nNext = -1;

	IZ_FLOAT fNormTime = ComputeNomralizedTime(
							fTime,
							nPrev, nNext,
							nKeyNum,
							pKeys);

	IZ_ASSERT(nPos < pKeys[nPrev]->numParams);
	IZ_FLOAT param_0 = pKeys[nPrev]->params[nPos];

	IZ_ASSERT(nPos < pKeys[nNext]->numParams);
	IZ_FLOAT param_1 = pKeys[nNext]->params[nPos];

	IZ_FLOAT ret = param_0 * (1.0f - fNormTime) + param_1 * fNormTime;

	return ret;
}

IZ_FLOAT CAnimationUtil::ComputeBezier(
	IZ_FLOAT fTime,
	IZ_UINT nKeyNum,
	IZ_UINT nPos,
	S_ANM_KEY** const pKeys)
{
	// TODO
	IZ_ASSERT(IZ_FALSE);
	return 0.0f;
}

IZ_FLOAT CAnimationUtil::ComputeHermite(
	IZ_FLOAT fTime,
	IZ_UINT nKeyNum,
	IZ_UINT nPos,
	S_ANM_KEY** const pKeys)
{
	IZ_ASSERT(pKeys != IZ_NULL);

	IZ_FLOAT ret = 0.0f;

	const IZ_UINT KEY_PARAM_VALUE       = nPos * 3;
	const IZ_UINT KEY_PARAM_IN_TANGENT  = KEY_PARAM_VALUE + 1;
	const IZ_UINT KEY_PARAM_OUT_TANGENT = KEY_PARAM_IN_TANGENT + 1;

	// NOTE
	// s = (time - time0) / (time1 - time0)	: Normalize time 0 to 1
	// S = { s^3, s^2, s^1, 1}
	// C = { P1, P2, T1, T2 }
	// b : Bezier matrix
	// P = S * b * c

	static const SMatrix mtxBezier = {
		 2.0f, -2.0f,  1.0f,  1.0f,
		-3.0f,  3.0f, -2.0f, -1.0f,
		 0.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,  0.0f,
	};

	if (pKeys[0]->keyTime >= fTime) {
		ret = pKeys[0]->params[KEY_PARAM_VALUE];
	}
	else if (pKeys[nKeyNum - 1]->keyTime <= fTime) {
		ret = pKeys[nKeyNum - 1]->params[KEY_PARAM_VALUE];
	}
	else {
		IZ_INT nPrev = 0;
		IZ_INT nNext = -1;

		IZ_FLOAT fNormTime = ComputeNomralizedTime(
								fTime,
								nPrev, nNext,
								nKeyNum,
								pKeys);

		IZ_FLOAT fNormTime_2 = fNormTime * fNormTime;

		SVector vecS = {
			fNormTime_2 * fNormTime,
			fNormTime_2,
			fNormTime,
			1.0f,
		};

		SVector vecC = {
			pKeys[nPrev]->params[KEY_PARAM_VALUE],
			pKeys[nNext]->params[KEY_PARAM_VALUE],
			pKeys[nPrev]->params[KEY_PARAM_OUT_TANGENT],
			pKeys[nNext]->params[KEY_PARAM_IN_TANGENT],
		};

		SMatrix::Apply(vecS, vecS, mtxBezier);
		ret = SVector::Dot(vecS, vecC);
	}

	return ret;
}

void CAnimationUtil::ComputeSlerp(
	SVector& vRef,
	IZ_FLOAT fTime,
	IZ_UINT nKeyNum,
	IZ_UINT nPos,
	S_ANM_KEY** const pKeys)
{
	IZ_ASSERT(pKeys != IZ_NULL);

	if (pKeys[0]->keyTime >= fTime) {
		vRef.x = pKeys[0]->params[0];
		vRef.y = pKeys[0]->params[1];
		vRef.z = pKeys[0]->params[2];
		vRef.w = pKeys[0]->params[3];
	}
	else if (pKeys[nKeyNum - 1]->keyTime <= fTime) {
		vRef.x = pKeys[nKeyNum - 1]->params[0];
		vRef.y = pKeys[nKeyNum - 1]->params[1];
		vRef.z = pKeys[nKeyNum - 1]->params[2];
		vRef.w = pKeys[nKeyNum - 1]->params[3];
	}
	else {
		IZ_INT nPrev = 0;
		IZ_INT nNext = -1;

		IZ_FLOAT fNormTime = ComputeNomralizedTime(
								fTime,
								nPrev, nNext,
								nKeyNum,
								pKeys);

		SQuat quat1;
		quat1.Set(
			pKeys[nPrev]->params[0],
			pKeys[nPrev]->params[1],
			pKeys[nPrev]->params[2],
			pKeys[nPrev]->params[3]);

		SQuat quat2;
		quat2.Set(
			pKeys[nNext]->params[0],
			pKeys[nNext]->params[1],
			pKeys[nNext]->params[2],
			pKeys[nNext]->params[3]);

		// Slerp
		SlerpQuat(
			vRef,
			quat1, quat2,
			fNormTime);
	}
}
