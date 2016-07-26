#include "scenegraph/Camera.h"
#include "izGraph.h"

using namespace izanagi;

// NOTE
// DirectX の座標系は左手系
// OpenGL の座標系は右手系

// 初期化
void CCamera::Init(
    const math::SVector4& vecPos,
    const math::SVector4& vecRef,
    const math::SVector4& vecUp,
    IZ_FLOAT fNear, IZ_FLOAT fFar,
    IZ_FLOAT fFov,
    IZ_FLOAT fAspect)
{
    m_Param.cameraNear = fNear;
    m_Param.cameraFar = fFar;
    m_Param.verticalFOV = fFov;

    m_Param.aspect = fAspect;

    math::SVector4::Copy(m_Param.pos, vecPos);
    math::SVector4::Copy(m_Param.ref, vecRef);
    math::SVector4::Copy(m_Param.up, vecUp);

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
        sParam.verticalFOV,
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
        math::SMatrix44::Mul(m_Param.mtxW2C, m_Param.mtxW2V, m_Param.mtxV2C);

        m_IsDirtyW2V = IZ_FALSE;
        m_IsDirtyV2C = IZ_FALSE;
    }
}

// カメラ座標系でのオフセットを考慮にいれたV2Cマトリクスを取得.
void CCamera::GetOffsetV2C(
    math::SMatrix44& mtxV2C,
    const math::SVector4& pos,
    IZ_FLOAT delta)
{
    // 念のため
    Update();

    // カメラ座標系の位置を計算
    math::SVector4 viewPos;
    math::SMatrix44::Apply(
        viewPos,
        pos,
        m_Param.mtxW2V);

    GetOffsetV2C(
        mtxV2C,
        viewPos.z,
        delta);
}

void CCamera::GetOffsetV2C(
    math::SMatrix44& mtxV2C,
    IZ_FLOAT viewZ,
    IZ_FLOAT delta)
{
    // 念のため
    Update();

    IZ_FLOAT f = m_Param.cameraFar;
    IZ_FLOAT n = m_Param.cameraNear;

    IZ_FLOAT epsilon = -2.0f * f * n * delta;
    epsilon /= ((f + n) * viewZ * (viewZ + delta));

    math::SMatrix44::Copy(mtxV2C, m_Param.mtxV2C);

    mtxV2C.m[2][2] *= (1.0f + epsilon);
}

// 更新済みかどうか.
IZ_BOOL CCamera::IsUpdated() const
{
    return (!m_IsDirtyW2V && !m_IsDirtyV2C);
}

// カメラパラメータを取得.
const SCameraParam& CCamera::GetParam() const
{
    return m_Param;
}

// 視点をセット.
void CCamera::SetPos(const math::SVector4& vecPos)
{
    math::SVector4::Copy(m_Param.pos, vecPos);
    m_IsDirtyW2V = IZ_TRUE;
}

// 注視点をセット.
void CCamera::SetAt(const math::SVector4& vecAt)
{
    math::SVector4::Copy(m_Param.ref, vecAt);
    m_IsDirtyW2V = IZ_TRUE;
}

// 上方向ベクトルをセット.
void CCamera::SetUp(const math::SVector4& vecUp)
{
    math::SVector4::Copy(m_Param.up, vecUp);
    m_IsDirtyW2V = IZ_TRUE;
}

// カメラのnear値をセット.
void CCamera::SetNear(IZ_FLOAT fNear)
{
    m_Param.cameraNear = fNear;
    m_IsDirtyV2C = IZ_TRUE;
}

// カメラのfar値をセット.
void CCamera::SetFar(IZ_FLOAT fFar)
{
    m_Param.cameraFar = fFar;
    m_IsDirtyV2C = IZ_TRUE;
}

// カメラの画角をセット.
void CCamera::SetVerticalFOV(IZ_FLOAT _verticalFOV)
{
    m_Param.verticalFOV = _verticalFOV;
    m_IsDirtyV2C = IZ_TRUE;
}

// カメラのアスペクト比をセット.
void CCamera::SetAspect(IZ_FLOAT fAspect)
{
    m_Param.aspect = fAspect;
    m_IsDirtyV2C = IZ_TRUE;
}

math::SVector4 CCamera::GetDir()
{
    math::SVector4 ret;
    math::SVector4::Sub(ret, m_Param.ref, m_Param.pos);
    return ret;
}

// World - View
void CCamera::ComputeW2V()
{
    math::SMatrix44::SetUnit(m_Param.mtxW2V);

    math::SVector4 vecX;
    math::SVector4 vecY;
    math::SVector4 vecZ;

    vecX.w = vecY.w = vecZ.w = 0.0f;

    // Z
    // 視点から注視点へのベクトル
#ifdef IZ_COORD_LEFT_HAND
    math::SVector4::SubXYZ(vecZ, m_Param.ref, m_Param.pos);  // 左手
#else   // #ifdef VIEW_LH
    math::SVector4::SubXYZ(vecZ, m_Param.pos, m_Param.ref);  // 右手
#endif  // #ifdef VIEW_LH

    vecZ.w = 0.0f;
    math::SVector4::Normalize(vecZ, vecZ);

    // NOTE
    // 右手座標

    // TODO
    // OpenGL

    // X = Y x Z
    math::SVector4::Cross(vecX, m_Param.up, vecZ);
    math::SVector4::Normalize(vecX, vecX);

    // Y = Z x X
    math::SVector4::Cross(vecY, vecZ, vecX);
    vecY.w = 0.0f;

    math::SVector4::Copy(m_Param.mtxW2V.v[0], vecX);
    math::SVector4::Copy(m_Param.mtxW2V.v[1], vecY);
    math::SVector4::Copy(m_Param.mtxW2V.v[2], vecZ);

    math::SMatrix44::Transpose(m_Param.mtxW2V, m_Param.mtxW2V);

    m_Param.mtxW2V.m[3][0] = -1.0f * math::SVector4::Dot(vecX, m_Param.pos);
    m_Param.mtxW2V.m[3][1] = -1.0f * math::SVector4::Dot(vecY, m_Param.pos);
    m_Param.mtxW2V.m[3][2] = -1.0f * math::SVector4::Dot(vecZ, m_Param.pos);
    m_Param.mtxW2V.m[3][3] = 1.0f;
}

// View - Clip
void CCamera::ComputeV2C()
{
    math::SMatrix44::SetUnit(m_Param.mtxV2C);

    // Use Vertical FOV
    const IZ_FLOAT fH = 1 / tanf(m_Param.verticalFOV * 0.5f);
    const IZ_FLOAT fW = fH / m_Param.aspect;

    const IZ_FLOAT fQ = -m_Param.cameraFar / (m_Param.cameraFar - m_Param.cameraNear);

    m_Param.mtxV2C.m[0][0] = fW;
    m_Param.mtxV2C.m[1][1] = fH;

#ifdef IZ_COORD_LEFT_HAND
    // 左手
    m_Param.mtxV2C.m[2][2] = -fQ;
    m_Param.mtxV2C.m[3][2] = fQ * m_Param.cameraNear;
    m_Param.mtxV2C.m[2][3] = 1.0f;
#else   // #ifdef VIEW_LH
    // 右手
    m_Param.mtxV2C.m[2][2] = fQ;
    m_Param.mtxV2C.m[3][2] = fQ * m_Param.cameraNear;
    m_Param.mtxV2C.m[2][3] = -1.0f;
#endif  // #ifdef VIEW_LH

    m_Param.mtxV2C.m[3][3] = 0.0f;
}
