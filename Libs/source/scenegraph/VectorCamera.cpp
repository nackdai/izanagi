#include "scenegraph/VectorCamera.h"
#include "izGraph.h"

using namespace izanagi;

CVectorCamera::CVectorCamera()
: m_Pos(m_Param.pos), m_Y(m_Param.up)
{
}

CVectorCamera::~CVectorCamera()
{
}

// 初期化
void CVectorCamera::Init(
	const SVector& pos,
	const SVector& at,
	IZ_FLOAT fNear, IZ_FLOAT fFar,
	IZ_FLOAT fov,
	IZ_FLOAT aspect)
{
	m_Param.cameraNear = fNear;
	m_Param.cameraFar = fFar;
	m_Param.fov = fov;

	m_Param.aspect = aspect;

	m_IsDirtyW2V = IZ_TRUE;
	m_IsDirtyV2C = IZ_TRUE;

	m_Y.Set(0.0f, 1.0f, 0.0f, 0.0f);

	m_Z.Set(0.0f, 0.0f, at.z - pos.z, 0.0f);
	if (m_Z.z < 0.0f) {
		m_Z.z = -1.0f;
	}
	else {
		m_Z.z = 1.0f;
	}

	SVector::Cross(m_X, m_Y, m_Z);

	m_Pos.Set(pos.x, pos.y, pos.z);

	izanagi::SMatrix::SetUnit(m_Transform);
}

// カメラ更新
void CVectorCamera::Update()
{
	if (m_IsDirtyW2V) {
		SMatrix::SetUnit(m_Param.mtxW2V);

		SVector::Copy(m_Param.mtxW2V.v[0], m_X);
		SVector::Copy(m_Param.mtxW2V.v[1], m_Y);
		SVector::Copy(m_Param.mtxW2V.v[2], m_Z);

		SMatrix::Transpose(m_Param.mtxW2V, m_Param.mtxW2V);

		m_Param.mtxW2V.m[3][0] = -1.0f * SVector::Dot(m_X, m_Pos);
		m_Param.mtxW2V.m[3][1] = -1.0f * SVector::Dot(m_Y, m_Pos);
		m_Param.mtxW2V.m[3][2] = -1.0f * SVector::Dot(m_Z, m_Pos);
		m_Param.mtxW2V.m[3][3] = 1.0f;

		// 一応、注視点を更新
#ifdef IZ_COORD_LEFT_HAND
		// 右手系なので、マイナスする
		izanagi::SVector::SubXYZ(m_Param.ref, m_Z, m_Pos);
#else
		// 右手系なので、マイナスする
		izanagi::SVector::SubXYZ(m_Param.ref, m_Pos, m_Z);
#endif

		{
			SVector::Copy(m_Transform.v[0], m_X);
			SVector::Copy(m_Transform.v[1], m_Y);
			SVector::Copy(m_Transform.v[2], m_Z);
			SVector::Copy(m_Transform.v[3], m_Pos);
		}
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

// 移動.
void CVectorCamera::Translate(const SVector& trans)
{
	SVector::AddXYZ(m_Pos, m_Pos, trans);
	m_IsDirtyW2V = IZ_TRUE;
}

// X軸回転.
void CVectorCamera::RotateByX(IZ_FLOAT rad)
{
	Rotate(m_X, rad);
}

// Y軸回転.
void CVectorCamera::RotateByY(IZ_FLOAT rad)
{
	Rotate(m_Y, rad);
}

// Z軸回転.
void CVectorCamera::RotateByZ(IZ_FLOAT rad)
{
	Rotate(m_Z, rad);
}

// 任意軸回転.
void CVectorCamera::Rotate(
	const SVector& axis,
	IZ_FLOAT rad)
{
	SQuat quat;
	SQuat::SetQuatFromRadAxis(quat, rad, axis);

	SMatrix mtx;
	SQuat::MatrixFromQuat(mtx, quat);

	SMatrix::Apply(m_X, m_X, mtx);
	SMatrix::Apply(m_Y, m_Y, mtx);
	SMatrix::Apply(m_Z, m_Z, mtx);
	SMatrix::Apply(m_Pos, m_Pos, mtx);

	m_IsDirtyW2V = IZ_TRUE;
}

// 位置指定.
void CVectorCamera::SetPos(const SVector& pos)
{
	m_Pos.Set(pos.x, pos.y, pos.z);
	m_IsDirtyW2V = IZ_TRUE;
}
