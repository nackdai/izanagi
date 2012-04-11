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
	SMatrix::SetUnit(mtxC2S);

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
void CSceneGraphUtil::Screen2Clip(
	SVector& vClip,
	const SCameraParam& camera,
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
	SVector& ray,
	const SCameraParam& camera,
	const SViewport& vp,
	IZ_INT nX,
	IZ_INT nY)
{
#if 0
	SVector clip;

	// Screen -> Clip
	Screen2Clip(
		clip,
		camera,
		vp,
		vp.width >> 1,
		vp.height >> 1);

	SMatrix mtxC2V;
	SMatrix::Inverse(mtxC2V, camera.mtxV2C);

	SMatrix mtxV2W;
	SMatrix::Inverse(mtxV2W, camera.mtxW2V);

	SVector rayFar;

	clip.z = vp.maxZ;
	{
		// Clip -> View
		izanagi::SMatrix::Apply(rayFar, clip, mtxC2V);

		// View -> World
		izanagi::SMatrix::Apply(rayFar, rayFar, mtxV2W);

		izanagi::SVector::Div(rayFar, rayFar, rayFar.w);
	}

	izanagi::SVector::Sub(ray, rayFar, camera.pos);
#else
	// NOTE
	//
	// Clip->Screen変換
	// | W/2  0    0 0 |
	// |  0 -H/2   0 0 |
	// |  0   0    1 0 |
	// | W/2 H/2  -1 1 |
	//
	// 逆行列
	// | 1/(W/2)   0      0 0 |
	// |  0      -1/(H/2) 0 0 |
	// |  0        0      1 0 |
	// | -1        1      0 1 |
	//
	// ここから、(x, y, z, 1)のスクリーン座標をClip座標に変換すると
	// (x/(W/2) - 1, -y/(H/2) + 1, z, 1)
	//
	// Scree->View変換の逆行列
	// | 1/Sx  0   0    0     |
	// |   0  1/Sy 0    0     |
	// |   0   0   0 -1/(SzN) |
	// |   0   0   1   1/N    |
	//
	// Sx = 1/(a・tan(θ/2))
	// Sy = 1/tan(θ/2)
	// Sz = F/(F - N)
	//
	// ここにClip座標に変換した座標をかけると
	//  2a(x - W/2)       -2(y - H/2)           -z(F - N) + F
	// (-----------tanθ, ------------tanθ, 1, -------------)
	//       W                 H                     FN
	// となる

	IZ_INT nW = vp.width;
	IZ_INT nH = vp.height;
	IZ_INT nCx = nW >> 1;
	IZ_INT nCy = nH >> 1;

	IZ_FLOAT fTan = tanf(camera.fov * 0.5f);
	const IZ_FLOAT& fAspect = camera.aspect;

	// Scree -> Clip
	IZ_FLOAT x = 2.0f * fAspect * (nX - nCx) * fTan / nW;
	IZ_FLOAT y = -2.0f * (nY - nCy) * fTan / nH;

	IZ_FLOAT div = 1.0f / (camera.cameraFar * camera.cameraNear);
	IZ_FLOAT z = vp.maxZ;
	IZ_FLOAT w = (-z * (camera.cameraFar - camera.cameraNear) + camera.cameraFar) * div;

	SMatrix mtx;
	SMatrix::Inverse(mtx, camera.mtxW2V);

	CVector rayFar(x, y, 1.0f, w);

	SMatrix::Apply(rayFar, rayFar, mtx);
	SVector::Div(rayFar, rayFar, rayFar.w);

	SVector::Sub(ray, rayFar, camera.pos);
#endif
}
