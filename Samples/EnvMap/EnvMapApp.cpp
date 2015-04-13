#include "EnvMapApp.h"

CCubeMapApp::CCubeMapApp()
{
    m_Cube = IZ_NULL;
    m_Img = IZ_NULL;
    m_Shader = IZ_NULL;

    izanagi::math::SMatrix44::SetUnit(m_L2W);
}

CCubeMapApp::~CCubeMapApp()
{
}

// 初期化.
IZ_BOOL CCubeMapApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

    // Cube
    {
        m_Cube = izanagi::sample::CSampleEnvBox::CreateSampleEnvBox(
            allocator,
            device);
        VGOTO(result = (m_Cube != IZ_NULL), __EXIT__);
    }

    // テクスチャ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/EnvMap.img"), __EXIT__);

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/EnvMapBoxShader.shd"), __EXIT__);

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f,  0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, -1.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f,  0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void CCubeMapApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Cube);
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Shader);
}

// 更新.
void CCubeMapApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    izanagi::math::SMatrix44::SetScale(m_L2W, 100.0f, 100.0f, 100.0f);

    // カメラの位置にあわせて移動する
    izanagi::math::SMatrix44::Trans(
        m_L2W,
        m_L2W,
        GetCamera().GetParam().pos);
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
void CCubeMapApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    device->SetTexture(0, m_Img->GetTexture(1));

    // テクスチャあり
    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(izanagi::math::SMatrix44));

            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&m_L2W,
                sizeof(m_L2W));

            m_Shader->CommitChanges(device);

            m_Cube->Render(device);

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);
}
