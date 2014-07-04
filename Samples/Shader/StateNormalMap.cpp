#include "StateNormalMap.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

CStateNormalMap::CStateNormalMap(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
    m_Shader = IZ_NULL;
    m_Axis = IZ_NULL;
    m_Plane = IZ_NULL;
    m_Image = IZ_NULL;
}

CStateNormalMap::~CStateNormalMap()
{
    Destroy();
}

// 描画.
IZ_BOOL CStateNormalMap::Render(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::SetUnit(mtxL2W);

    device->SetTexture(0, m_Image->GetTexture(0));
    device->SetTexture(1, m_Image->GetTexture(1));

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
            //m_Shader->SetTexture("texAlbedo", m_Image->GetTexture(0));
            //m_Shader->SetTexture("texNormal", m_Image->GetTexture(1));

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
                SetShaderParam(m_Shader, "g_vLitParallelColor", &m_ParallelLight.color, sizeof(m_ParallelLight.color));
                SetShaderParam(m_Shader, "g_vLitAmbientColor", &m_Ambient.color, sizeof(m_Ambient.color));

                SetShaderParam(m_Shader, "g_vMtrlDiffuse", &m_Mtrl.vDiffuse, sizeof(m_Mtrl.vDiffuse));
                SetShaderParam(m_Shader, "g_vMtrlAmbient", &m_Mtrl.vAmbient, sizeof(m_Mtrl.vAmbient));
                SetShaderParam(m_Shader, "g_vMtrlSpecular", &m_Mtrl.vSpecular, sizeof(m_Mtrl.vSpecular));
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

            m_Plane->Draw(device);
        }
    }
    m_Shader->End(device);

    RenderName(device, "NormalMap");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateNormalMap::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::CValue& arg)
{
    izanagi::graph::CGraphicsDevice* device = reinterpret_cast<izanagi::graph::CGraphicsDevice*>(arg.GetValueAsPtr());

    IZ_BOOL result = IZ_TRUE;

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/NormalMapShader.shd"));

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
                        | izanagi::E_DEBUG_MESH_VTX_FORM_UV
                        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
                        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
                        | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT;

        m_Plane = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            10, 10,
            50.0f, 50.0f);
        VGOTO(result = (m_Plane != IZ_NULL), __EXIT__);
    }

    // テクスチャ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/asset.img"));
        
        m_Image = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);
        VGOTO(result = (m_Image != IZ_NULL), __EXIT__);
    }

    // ライトパラメータ
    {
        // Ambient Light Color
        m_Ambient.color.Set(0.0f, 0.0f, 0.0f);

        // Parallel Light Color
        m_ParallelLight.color.Set(1.0f, 1.0f, 1.0f);

        // Parallel Light Direction
        m_ParallelLight.vDir.Set(0.0f, 0.0f, -1.0f);
        izanagi::math::SVector::Normalize(m_ParallelLight.vDir, m_ParallelLight.vDir);

        // マテリアル
        m_Mtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
        m_Mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
        m_Mtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
    }

    izanagi::math::SMatrix::SetUnit(m_L2W);

__EXIT__:
    if (!result) {
        Leave();
    }

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateNormalMap::Leave()
{
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Axis);
    SAFE_RELEASE(m_Plane);
    SAFE_RELEASE(m_Image);

    return IZ_TRUE;
}
