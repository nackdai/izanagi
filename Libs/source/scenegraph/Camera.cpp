#include "scenegraph/Camera.h"
#include "izGraph.h"

using namespace izanagi;

// NOTE
// DirectX の座標系は左手系
// OpenGL の座標系は右手系

// 初期化
void CCamera::Init(
	const SVector& vecPos,
	const SVector& vecRef,
	const SVector& vecUp,
	IZ_FLOAT fNear, IZ_FLOAT fFar,
	IZ_FLOAT fFov,
	IZ_FLOAT fAspect)
{
	m_Param.cameraNear = fNear;
	m_Param.cameraFar = fFar;
	m_Param.fov = fFov;

	m_Param.aspect = fAspect;

	SVector::Copy(m_Param.pos, vecPos);
	SVector::Copy(m_Param.ref, vecRef);
	SVector::Copy(m_Param.up, vecUp);

	m_IsDirtyW2V = IZ_TRUE;
	m_IsDirtyV2C = IZ_TRUE;
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
void CCamera::Update()
{
	if (m_IsDirtyW2V) {
		ComputeW2V();
	}
	if (m_IsDirtyV2C) {
		ComputeV2C();
	}

	if (m_IsDirtyW2V || m_IsDirtyV2C) {
		SMatrix::Mul(m_Param.mtxW2C, m_Param.mtxW2V, m_Param.mtxV2C);

		m_IsDirtyW2V = IZ_FALSE;
		m_IsDirtyV2C = IZ_FALSE;
	}
}

// カメラ座標系でのオフセットを考慮にいれたV2Cマトリクスを取得.
void CCamera::GetOffsetV2C(
	SMatrix& mtxV2C,
	const SVector& pos,
	IZ_FLOAT delta)
{
	// 念のため
	Update();

	// カメラ座標系の位置を計算
	SVector viewPos;
	SMatrix::Apply(
		viewPos,
		pos,
		m_Param.mtxW2V);

	GetOffsetV2C(
		mtxV2C,
		viewPos.z,
		delta);
}

void CCamera::GetOffsetV2C(
	SMatrix& mtxV2C,
	IZ_FLOAT viewZ,
	IZ_FLOAT delta)
{
	// 念のため
	Update();

	IZ_FLOAT f = m_Param.cameraFar;
	IZ_FLOAT n = m_Param.cameraNear;

	IZ_FLOAT epsilon = -2.0f * f * n * delta;
	epsilon /= ((f + n) * viewZ * (viewZ + delta));

	SMatrix::Copy(mtxV2C, m_Param.mtxV2C);

	mtxV2C.m[2][2] *= (1.0f + epsilon);
}

// World - View
void CCamera::ComputeW2V()
{
	SMatrix::SetUnit(m_Param.mtxW2V);

	SVector vecX;
	SVector vecY;
	SVector vecZ;

	vecX.w = vecY.w = vecZ.w = 0.0f;

	// Z
	// 視点から注視点へのベクトル
#ifdef IZ_COORD_LEFT_HAND
	SVector::SubXYZ(vecZ, m_Param.ref, m_Param.pos);	// 左手
#else	// #ifdef VIEW_LH
	SVector::SubXYZ(vecZ, m_Param.pos, m_Param.ref);	// 右手
#endif	// #ifdef VIEW_LH

	SVector::Normalize(vecZ, vecZ);

	// NOTE
	// 右手座標

	// TODO
	// OpenGL

	// X = Y x Z
	SVector::Cross(vecX, m_Param.up, vecZ);
	SVector::Normalize(vecX, vecX);

	// Y = Z x X
	SVector::Cross(vecY, vecZ, vecX);
	vecY.w = 0.0f;

	SVector::Copy(m_Param.mtxW2V.v[0], vecX);
	SVector::Copy(m_Param.mtxW2V.v[1], vecY);
	SVector::Copy(m_Param.mtxW2V.v[2], vecZ);

	SMatrix::Transpose(m_Param.mtxW2V, m_Param.mtxW2V);

	m_Param.mtxW2V.m[3][0] = -1.0f * SVector::Dot(vecX, m_Param.pos);
	m_Param.mtxW2V.m[3][1] = -1.0f * SVector::Dot(vecY, m_Param.pos);
	m_Param.mtxW2V.m[3][2] = -1.0f * SVector::Dot(vecZ, m_Param.pos);
	m_Param.mtxW2V.m[3][3] = 1.0f;
}

// View - Clip
void CCamera::ComputeV2C()
{
	SMatrix::SetUnit(m_Param.mtxV2C);

	// Use Vertical FOV
	const IZ_FLOAT fH = 1 / tanf(m_Param.fov * 0.5f);
	const IZ_FLOAT fW = fH / m_Param.aspect;

	const IZ_FLOAT fQ = -m_Param.cameraFar / (m_Param.cameraFar - m_Param.cameraNear);

	m_Param.mtxV2C.m[0][0] = fW;
	m_Param.mtxV2C.m[1][1] = fH;

#ifdef IZ_COORD_LEFT_HAND
	// 左手
	m_Param.mtxV2C.m[2][2] = -fQ;
	m_Param.mtxV2C.m[3][2] = fQ * m_Param.cameraNear;
	m_Param.mtxV2C.m[2][3] = 1.0f;
#else	// #ifdef VIEW_LH
	// 右手
	m_Param.mtxV2C.m[2][2] = fQ;
	m_Param.mtxV2C.m[3][2] = fQ * m_Param.cameraNear;
	m_Param.mtxV2C.m[2][3] = -1.0f;
#endif	// #ifdef VIEW_LH

	m_Param.mtxV2C.m[3][3] = 0.0f;
}
