#include "StatePhongShader.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

CStatePhongShader::CStatePhongShader(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
    m_Shader = IZ_NULL;
    m_Sphere = IZ_NULL;
    m_Axis = IZ_NULL;
}

CStatePhongShader::~CStatePhongShader()
{
    Destroy();
}

// 描画.
IZ_BOOL CStatePhongShader::Render(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::SetUnit(mtxL2W);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(1))
        {
            SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&m_Camera.mtxW2C,
                sizeof(m_Camera.mtxW2C));

            m_Shader->CommitChanges(device);

            m_Axis->Draw(device);

            m_Shader->EndPass();
        }

        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&m_Camera.mtxW2C,
                sizeof(m_Camera.mtxW2C));

            // ライトパラメータ
            {
                // Ambient Light Color
                izanagi::SAmbientLightParam ambient;
                ambient.color.Set(0.0f, 0.0f, 0.0f);

                // Parallel Light Color
                m_ParallelLight.color.Set(1.0f, 1.0f, 1.0f);

                // Parallel Light Direction
                m_ParallelLight.vDir.Set(-1.0f, -1.0f, -1.0f);
                izanagi::math::SVector::Normalize(m_ParallelLight.vDir, m_ParallelLight.vDir);

                // マテリアル
                izanagi::SMaterialParam mtrl;
                {
                    mtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
                    mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
                    mtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
                }

                SetShaderParam(m_Shader, "g_vLitParallelColor", &m_ParallelLight.color, sizeof(m_ParallelLight.color));
                SetShaderParam(m_Shader, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));

                SetShaderParam(m_Shader, "g_vMtrlDiffuse", &mtrl.vDiffuse, sizeof(mtrl.vDiffuse));
                SetShaderParam(m_Shader, "g_vMtrlAmbient", &mtrl.vAmbient, sizeof(mtrl.vAmbient));
                SetShaderParam(m_Shader, "g_vMtrlSpecular", &mtrl.vSpecular, sizeof(mtrl.vSpecular));
            }

            {
                // ライトの方向をローカル座標に変換する

                // ライトの方向はワールド座標なので World -> Localマトリクスを計算する
                izanagi::math::SMatrix mtxW2L;
                izanagi::math::SMatrix::Inverse(mtxW2L, m_L2W);

                // World -> Local
                izanagi::math::SVector parallelLightLocalDir;
                izanagi::math::SMatrix::ApplyXYZ(
                    parallelLightLocalDir,
                    m_ParallelLight.vDir,
                    mtxW2L);

                SetShaderParam(
                    m_Shader,
                    "g_vLitParallelDir",
                    (void*)&parallelLightLocalDir,
                    sizeof(parallelLightLocalDir));

                // L2V = L2W * W2V の逆行列を計算する
                izanagi::math::SMatrix mtxV2L;
                izanagi::math::SMatrix::Mul(mtxV2L, m_L2W, m_Camera.mtxW2V);
                izanagi::math::SMatrix::Inverse(mtxV2L, mtxV2L);

                // ビュー座標系における視点は常に原点
                izanagi::math::CVector eyePos(0.0f, 0.0f, 0.0f, 1.0f);

                // 視点のローカル座標を計算する
                izanagi::math::SMatrix::Apply(eyePos, eyePos, mtxV2L);

                SetShaderParam(
                    m_Shader,
                    "g_vEye",
                    (void*)&eyePos,
                    sizeof(eyePos));
            }

            m_Shader->CommitChanges(device);

            m_Sphere->Draw(device);
        }
    }
    m_Shader->End(device);

    RenderName(device, "PhongShader");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStatePhongShader::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::CValue& arg)
{
    izanagi::graph::CGraphicsDevice* device = reinterpret_cast<izanagi::graph::CGraphicsDevice*>(arg.GetValueAsPtr());

    IZ_BOOL result = IZ_TRUE;

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/PhongShader.shd"));

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // XYZ軸
    {
        m_Axis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(allocator, device);
        VGOTO(result = (m_Axis != IZ_NULL), __EXIT__);
    }

    // 球
    {
        IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
                        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

        m_Sphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
                        allocator,
                        device,
                        flag,
                        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                        5.0f,
                        20, 20);
        VGOTO(result = (m_Sphere != IZ_NULL), __EXIT__);
    }

    izanagi::math::SMatrix::SetUnit(m_L2W);

__EXIT__:
    if (!result) {
        Leave();
    }

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStatePhongShader::Leave()
{
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Sphere);
    SAFE_RELEASE(m_Axis);

    return IZ_TRUE;
}
