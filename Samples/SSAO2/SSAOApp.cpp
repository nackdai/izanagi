#include "SSAOApp.h"
#include "izGraph.h"
#include "izSystem.h"

SSAOApp::SSAOApp()
{
}

SSAOApp::~SSAOApp()
{
}

// 開始
IZ_BOOL SSAOApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    m_shd.init(
        device,
        "shader/vs_default.glsl",
        "shader/ps_default.glsl");

    m_obj = izanagi::sample::ObjModel::create(
        allocator,
        device,
        "../../Media/teapot/teapot.obj");

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 10.0f, 300.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return result;
}

// 更新.
void SSAOApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
}

// 描画.
void SSAOApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    const auto& camera = GetCamera();

    izanagi::math::CMatrix44 mtxL2W;
    const auto& mtxW2C = camera.GetParam().mtxW2C;
    izanagi::math::CVector4 color(1.0f, 0.0f, 0.0f, 1.0f);

    auto* shd = m_shd.m_program;

    device->SetShaderProgram(shd);

    auto hL2W = shd->GetHandleByName("mtxL2W");
    shd->SetMatrix(device, hL2W, mtxL2W);

    auto hW2C = shd->GetHandleByName("mtxW2C");
    shd->SetMatrix(device, hW2C, mtxW2C);

    auto hClr = shd->GetHandleByName("color");
    shd->SetVector(device, hClr, color);

    m_obj->render(device);
}

void SSAOApp::ReleaseInternal()
{
    SAFE_RELEASE(m_obj);

    m_shd.release();
}
