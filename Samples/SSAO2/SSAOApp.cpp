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

    m_shdGeometryPass.init(
        device,
        "shader/vs_geometry.glsl",
        "shader/ps_geometry.glsl");
    m_shdSSAOPass.init(
        device,
        "shader/vs_fillscreen.glsl",
        "shader/ps_ssao.glsl");

    m_obj = izanagi::sample::ObjModel::create(
        allocator,
        device,
        "../../Media/teapot/teapot.obj");

    m_gbuffer.init(allocator, device);

    m_screenFillPlane = izanagi::CDebugMeshScreenFillPlane::create(
        allocator,
        device,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff));
    IZ_ASSERT(m_screenFillPlane != IZ_NULL);

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, 300.0f, 1.0f),
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
    renderGeometryPass(device);
    renderSSAOPass(device);

    device->SetTexture(0, IZ_NULL);
    m_gbuffer.drawBuffers(device);
}

void SSAOApp::renderGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    const auto& camera = GetCamera();

    izanagi::math::CMatrix44 mtxL2W;
    const auto& mtxW2C = camera.GetParam().mtxW2C;
    izanagi::math::CVector4 color(1.0f, 0.0f, 0.0f, 1.0f);

    auto* shd = m_shdGeometryPass.m_program;

    device->SetShaderProgram(shd);

    auto hL2W = shd->GetHandleByName("mtxL2W");
    shd->SetMatrix(device, hL2W, mtxL2W);

    auto hW2C = shd->GetHandleByName("mtxW2C");
    shd->SetMatrix(device, hW2C, mtxW2C);

    auto hClr = shd->GetHandleByName("color");
    shd->SetVector(device, hClr, color);

    auto hFar = shd->GetHandleByName("depthFar");
    shd->SetFloat(device, hFar, camera.GetParam().cameraFar);

    m_gbuffer.beginGeometryPass(device);

    m_obj->render(device);

    m_gbuffer.endGeometryPass(device);
}

void SSAOApp::renderSSAOPass(izanagi::graph::CGraphicsDevice* device)
{
    device->SaveRenderState();

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZENABLE,
        IZ_FALSE);
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
        IZ_FALSE);

    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    const auto& camera = GetCamera();

    const auto& mtxW2C = camera.GetParam().mtxW2C;
    
    izanagi::math::CMatrix44 mtxC2V, mtxV2W;
    izanagi::math::SMatrix44::Inverse(mtxC2V, camera.GetParam().mtxV2C);
    izanagi::math::SMatrix44::Inverse(mtxV2W, camera.GetParam().mtxW2V);

    float depth = 200.0f;

    izanagi::math::CVector4 pos(10, 10, 100.0f, 1);
    izanagi::math::SMatrix44::Apply(pos, pos, mtxW2C);

    pos.x /= pos.w;
    pos.y /= pos.w;

    pos.z = 0.0f; 
    pos.w = 1.0f;

    pos.x *= depth;
    pos.y *= depth;

    izanagi::math::SMatrix44::Apply(pos, pos, mtxC2V);
    pos.z = depth;

    izanagi::math::SMatrix44::Apply(pos, pos, mtxV2W);

    auto* shd = m_shdSSAOPass.m_program;

    device->SetShaderProgram(shd);

    auto hC2V = shd->GetHandleByName("mtxC2V");
    shd->SetMatrix(device, hC2V, mtxC2V);

    auto hV2W = shd->GetHandleByName("mtxV2W");
    shd->SetMatrix(device, hV2W, mtxV2W);

    auto hW2C = shd->GetHandleByName("mtxW2C");
    shd->SetMatrix(device, hW2C, mtxW2C);

    auto hInvScr = shd->GetHandleByName("invScreen");
    izanagi::math::CVector4 invScr(1.0f / width, 1.0f / height, 0.0f, 1.0f);
    shd->SetVector(device, hInvScr, invScr);

    auto hFar = shd->GetHandleByName("depthFar");
    shd->SetFloat(device, hFar, camera.GetParam().cameraFar);

    m_gbuffer.bind(device);

    m_screenFillPlane->Draw(device);

    device->LoadRenderState();
}

void SSAOApp::ReleaseInternal()
{
    SAFE_RELEASE(m_obj);

    m_shdGeometryPass.release();
    m_shdSSAOPass.release();

    SAFE_RELEASE(m_screenFillPlane);

    m_gbuffer.clear();
}
