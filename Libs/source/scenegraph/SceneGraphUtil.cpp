#include "scenegraph/SceneGraphUtil.h"

using namespace izanagi;

// Clip - Screen 座標変換マトリクス計算
void CSceneGraphUtil::ComputeC2S(
	SMatrix& mtxC2S,
	IZ_FLOAT fScreenWidth,
	IZ_FLOAT fScreenHeight,
	IZ_FLOAT fCenterX,
	IZ_FLOAT fCenterY,
	IZ_FLOAT fMinZ,
	IZ_FLOAT fMaxZ)
{
	SetUnitMatrix(mtxC2S);

	mtxC2S.m[0][0] = fScreenWidth / 2.0f;
	mtxC2S.m[1][1] = -fScreenHeight / 2.0f;

	mtxC2S.m[2][2] = fMaxZ - fMinZ;
	mtxC2S.m[3][2] = fMinZ;

	mtxC2S.m[3][0] = fCenterX;
	mtxC2S.m[3][1] = fCenterY;
}

// スクリーン距離計算
IZ_FLOAT CSceneGraphUtil::ComputeScreenDistance(
	IZ_FLOAT fScreenWidth,
	IZ_FLOAT fFov)
{
	// NOTE
	// (W/2)/S = tan(θ/2) <=> S = (W/2)/tan(θ/2)
	IZ_FLOAT ret = fScreenWidth * 0.5f / tanf(fFov * 0.5f);
	return ret;
}

// クリップ座標取得
void CSceneGraphUtil::Point2Clip(
	SVector& vClip,
	const SCameraParam& sCamera,
	const SViewport& vp,
	IZ_INT nX,
	IZ_INT nY)
{
	IZ_INT nW = vp.width;
	IZ_INT nH = vp.height;
	IZ_INT nCx = nW >> 1;
	IZ_INT nCy = nH >> 1;

	vClip.Set(
		2.0f * (nX - nCx) / nW,
		-2.0f * (nY - nCy) / nH,
		0.0f,
		1.0f);
}

// 光線を取得
void CSceneGraphUtil::Point2Ray(
	SVector& vRay,
	const SCameraParam& sCamera,
	const SViewport& vp,
	IZ_INT nX,
	IZ_INT nY)
{
#if 0
	// クリップ座標取得
	SVector vClip;
	Point2Clip(
		vClip,
		sCamera, vp,
		nX, nY);

	SMatrix mC2V;
	InverseMatrix(mC2V, sCamera.mtxV2C);

	ApplyMatrix(vRay, vClip, mC2V);
	DivVector(vRay, vRay, vRay.w);
	vRay.w = 0.0f;

	SVector::Normalize(vRay, vRay);

	SMatrix mV2W;
	InverseMatrix(mV2W, sCamera.mtxW2V);
	ApplyMatrix(vRay, vRay, mV2W);
#else
	IZ_INT nW = vp.width;
	IZ_INT nH = vp.height;
	IZ_INT nCx = nW >> 1;
	IZ_INT nCy = nH >> 1;

	IZ_FLOAT fTan = tanf(sCamera.fov * 0.5f);
	const IZ_FLOAT& fAspect = sCamera.aspect;

	IZ_FLOAT x = 2.0f * (nX - nCx) * fTan / (fAspect * nW);
	IZ_FLOAT y = -2.0f * (nY - nCy) * fTan / nH;

	SVector vX, vY;
	SVector::Scale(vX, sCamera.mtxW2V.v[0], x);
	SVector::Scale(vY, sCamera.mtxW2V.v[1], y);

	SVector vZ;
	SVector::Copy(vZ, sCamera.mtxW2V.v[2]);
	vZ.z *= -1.0f;

	SVector::Add(vRay, vX, vY);
	SVector::Add(vRay, vRay, vZ);

	SVector::Normalize(vRay, vRay);
#endif
}
