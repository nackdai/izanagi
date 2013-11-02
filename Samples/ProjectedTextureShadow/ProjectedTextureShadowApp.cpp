#include "ProjectedTextureShadowApp.h"
#include "Shadow.h"

CProjectedTextureShadowApp::CProjectedTextureShadowApp()
{
    m_Torus = IZ_NULL;
    m_Plane = IZ_NULL;

    m_Axis = IZ_NULL;

    m_Img = IZ_NULL;

    m_Shader = IZ_NULL;

    m_Shadow = IZ_NULL;
}

CProjectedTextureShadowApp::~CProjectedTextureShadowApp()
{
}

// 初期化.
IZ_BOOL CProjectedTextureShadowApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // Axis
    m_Axis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
                allocator,
                device);
    VGOTO(result = (m_Axis != IZ_NULL), __EXIT__);

    // Torus
    {
        IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
                    | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

        m_Torus = izanagi::CDebugMeshTorus::CreateDebugMeshTorus(
                    allocator,
                    device,
                    flag,
                    IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                    2.0f, 5.0f,
                    10, 10);
        VGOTO(result = (m_Torus != IZ_NULL), __EXIT__);
    }

    // Plane
    {
        IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

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
        VGOTO(result = in.Open("data/earth.img"), __EXIT__);

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/BasicShader.shd"), __EXIT__);

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // 影用
    {
        m_Shadow = CShadow::Create(allocator, device, 640, 640);
        VGOTO(result = (m_Shadow != IZ_NULL), __EXIT__);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector(0.0f, 0.0f, 30.0f, 1.0f),
        izanagi::math::CVector(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void CProjectedTextureShadowApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Torus);
    SAFE_RELEASE(m_Plane);

    SAFE_RELEASE(m_Axis);

    SAFE_RELEASE(m_Img);

    SAFE_RELEASE(m_Shader);

    SAFE_RELEASE(m_Shadow);
}

// 更新.
void CProjectedTextureShadowApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
}

namespace {
    inline void _SetShaderParam(
        izanagi::shader::CShaderBasic* shader,
        const char* name,
        const void* value,
        IZ_UINT bytes)
    {
        izanagi::shader::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
        IZ_ASSERT(handle != 0);

        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
}

// 描画.
void CProjectedTextureShadowApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    RenderForShadow(device);

    izanagi::sample::CSampleCamera& camera = GetCamera();

    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::SetUnit(mtxL2W);

    // テクスチャなし
    m_Shader->Begin(1, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            // シェーダ設定
            m_Shader->CommitChanges();

            m_Axis->Draw(device);

            m_Shader->EndPass();
        }
    }
    m_Shader->End();

    // 影テクスチャセット
    device->SetTexture(0, m_Shadow->GetRT());

    // 影付き
    m_Shader->Begin(3, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(izanagi::math::SMatrix));

            izanagi::math::SMatrix mtxShadowTex;
            izanagi::math::SMatrix::Mul(
                mtxShadowTex,
                m_Shadow->GetLightViewProjMtx(),
                m_Shadow->GetShadowTexMtx());

            _SetShaderParam(
                m_Shader,
                "g_mShadowTex",
                (void*)&mtxShadowTex,
                sizeof(izanagi::math::SMatrix));

            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix));

                m_Shader->CommitChanges();

                m_Plane->Draw(device);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End();

    device->SetTexture(0, m_Img->GetTexture(0));

    // テクスチャあり
    m_Shader->Begin(0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(izanagi::math::SMatrix));

            // Torus
            {
                izanagi::math::SMatrix::GetTrans(
                    mtxL2W,
                    izanagi::math::CVector(0.0f, 5.0f, 0.0f));

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix));

                m_Shader->CommitChanges();

                m_Torus->Draw(device);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End();
}

void CProjectedTextureShadowApp::RenderForShadow(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::SetUnit(mtxL2W);

    m_Shadow->BeginShadowRender(
        device,
        izanagi::math::CVector(10.0f, 10.0f, 10.0f),    // ライトの位置
        izanagi::math::CVector(-1.0f, -1.0f, -1.0f));    // ライトの向き

    // 影用
    m_Shader->Begin(2, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&m_Shadow->GetLightViewProjMtx(),
                sizeof(izanagi::math::SMatrix));

            // Torus
            {
                izanagi::math::SMatrix::GetTrans(
                    mtxL2W,
                    izanagi::math::CVector(0.0f, 5.0f, 0.0f));

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix));

                m_Shader->CommitChanges();

                m_Torus->Draw(device);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End();

    m_Shadow->EndShadowRender(device);
}
