#include "Environment.h"
#include "Utility.h"
#include "Configure.h"

Environment Environment::s_Instance;

Environment& Environment::Instance()
{
    return s_Instance;
}

void Environment::SetGlobalLightParam(
    const izanagi::CCamera& camera,
    izanagi::shader::CShaderBasic* shader)
{
    // Ambient Light Color
    izanagi::SAmbientLightParam ambient;
    ambient.color.Set(0.4f, 0.4f, 0.4f);

    // Parallel Light Color
    m_ParallelLight.color.Set(0.8f, 0.8f, 0.8f);

    // Parallel Light Direction
    //m_ParallelLight.vDir.Set(1.0f, -1.0f, 1.0f);
    m_ParallelLight.vDir.Set(6.0f, -5.0f, 1.0f);
    izanagi::math::SVector4::Normalize(m_ParallelLight.vDir, m_ParallelLight.vDir);

    // マテリアル
    izanagi::SMaterialParam mtrl;
    {
        mtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
        mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
        mtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
    }

    Utility::SetShaderParam(shader, "g_vLitParallelColor", &m_ParallelLight.color, sizeof(m_ParallelLight.color));
    Utility::SetShaderParam(shader, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));

    Utility::SetShaderParam(shader, "g_vMtrlDiffuse", &mtrl.vDiffuse, sizeof(mtrl.vDiffuse));
    Utility::SetShaderParam(shader, "g_vMtrlAmbient", &mtrl.vAmbient, sizeof(mtrl.vAmbient));
    Utility::SetShaderParam(shader, "g_vMtrlSpecular", &mtrl.vSpecular, sizeof(mtrl.vSpecular));
}

void Environment::SetLocalLightParam(
    const izanagi::CCamera& camera,
    const izanagi::math::SMatrix44& mtxL2W,
    izanagi::shader::CShaderBasic* shader)
{
    const izanagi::SCameraParam& param = camera.GetParam();

    // ライトの方向をローカル座標に変換する

    // ライトの方向はワールド座標なので World -> Localマトリクスを計算する
    izanagi::math::SMatrix44 mtxW2L;
    izanagi::math::SMatrix44::Inverse(mtxW2L, mtxL2W);

    // World -> Local
    izanagi::math::SVector4 parallelLightLocalDir;
    izanagi::math::SMatrix44::ApplyXYZ(
        parallelLightLocalDir,
        m_ParallelLight.vDir,
        mtxW2L);

    Utility::SetShaderParam(
        shader,
        "g_vLitParallelDir",
        (void*)&parallelLightLocalDir,
        sizeof(parallelLightLocalDir));

    // L2V = L2W * W2V の逆行列を計算する
    izanagi::math::SMatrix44 mtxV2L;
    izanagi::math::SMatrix44::Mul(mtxV2L, mtxL2W, param.mtxW2V);
    izanagi::math::SMatrix44::Inverse(mtxV2L, mtxV2L);

    // ビュー座標系における視点は常に原点
    izanagi::math::CVector4 eyePos(0.0f, 0.0f, 0.0f, 1.0f);

    // 視点のローカル座標を計算する
    izanagi::math::SMatrix44::Apply(eyePos, eyePos, mtxV2L);

    Utility::SetShaderParam(
        shader,
        "g_vEye",
        (void*)&eyePos,
        sizeof(eyePos));
}
