#include "scenegraph/Camera.h"
#include "izGraph.h"

using namespace izanagi;

// NOTE
// DirectX の座標系は左手系
// OpenGL の座標系は右手系

//#define VIEW_LH

// 初期化
void CCamera::Init(
	const SVector& vecPos,
	const SVector& vecRef,
	const SVector& vecUp,
	IZ_FLOAT fNear, IZ_FLOAT fFar,
	IZ_FLOAT fFov,
	IZ_FLOAT fAspect)
{
	cameraNear = fNear;
	cameraFar = fFar;
	fov = fFov;

	aspect = fAspect;

	CopyVector(pos, vecPos);
	CopyVector(ref, vecRef);
	CopyVector(up, vecUp);
}

void CCamera::Init(const SCameraParam& sParam)
{
	Init(
		sParam.pos,
		sParam.ref,
		sParam.up,
		sParam.cameraNear,
		sParam.cameraFar,
		sParam.fov,
		sParam.aspect);
}

// カメラ更新
void CCamera::UpdateCamera()
{
	ComputeW2V();
	ComputeV2C();

	MulMatrix(mtxW2C, mtxW2V, mtxV2C);
}

#if 0
void CCamera::RenderCamera(CGraphicsDevice* pDevice)
{
	// うーん・・・
	IZ_C_ASSERT(sizeof(SMatrix) == sizeof(D3DMATRIX));

	D3D_DEVICE* pDev = pDevice->GetRawInterface();

	// DirectX限定
	pDev->SetTransform(
		D3DTS_VIEW,
		(D3DMATRIX*)&mtxW2V.m);

	pDev->SetTransform(
		D3DTS_PROJECTION,
		(D3DMATRIX*)&mtxV2C.m);
}
#endif

// World - View
void CCamera::ComputeW2V()
{
	SetUnitMatrix(mtxW2V);

	SVector vecX;
	SVector vecY;
	SVector vecZ;

	// Z
	// 視点から注視点へのベクトル
#ifdef VIEW_LH
	SubVectorXYZ(vecZ, ref, pos);	// 左手
#else	// #ifdef VIEW_LH
	SubVectorXYZ(vecZ, pos, ref);	// 右手
#endif	// #ifdef VIEW_LH

	NormalizeVector(vecZ, vecZ);

	// X
	CrossVector(vecX, up, vecZ);
	NormalizeVector(vecX, vecX);

	// Y
	CrossVector(vecY, vecZ, vecX);

	CopyVector(mtxW2V.v[0], vecX);
	CopyVector(mtxW2V.v[1], vecY);
	CopyVector(mtxW2V.v[2], vecZ);

	TransposeMatrix(mtxW2V, mtxW2V);

	mtxW2V.m[3][0] = -1.0f * DotVector(vecX, pos);
	mtxW2V.m[3][1] = -1.0f * DotVector(vecY, pos);
	mtxW2V.m[3][2] = -1.0f * DotVector(vecZ, pos);
	mtxW2V.m[3][3] = 1.0f;
}

// View - Clip
void CCamera::ComputeV2C()
{
	SetUnitMatrix(mtxV2C);

#if 0
	// Use Horizontal FOV
	const IZ_FLOAT fH = 1 / tanf(fov * 0.5f);
	const IZ_FLOAT fW = fH / aspect;
#else
	// Use Vertical FOV
	const IZ_FLOAT fW = 1 / tanf(fov * 0.5f);
	const IZ_FLOAT fH = fW * aspect;
#endif
	const IZ_FLOAT fQ = -cameraFar / (cameraFar - cameraNear);

	mtxV2C.m[0][0] = fW;
	mtxV2C.m[1][1] = fH;

#ifdef VIEW_LH
	// 左手
	mtxV2C.m[2][2] = -fQ;
	mtxV2C.m[3][2] = fQ * cameraNear;
	mtxV2C.m[2][3] = 1.0f;
#else	// #ifdef VIEW_LH
	// 右手
	mtxV2C.m[2][2] = fQ;
	mtxV2C.m[3][2] = fQ * cameraNear;
	mtxV2C.m[2][3] = -1.0f;
#endif	// #ifdef VIEW_LH

	mtxV2C.m[3][3] = 0.0f;
}
