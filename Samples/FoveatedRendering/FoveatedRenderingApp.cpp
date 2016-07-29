#include <vector>
#include "FoveatedRenderingApp.h"

bool FoveatedRenderingApp::Shader::Init(
    izanagi::graph::CGraphicsDevice* device,
    const char* pathVs, const char* pathPs)
{
    // シェーダ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(pathVs));

        std::vector<uint8_t> buf(in.GetSize() + 1);
        in.Read(&buf[0], 0, in.GetSize());

        m_vs = device->CreateVertexShader(&buf[0]);

        VRETURN(m_vs != IZ_NULL);
    }
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(pathPs));

        std::vector<uint8_t> buf(in.GetSize() + 1);
        in.Read(&buf[0], 0, in.GetSize());

        m_fs = device->CreatePixelShader(&buf[0]);

        VRETURN(m_fs != IZ_NULL);
    }
    {
        m_shd = device->CreateShaderProgram();
        m_shd->AttachVertexShader(m_vs);
        m_shd->AttachPixelShader(m_fs);
    }

    return true;
}

void FoveatedRenderingApp::Shader::Release()
{
    SAFE_RELEASE(m_vs);
    SAFE_RELEASE(m_fs);
    SAFE_RELEASE(m_shd);
}

FoveatedRenderingApp::FoveatedRenderingApp()
{
    m_Cube = IZ_NULL;
    m_Img = IZ_NULL;

    izanagi::math::SMatrix44::SetUnit(m_L2W);
}

FoveatedRenderingApp::~FoveatedRenderingApp()
{
}

// 初期化.
IZ_BOOL FoveatedRenderingApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS;

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

    // FBO.
    {
        m_rt = device->CreateRenderTarget(
            SCREEN_WIDTH, SCREEN_HEIGHT,
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);

        m_depth = device->CreateRenderTarget(
            SCREEN_WIDTH, SCREEN_HEIGHT,
            izanagi::graph::E_GRAPH_PIXEL_FMT_D24S8);

        m_mask = device->CreateRenderTarget(
            SCREEN_WIDTH, SCREEN_HEIGHT,
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    }

    m_shdDrawCube.Init(
        device,
        "shader/vs_equirect.glsl",
        "shader/fs_equirect.glsl");
    m_shdMakeMask.Init(
        device,
        "shader/vs_2dscreen.glsl",
        "shader/fs_radicaldensitymask.glsl");
    m_shdFilter.Init(
        device,
        "shader/vs_2dscreen.glsl",
        "shader/fs_filter.glsl");

    IZ_FLOAT aspect = (IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT;
    IZ_FLOAT verticalFOV = izanagi::math::CMath::Deg2Rad(60.0f);

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f,  0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f,  1.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f,  0.0f, 1.0f),
        1.0f,
        500.0f,
        verticalFOV,
        aspect);
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void FoveatedRenderingApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Cube);
    SAFE_RELEASE(m_Img);

    m_shdMakeMask.Release();
    m_shdDrawCube.Release();
    m_shdFilter.Release();

    SAFE_RELEASE(m_mask);

    SAFE_RELEASE(m_rt);
    SAFE_RELEASE(m_depth);
}

// 更新.
void FoveatedRenderingApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
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
void FoveatedRenderingApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (m_canFoveated) {
        FoveatedRender(device);
    }
    else {
        DefaultRender(device);
    }

    if (device->Begin2D()) {
        izanagi::CDebugFont* debugFont = GetDebugFont();

        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        debugFont->DBPrint(
            device,
            "%s\n",
            m_canFoveated ? "Foveated" : "Default");

        debugFont->End();

        device->End2D();
    }
}

void FoveatedRenderingApp::DefaultRender(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    device->SetTexture(0, m_Img->GetTexture(m_Idx));
    device->SetTexture(1, m_mask);

    auto shd = m_shdDrawCube.m_shd;

    device->SetShaderProgram(shd);

    auto hL2W = shd->GetHandleByName("g_mL2W");
    shd->SetMatrix(device, hL2W, m_L2W);

    auto mtxW2C = camera.GetParam().mtxW2C;

    auto hW2C = shd->GetHandleByName("g_mW2C");
    shd->SetMatrix(device, hW2C, mtxW2C);

    auto hEye = shd->GetHandleByName("g_vEye");
    shd->SetVector(device, hEye, camera.GetParam().pos);

    auto hInvScr = shd->GetHandleByName("invScreen");
    izanagi::math::CVector4 invScr(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT, 0, 0);
    shd->SetVector(device, hInvScr, invScr);

    auto hCanFoveated = shd->GetHandleByName("canFoveated");
    shd->SetBool(device, hCanFoveated, false);

    m_Cube->Render(device);
}

void FoveatedRenderingApp::FoveatedRender(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    device->BeginScene(
        &m_mask, 1,
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL);
    {
        // For rendering full screen quad without vertex buffer.
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_CULLMODE,
            izanagi::graph::E_GRAPH_CULL_NONE);

        auto shd = m_shdMakeMask.m_shd;

        device->SetShaderProgram(shd);

        auto hInvScr = shd->GetHandleByName("invScreen");
        izanagi::math::CVector4 invScr(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT, 0, 0);
        shd->SetVector(device, hInvScr, invScr);

        auto hCenter = shd->GetHandleByName("center");
        izanagi::math::CVector4 center(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0, 0);
        shd->SetVector(device, hCenter, center);

        CALL_GL_API(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    }
    device->EndScene();

    device->BeginScene(
        &m_rt, 1,
        m_depth,
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL);
    {
        device->SetTexture(0, m_Img->GetTexture(m_Idx));
        device->SetTexture(1, m_mask);

        auto shd = m_shdDrawCube.m_shd;

        device->SetShaderProgram(shd);

        auto hL2W = shd->GetHandleByName("g_mL2W");
        shd->SetMatrix(device, hL2W, m_L2W);

        auto mtxW2C = camera.GetParam().mtxW2C;

        auto hW2C = shd->GetHandleByName("g_mW2C");
        shd->SetMatrix(device, hW2C, mtxW2C);

        auto hEye = shd->GetHandleByName("g_vEye");
        shd->SetVector(device, hEye, camera.GetParam().pos);

        auto hInvScr = shd->GetHandleByName("invScreen");
        izanagi::math::CVector4 invScr(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT, 0, 0);
        shd->SetVector(device, hInvScr, invScr);

        auto hCanFoveated = shd->GetHandleByName("canFoveated");
        shd->SetBool(device, hCanFoveated, true);

        m_Cube->Render(device);
    }
    device->EndScene();

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
        IZ_FALSE);
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZENABLE,
        IZ_FALSE);

    {
        auto shd = m_shdFilter.m_shd;

        device->SetShaderProgram(shd);

        m_rt->SetFilter(
            izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
            izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
            izanagi::graph::E_GRAPH_TEX_FILTER_NONE);
        m_rt->SetAddress(
            izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP,
            izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP);

        device->SetTexture(0, m_rt);
        auto hTex0 = shd->GetHandleByName("s0");
        CALL_GL_API(glUniform1i(hTex0, 0));

        device->SetTexture(1, m_mask);
        auto hTex1 = shd->GetHandleByName("s1");
        CALL_GL_API(glUniform1i(hTex1, 1));

        auto hInvScr = shd->GetHandleByName("invScreen");
        izanagi::math::CVector4 invScr(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT, 0, 0);
        shd->SetVector(device, hInvScr, invScr);

        CALL_GL_API(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    }
}

IZ_BOOL FoveatedRenderingApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_RETURN) {
        m_canFoveated = !m_canFoveated;
    }
    return IZ_TRUE;
}
