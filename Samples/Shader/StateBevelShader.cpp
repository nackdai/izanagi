#include "StateBevelShader.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

CStateBevelShader::CStateBevelShader(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
    m_Shader = IZ_NULL;
    m_Mesh = IZ_NULL;
}

CStateBevelShader::~CStateBevelShader()
{
    Destroy();
}

// 描画.
IZ_BOOL CStateBevelShader::Render(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
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

            izanagi::SParallelLightParam parallelLight;

            // ライトパラメータ
            {
                // Ambient Light Color
                izanagi::SAmbientLightParam ambient;
                ambient.color.Set(0.0f, 0.0f, 0.0f);

                // Parallel Light Color
                parallelLight.color.Set(1.0f, 1.0f, 1.0f);

                // Parallel Light Direction
                parallelLight.vDir.Set(-1.0f, -1.0f, -1.0f);
                izanagi::math::SVector4::Normalize(parallelLight.vDir, parallelLight.vDir);

                // マテリアル
                izanagi::SMaterialParam mtrl;
                {
                    mtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
                    mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
                    mtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
                }

                SetShaderParam(m_Shader, "g_vLitParallelColor", &parallelLight.color, sizeof(parallelLight.color));
                SetShaderParam(m_Shader, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));

                SetShaderParam(m_Shader, "g_vMtrlDiffuse", &mtrl.vDiffuse, sizeof(mtrl.vDiffuse));
                SetShaderParam(m_Shader, "g_vMtrlAmbient", &mtrl.vAmbient, sizeof(mtrl.vAmbient));
                SetShaderParam(m_Shader, "g_vMtrlSpecular", &mtrl.vSpecular, sizeof(mtrl.vSpecular));
            }

            {
                // ライトの方向をローカル座標に変換する

                // ライトの方向はワールド座標なので World -> Localマトリクスを計算する
                izanagi::math::SMatrix44 mtxW2L;
                izanagi::math::SMatrix44::Inverse(mtxW2L, mtxL2W);

                // World -> Local
                izanagi::math::SVector4 parallelLightLocalDir;
                izanagi::math::SMatrix44::ApplyXYZ(
                    parallelLightLocalDir,
                    parallelLight.vDir,
                    mtxW2L);

                SetShaderParam(
                    m_Shader,
                    "g_vLitParallelDir",
                    (void*)&parallelLightLocalDir,
                    sizeof(parallelLightLocalDir));

                // L2V = L2W * W2V の逆行列を計算する
                izanagi::math::SMatrix44 mtxV2L;
                izanagi::math::SMatrix44::Mul(mtxV2L, mtxL2W, m_Camera.mtxW2V);
                izanagi::math::SMatrix44::Inverse(mtxV2L, mtxV2L);

                // ビュー座標系における視点は常に原点
                izanagi::math::CVector4 eyePos(0.0f, 0.0f, 0.0f, 1.0f);

                // 視点のローカル座標を計算する
                izanagi::math::SMatrix44::Apply(eyePos, eyePos, mtxV2L);

                SetShaderParam(
                    m_Shader,
                    "g_vEye",
                    (void*)&eyePos,
                    sizeof(eyePos));
            }

            m_Shader->CommitChanges(device);

            m_Mesh->Draw(device);
        }
    }
    m_Shader->End(device);

    RenderName(device, "BevelShader");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateBevelShader::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    IZ_BOOL result = IZ_TRUE;

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/BevelShader.shd"));

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // メッシュ
    {
        IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

        m_Mesh = BevelShaderMesh::Create(
                        allocator,
                        device,
                        flag,
                        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                        5.0f, 5.0f, 5.0f);
        VGOTO(result = (m_Mesh != IZ_NULL), __EXIT__);
    }

__EXIT__:
    if (!result) {
        Leave();
    }

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateBevelShader::Leave()
{
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Mesh);

    return IZ_TRUE;
}
