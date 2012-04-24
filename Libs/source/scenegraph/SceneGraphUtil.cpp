#include "scenegraph/SceneGraphUtil.h"

using namespace izanagi;

// Clip - Screen 座標変換マトリクス計算
void CSceneGraphUtil::ComputeC2S(
	SMatrix& mtxC2S,
	IZ_INT screenWidth,
	IZ_INT screenHeight,
	IZ_FLOAT minZ,
	IZ_FLOAT maxZ)
{
	SMatrix::SetUnit(mtxC2S);

	mtxC2S.m[0][0] = screenWidth * 0.5f;;
	mtxC2S.m[1][1] = -screenHeight * 0.5f;;

	mtxC2S.m[2][2] = maxZ - minZ;
	mtxC2S.m[3][2] = minZ;

	mtxC2S.m[3][0] = screenWidth * 0.5f;;
	mtxC2S.m[3][1] = screenHeight * 0.5f;
}

// スクリーン距離計算
IZ_FLOAT CSceneGraphUtil::ComputeScreenDistance(
	IZ_FLOAT screenHeight,
	IZ_FLOAT fov)
{
	// NOTE
	// (H/2)/S = tan(θ/2) <=> S = (H/2)/tan(θ/2)
	IZ_FLOAT ret = screenHeight * 0.5f / tanf(fov * 0.5f);
	return ret;
}

// クリップ座標取得
void CSceneGraphUtil::Screen2Clip(
	SVector& vClip,
	const SCameraParam& camera,
	const SViewport& vp,
	IZ_INT nX, IZ_INT nY,
	IZ_FLOAT fZ)
{
	IZ_INT nW = vp.width;
	IZ_INT nH = vp.height;
	IZ_INT nCx = nW >> 1;
	IZ_INT nCy = nH >> 1;

	vClip.Set(
		2.0f * (nX - nCx) / nW,
		-2.0f * (nY - nCy) / nH,
		(fZ - vp.minZ) / (vp.maxZ - vp.minZ),
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
	SMatrix mtxS2C;
	ComputeC2S(
		mtxS2C,
		vp.width, vp.height,
		vp.minZ, vp.maxZ);
	SMatrix::Inverse(mtxS2C, mtxS2C);

	SVector clip;

	// Screen -> Clip
	SMatrix::Apply(
		clip,
		CVector((IZ_FLOAT)nX, (IZ_FLOAT)nY, 1.0f, 1.0f),
		mtxS2C);

	SMatrix mtxC2V;
	SMatrix::Inverse(mtxC2V, camera.mtxV2C);

	SMatrix mtxV2W;
	SMatrix::Inverse(mtxV2W, camera.mtxW2V);

	SVector rayFar;
	{
		// Clip -> View
		izanagi::SMatrix::Apply(rayFar, clip, mtxC2V);

		// View -> World
		izanagi::SMatrix::Apply(rayFar, rayFar, mtxV2W);

		izanagi::SVector::Div(rayFar, rayFar, rayFar.w);
	}

	izanagi::SVector::Sub(ray, rayFar, camera.pos);
#elif 0
	SVector clip;

	// Screen -> Clip
	Screen2Clip(
		clip,
		camera,
		vp,
		nX, nY,
		1.0f);

	SMatrix mtxC2V;
	SMatrix::Inverse(mtxC2V, camera.mtxV2C);

	SMatrix mtxV2W;
	SMatrix::Inverse(mtxV2W, camera.mtxW2V);

	SVector rayFar;
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
	// | W/2  0        0       0 |
	// |  0 -H/2       0       0 |
	// |  0   0   maxZ - minZ  0 |
	// | W/2 H/2     -minZ     1 |
	//
	// 逆行列
	// | 1/(W/2)   0      0 0 |
	// |  0      -1/(H/2) 0 0 |
	// |  0        0      a 0 |
	// | -1        1      b 1 |
	//
	// a = 1 / (maxZ - miinZ)
	// b = -minZ / (maxZ - miinZ)
	//
	// ここから、(x, y, z, 1)のスクリーン座標をClip座標に変換すると
	// (x/(W/2) - 1, -y/(H/2) + 1, (z - minZ) / (maxZ - minZ), 1)
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
	//  2a(x - W/2)       -2(y - H/2)           -Z(F - N) + F
	// (-----------tanθ, ------------tanθ, 1, -------------)
	//       W                 H                      FN
	//
	// Z = (z - minZ) / (maxZ - minZ)
	//
	// となる

	const IZ_INT nW = vp.width;
	const IZ_INT nH = vp.height;
	const IZ_INT nCx = nW >> 1;
	const IZ_INT nCy = nH >> 1;

	const IZ_FLOAT fTan = tanf(camera.fov * 0.5f);
	const IZ_FLOAT fAspect = camera.aspect;

	// Scree -> Clip
	IZ_FLOAT x = 2.0f * fAspect * (nX - nCx) * fTan / nW;
	IZ_FLOAT y = -2.0f * (nY - nCy) * fTan / nH;

	const IZ_FLOAT F = camera.cameraFar;
	const IZ_FLOAT N = camera.cameraNear;
	IZ_FLOAT z = (1.0f - vp.minZ) / (vp.maxZ - vp.minZ);
	IZ_FLOAT w = (-z * (F - N) + F) / (F * N);

	SMatrix mtx;
	SMatrix::Inverse(mtx, camera.mtxW2V);

	CVector rayFar(x, y, 1.0f, w);

	SMatrix::Apply(rayFar, rayFar, mtx);
	SVector::Div(rayFar, rayFar, rayFar.w);

	SVector::Sub(ray, rayFar, camera.pos);
#endif
}
