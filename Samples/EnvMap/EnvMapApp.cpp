#include "EnvMapApp.h"

CEnvMapApp::CEnvMapApp()
{
    m_Cube = IZ_NULL;
    m_Img = IZ_NULL;
    m_Shader = IZ_NULL;

    izanagi::math::SMatrix44::SetUnit(m_L2W);

    m_Idx = 0;
}

CEnvMapApp::~CEnvMapApp()
{
}

// 初期化.
IZ_BOOL CEnvMapApp::InitInternal(
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
void CEnvMapApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Cube);
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Shader);
}

// 更新.
void CEnvMapApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
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
void CEnvMapApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    static IZ_INT texIdx[] = {
        1,
        3,
        4,
    };

    static IZ_CHAR* name[] = {
        "Cube",
        "Latitude-Longitude",
        "Angular",
    };

    device->SetTexture(0, m_Img->GetTexture(texIdx[m_Idx]));

    // テクスチャあり
    m_Shader->Begin(device, m_Idx, IZ_FALSE);
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

            _SetShaderParam(
                m_Shader,
                "g_vEye",
                (void*)&camera.GetParam().pos,
                sizeof(izanagi::math::SVector4));

            m_Shader->CommitChanges(device);

            m_Cube->Render(device);

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    if (device->Begin2D()) {
        izanagi::CDebugFont* debugFont = GetDebugFont();

        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        debugFont->DBPrint(
            device, 
            "%s\n",
            name[m_Idx]);

        debugFont->End();

        device->End2D();
    }
}

IZ_BOOL CEnvMapApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (m_Shader != IZ_NULL) {
        if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
            m_Idx--;
        }
        else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
            m_Idx++;
        }

        IZ_INT techNum = m_Shader->GetTechNum();
        m_Idx = (m_Idx >= techNum ? 0 : m_Idx);
        m_Idx = (m_Idx < 0 ? techNum - 1 : m_Idx);
    }

    return IZ_TRUE;
}
