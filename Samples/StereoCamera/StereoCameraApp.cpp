#include <vector>
#include "StereoCameraApp.h"

StereoCameraApp::StereoCameraApp()
{
    m_Cube = IZ_NULL;
    m_Img = IZ_NULL;

    izanagi::math::SMatrix44::SetUnit(m_L2W);
}

StereoCameraApp::~StereoCameraApp()
{
}

// 初期化.
IZ_BOOL StereoCameraApp::InitInternal(
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

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("shader/vs_equirect.glsl"), __EXIT__);

        std::vector<uint8_t> buf(in.GetSize() + 1);
        in.Read(&buf[0], 0, in.GetSize());

        m_vs = device->CreateVertexShader(&buf[0]);

        VGOTO(result = (m_vs != IZ_NULL), __EXIT__);
    }
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("shader/fs_equirect.glsl"), __EXIT__);

        std::vector<uint8_t> buf(in.GetSize() + 1);
        in.Read(&buf[0], 0, in.GetSize());

        m_fs = device->CreatePixelShader(&buf[0]);

        VGOTO(result = (m_fs != IZ_NULL), __EXIT__);
    }
    {
        m_shd = device->CreateShaderProgram();
        m_shd->AttachVertexShader(m_vs);
        m_shd->AttachPixelShader(m_fs);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f,  0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f,  1.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f,  0.0f, 1.0f),
        0.01f,
        5.0f,
        izanagi::math::CMath::Deg2Rad(90.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void StereoCameraApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Cube);
    SAFE_RELEASE(m_Img);

    SAFE_RELEASE(m_shd);
    SAFE_RELEASE(m_vs);
    SAFE_RELEASE(m_fs);
}

// 更新.
void StereoCameraApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    izanagi::math::SMatrix44::SetScale(m_L2W, 1, 1, 1);

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
void StereoCameraApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
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

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_CULLMODE,
        izanagi::graph::E_GRAPH_CULL_NONE);

    device->SetTexture(0, m_Img->GetTexture(texIdx[m_Idx]));

    device->SetShaderProgram(m_shd);

    auto hL2W = m_shd->GetHandleByName("g_mL2W");
    m_shd->SetMatrix(device, hL2W, m_L2W);

#if 1
    izanagi::HmdInfo info;
    izanagi::FovPort fov;

    // a typical DK1 half-FOV is 46 degrees inwards, 53 degrees outwards
    // https://developer.oculusvr.com/forums/viewtopic.php?p=111699#p111699

    // Left
    {
        izanagi::StereoCamera::setFov(
            izanagi::StereoCamera::Eye::Left,
            46, 53,
            90,
            fov);

        device->SetViewport(
            izanagi::graph::SViewport(
            0, 0,
            SCREEN_WIDTH / 2, SCREEN_HEIGHT));

        izanagi::CVectorCamera cameraL;

        izanagi::StereoCamera::getCamera(
            izanagi::StereoCamera::Eye::Left,
            info,
            fov,
            cameraL,
            camera);

        auto mtxW2C = cameraL.GetParam().mtxW2C;

        auto hW2C = m_shd->GetHandleByName("g_mW2C");
        m_shd->SetMatrix(device, hW2C, mtxW2C);

        auto hEye = m_shd->GetHandleByName("g_vEye");
        m_shd->SetVector(device, hEye, cameraL.GetParam().pos);

        m_Cube->Render(device);
    }

    // Right
    {
        izanagi::StereoCamera::setFov(
            izanagi::StereoCamera::Eye::Right,
            46, 53,
            90,
            fov);

        device->SetViewport(
            izanagi::graph::SViewport(
            SCREEN_WIDTH / 2, 0,
            SCREEN_WIDTH / 2, SCREEN_HEIGHT));

        izanagi::CVectorCamera cameraR;

        izanagi::StereoCamera::getCamera(
            izanagi::StereoCamera::Eye::Right,
            info,
            fov,
            cameraR,
            camera);

        auto mtxW2C = cameraR.GetParam().mtxW2C;

        auto hW2C = m_shd->GetHandleByName("g_mW2C");
        m_shd->SetMatrix(device, hW2C, mtxW2C);

        auto hEye = m_shd->GetHandleByName("g_vEye");
        m_shd->SetVector(device, hEye, cameraR.GetParam().pos);

        m_Cube->Render(device);
    }

    device->SetViewport(
        izanagi::graph::SViewport(
        0, 0,
        SCREEN_WIDTH, SCREEN_HEIGHT));
#else
    auto mtxW2C = camera.GetParam().mtxW2C;

    auto hW2C = m_shd->GetHandleByName("g_mW2C");
    m_shd->SetMatrix(device, hW2C, mtxW2C);

    auto hEye = m_shd->GetHandleByName("g_vEye");
    m_shd->SetVector(device, hEye, camera.GetParam().pos);

    m_Cube->Render(device);
#endif

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

IZ_BOOL StereoCameraApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}
