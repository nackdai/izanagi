#include "StateSSAO.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

CStateSSAO::CStateSSAO(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
    m_Shader = IZ_NULL;
    m_Sphere = IZ_NULL;
    m_Cube = IZ_NULL;
    m_Plane = IZ_NULL;
}

CStateSSAO::~CStateSSAO()
{
    Destroy();
}

// 描画.
IZ_BOOL CStateSSAO::Render(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::SetUnit(mtxL2W);

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

            // 地面
            RenderScene(
                device, 
                m_Plane,
                izanagi::math::CVector(0.0f, 0.0f, 0.0f));

            // 球
            RenderScene(
                device, 
                m_Sphere,
                izanagi::math::CVector(10.0f, 5.0f, 10.0f));
            RenderScene(
                device, 
                m_Sphere,
                izanagi::math::CVector(10.0f, 5.0f, -10.0f));

            // キューブ
            RenderScene(
                device, 
                m_Cube,
                izanagi::math::CVector(-10.0f, 5.0f, -10.0f));
            RenderScene(
                device, 
                m_Cube,
                izanagi::math::CVector(-10.0f, 5.0f, 10.0f));
        }
    }
    m_Shader->End(device);

    RenderName(device, "SSAO");

    return IZ_TRUE;
}

void CStateSSAO::RenderScene(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CDebugMesh* mesh,
    const izanagi::math::SVector& position)
{
    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::GetTrans(mtxL2W, position);

    SetShaderParam(
        m_Shader,
        "g_mL2W",
        (void*)&mtxL2W,
        sizeof(mtxL2W));

    m_Shader->CommitChanges(device);

    mesh->Draw(device);
}

// 開始
IZ_BOOL CStateSSAO::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
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

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;
    IZ_COLOR color = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);

    // 球
    {
        m_Sphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
                        allocator,
                        device,
                        flag,
                        color,
                        5.0f,
                        20, 20);
        VGOTO(result = (m_Sphere != IZ_NULL), __EXIT__);
    }

    // キューブ
    {
        m_Cube = izanagi::CDebugMeshBox::CreateDebugMeshBox(
            allocator,
            device,
            flag,
            color,
            10.0f, 10.0f, 10.0f);
        VGOTO(result = (m_Cube != IZ_NULL), __EXIT__);
    }

    // 地面
    {
        m_Plane = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
            allocator,
            device,
            flag,
            color,
            10, 10,
            50.0f, 50.0f);
        VGOTO(result = (m_Plane != IZ_NULL), __EXIT__);
    }

    izanagi::math::SMatrix::SetUnit(m_L2W);

__EXIT__:
    if (!result) {
        Leave();
    }

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateSSAO::Leave()
{
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Sphere);
    SAFE_RELEASE(m_Cube);
    SAFE_RELEASE(m_Plane);

    return IZ_TRUE;
}
