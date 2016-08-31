#include "scenegraph/VectorCamera.h"
#include "izGraph.h"

using namespace izanagi;

CVectorCamera::CVectorCamera()
    : m_Pos(m_Param.pos), m_Y(m_Param.up), m_At(m_Param.ref)
{
    m_NeedUpdateByTransform = IZ_FALSE;
}

CVectorCamera::~CVectorCamera()
{
}

// 初期化
void CVectorCamera::Init(
    const math::SVector4& pos,
    const math::SVector4& at,
    IZ_FLOAT fNear, IZ_FLOAT fFar,
    IZ_FLOAT verticalFOV,
    IZ_FLOAT aspect)
{
    CCamera::Init(
        pos, at,
        math::CVector4(0.0f, 1.0f, 0.0f, 0.0f),
        fNear, fFar,
        verticalFOV,
        aspect);

    computeLenghtBetweenPosAndAt();

    math::SMatrix44::SetUnit(m_Transform);

    m_NeedUpdateByTransform = IZ_FALSE;
}

void CVectorCamera::computeLenghtBetweenPosAndAt()
{
    m_lenghtBetweenPosAndAt = math::SVector4::Length2(m_Pos, m_At);
}

// カメラ更新
void CVectorCamera::Update()
{
    if (m_IsDirtyW2V) {
#if 0
        math::SMatrix44::SetUnit(m_Param.mtxW2V);

        math::SVector4::Copy(m_Param.mtxW2V.v[0], m_X);
        math::SVector4::Copy(m_Param.mtxW2V.v[1], m_Y);
        math::SVector4::Copy(m_Param.mtxW2V.v[2], m_Z);

        math::SMatrix44::Transpose(m_Param.mtxW2V, m_Param.mtxW2V);

        m_Param.mtxW2V.m[3][0] = -1.0f * math::SVector4::Dot(m_X, m_Pos);
        m_Param.mtxW2V.m[3][1] = -1.0f * math::SVector4::Dot(m_Y, m_Pos);
        m_Param.mtxW2V.m[3][2] = -1.0f * math::SVector4::Dot(m_Z, m_Pos);
        m_Param.mtxW2V.m[3][3] = 1.0f;

        // 一応、注視点を更新
#ifdef IZ_COORD_LEFT_HAND
        // 右手系なので、マイナスする
        izanagi::math::SVector4::SubXYZ(m_Param.ref, m_Z, m_Pos);
#else
        // 右手系なので、マイナスする
        izanagi::math::SVector4::SubXYZ(m_Param.ref, m_Pos, m_Z);
#endif
        {
            math::SVector4::Copy(m_Transform.v[0], m_X);
            math::SVector4::Copy(m_Transform.v[1], m_Y);
            math::SVector4::Copy(m_Transform.v[2], m_Z);
            math::SVector4::Copy(m_Transform.v[3], m_Pos);
        }
#else
        if (m_NeedUpdateByTransform) {
            math::SMatrix44::SetUnit(m_Param.mtxW2V);

            math::SVector4::Copy(m_Param.mtxW2V.v[0], m_X);
            math::SVector4::Copy(m_Param.mtxW2V.v[1], m_Y);
            math::SVector4::Copy(m_Param.mtxW2V.v[2], m_Z);

            math::SMatrix44::Transpose(m_Param.mtxW2V, m_Param.mtxW2V);

            m_Param.mtxW2V.m[3][0] = -1.0f * math::SVector4::Dot(m_X, m_Pos);
            m_Param.mtxW2V.m[3][1] = -1.0f * math::SVector4::Dot(m_Y, m_Pos);
            m_Param.mtxW2V.m[3][2] = -1.0f * math::SVector4::Dot(m_Z, m_Pos);
            m_Param.mtxW2V.m[3][3] = 1.0f;

#if 0
            // 一応、注視点を更新
            // 右手系なので、マイナスする
            izanagi::math::SVector4::SubXYZ(m_Param.ref, m_Z, m_Pos);
#else
            izanagi::math::CVector3 dir(m_Z.x, m_Z.y, m_Z.z);
            dir *= m_lenghtBetweenPosAndAt;

            izanagi::math::CVector4 pos(m_Pos);
            auto at = pos + dir;
            m_At.Set(at.x, at.y, at.z);
#endif

            {
                math::SVector4::Copy(m_Transform.v[0], m_X);
                math::SVector4::Copy(m_Transform.v[1], m_Y);
                math::SVector4::Copy(m_Transform.v[2], m_Z);
                math::SVector4::Copy(m_Transform.v[3], m_Pos);
            }
        }
        else {
            ComputeW2V();

            math::SMatrix44::Copy(m_Transform, m_Param.mtxW2V);
            math::SVector4::Set(m_Transform.v[3], 0.0f, 0.0f, 0.0f, 1.0f);
            math::SMatrix44::Transpose(m_Transform, m_Transform);
            math::SVector4::Copy(m_Transform.v[3], m_Pos);

            math::SVector4::Copy(m_X, m_Transform.v[0]);
            math::SVector4::Copy(m_Y, m_Transform.v[1]);
            math::SVector4::Copy(m_Z, m_Transform.v[2]);
        }

        m_NeedUpdateByTransform = IZ_FALSE;
#endif
    }

    if (m_IsDirtyV2C) {
        ComputeV2C();
    }

    if (m_IsDirtyW2V || m_IsDirtyV2C) {
        math::SMatrix44::Mul(m_Param.mtxW2C, m_Param.mtxW2V, m_Param.mtxV2C);

        m_IsDirtyW2V = IZ_FALSE;
        m_IsDirtyV2C = IZ_FALSE;
    }
}

// 移動.
void CVectorCamera::Translate(const math::SVector4& trans)
{
    math::SVector4::AddXYZ(m_Pos, m_Pos, trans);
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
    const math::SVector4& axis,
    IZ_FLOAT rad)
{
    math::SQuat quat;
    math::SQuat::SetQuatFromRadAxis(quat, rad, axis);

	Rotate(quat);
}

void CVectorCamera::Rotate(const math::SQuat& quat)
{
	math::SMatrix44 mtx;
	math::SQuat::MatrixFromQuat(mtx, quat);

    math::SMatrix44::Apply(m_Pos, m_Pos, mtx);

    // 変わらないはずだが、念のため.
    computeLenghtBetweenPosAndAt();

	m_IsDirtyW2V = IZ_TRUE;
}

void CVectorCamera::RotatePose(
    const math::SVector4& axis,
    IZ_FLOAT rad)
{
    math::SQuat quat;
    math::SQuat::SetQuatFromRadAxis(quat, rad, axis);

    RotatePose(quat);
}

void CVectorCamera::RotatePose(const math::SQuat& quat)
{
    math::SMatrix44 mtx;
    math::SQuat::MatrixFromQuat(mtx, quat);

    math::SMatrix44::Apply(m_X, m_X, mtx);
    math::SMatrix44::Apply(m_Y, m_Y, mtx);
    math::SMatrix44::Apply(m_Z, m_Z, mtx);

    math::CVector3 dir(m_Z.x, m_Z.y, m_Z.z);

    math::CVector4 pos(m_Pos);
    auto length = math::SVector4::Length2(m_Pos, m_At);

    auto at = pos + dir * length;
    m_At.Set(at.x, at.y, at.z);

    computeLenghtBetweenPosAndAt();

    m_IsDirtyW2V = IZ_TRUE;
    m_NeedUpdateByTransform = IZ_TRUE;
}

// 位置指定.
void CVectorCamera::SetPos(const math::SVector4& pos)
{
    m_Pos.Set(pos.x, pos.y, pos.z);

    computeLenghtBetweenPosAndAt();

    m_IsDirtyW2V = IZ_TRUE;
}

void CVectorCamera::SetAt(const math::SVector4& at)
{
    m_At.Set(at.x, at.y, at.z);

    computeLenghtBetweenPosAndAt();

    m_IsDirtyW2V = IZ_TRUE;
}

void CVectorCamera::Dump() const
{
    IZ_PRINTF("VectorCamera ====\n");
    IZ_PRINTF("X "); math::SVector4::Dump(m_X);
    IZ_PRINTF("Y "); math::SVector4::Dump(m_Y);
    IZ_PRINTF("Z "); math::SVector4::Dump(m_Z);
    IZ_PRINTF("P "); math::SVector4::Dump(m_Pos);
}