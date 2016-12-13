#include "TemporalReprojectionApp.h"
#include "izGraph.h"
#include "izSystem.h"

// NOTE
// http://john-chapman-graphics.blogspot.jp/2013/01/ssao-tutorial.html
// http://blazelight.net/en/articles/ssao-tutorial-english.php
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-simple-and-practical-approach-to-ssao-r2753

TemporalReprojectionApp::TemporalReprojectionApp()
{
}

TemporalReprojectionApp::~TemporalReprojectionApp()
{
}

// 開始
IZ_BOOL TemporalReprojectionApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    // TODO
    izanagi::math::CMathRand::Init(0);

    generateKernel();

    m_shdGeometryPass.init(
        device,
        "shader/vs_geometry.glsl",
        "shader/ps_geometry.glsl");
    m_shdSSAOPass.init(
        device,
        "shader/vs_fillscreen.glsl",
        "shader/ps_ssao.glsl");
    m_shdBlurPass.init(
        device,
        "shader/vs_fillscreen.glsl",
        "shader/ps_blur.glsl");
    m_shdFinalPass.init(
        device,
        "shader/vs_fillscreen.glsl",
        "shader/ps_final.glsl");

    m_obj = izanagi::sample::ObjModel::create(
        allocator,
        device,
        "../../Media/teapot.obj");

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

void TemporalReprojectionApp::generateKernel()
{
    for (IZ_UINT i = 0; i < KernelSize; i++) {
#if 0
        // Generate random postion in hemisphere.
        m_kernels[i] = izanagi::math::CVector4(
            izanagi::math::CMathRand::GetRandBetween<IZ_FLOAT>(-1.0f, 1.0f),
            izanagi::math::CMathRand::GetRandBetween<IZ_FLOAT>(-1.0f, 1.0f),
            izanagi::math::CMathRand::GetRandBetween<IZ_FLOAT>(0.0f, 1.0f),
            0.0f);

        m_kernels[i].Normalize();

        // Falloff the distance from the origin.
        IZ_FLOAT scale = (IZ_FLOAT)i / (IZ_FLOAT)KernelSize;
        scale = izanagi::math::CMath::Lerp(0.1f, 1.0f, scale * scale);
        m_kernels[i] *= scale;
#else
        // Importance sampling.

        IZ_FLOAT r1 = izanagi::math::CMathRand::GetRandFloat();
        IZ_FLOAT(r1 >= 0.0f);

        IZ_FLOAT r2 = izanagi::math::CMathRand::GetRandFloat();
        IZ_FLOAT(r2 >= 0.0f);

        IZ_FLOAT cos_theta = sqrt(1 - r2);
        IZ_FLOAT sin_theta = sqrt(r2);
        IZ_FLOAT phi = IZ_MATH_PI2 * r1;

        IZ_FLOAT x = cos(phi) * sin_theta;
        IZ_FLOAT y = sin(phi) * sin_theta;
        IZ_FLOAT z = cos_theta;

        m_kernels[i] = izanagi::math::CVector4(x, y, z, 0.0f);
#endif
    }
}

// 更新.
void TemporalReprojectionApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
}

// 描画.
void TemporalReprojectionApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    renderGeometryPass(device);
    renderSSAOPass(device);
    renderBlurPass(device);
    renderFinalPass(device);

    device->SetTexture(0, IZ_NULL);
    //m_gbuffer.drawBuffers(device);
}

void TemporalReprojectionApp::renderGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    const auto& camera = GetCamera();

    // TODO
    izanagi::math::CMatrix44 mtxL2W;
    mtxL2W.SetScale(100, 100, 100);

    izanagi::math::CVector4 color(1.0f, 0.0f, 0.0f, 1.0f);

    const auto& mtxW2C = camera.GetParam().mtxW2C;
    const auto& mtxW2V = camera.GetParam().mtxW2V;

    auto* shd = m_shdGeometryPass.m_program;

    device->SetShaderProgram(shd);

    auto hL2W = shd->GetHandleByName("mtxL2W");
    shd->SetMatrix(device, hL2W, mtxL2W);

    auto hW2C = shd->GetHandleByName("mtxW2C");
    shd->SetMatrix(device, hW2C, mtxW2C);

    auto hW2V = shd->GetHandleByName("mtxW2V");
    shd->SetMatrix(device, hW2V, mtxW2V);

    auto hClr = shd->GetHandleByName("color");
    shd->SetVector(device, hClr, color);

    auto hFar = shd->GetHandleByName("depthFar");
    shd->SetFloat(device, hFar, camera.GetParam().cameraFar);

    m_gbuffer.beginGeometryPass(device);

    m_obj->render(device);

    m_gbuffer.endGeometryPass(device);
}

void TemporalReprojectionApp::renderSSAOPass(izanagi::graph::CGraphicsDevice* device)
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
    const auto& mtxV2C = camera.GetParam().mtxV2C;
    
    izanagi::math::CMatrix44 mtxC2V, mtxV2W;
    izanagi::math::SMatrix44::Inverse(mtxC2V, camera.GetParam().mtxV2C);
    izanagi::math::SMatrix44::Inverse(mtxV2W, camera.GetParam().mtxW2V);

    auto* shd = m_shdSSAOPass.m_program;

    device->SetShaderProgram(shd);

    auto hC2V = shd->GetHandleByName("mtxC2V");
    shd->SetMatrix(device, hC2V, mtxC2V);

    auto hV2W = shd->GetHandleByName("mtxV2W");
    shd->SetMatrix(device, hV2W, mtxV2W);

    auto hW2C = shd->GetHandleByName("mtxW2C");
    shd->SetMatrix(device, hW2C, mtxW2C);

    auto hV2C = shd->GetHandleByName("mtxV2C");
    shd->SetMatrix(device, hV2C, mtxV2C);

    auto hInvScr = shd->GetHandleByName("invScreen");
    izanagi::math::CVector4 invScr(1.0f / width, 1.0f / height, 0.0f, 1.0f);
    shd->SetVector(device, hInvScr, invScr);

    auto hFar = shd->GetHandleByName("depthFar");
    shd->SetFloat(device, hFar, camera.GetParam().cameraFar);

    auto hKernel = shd->GetHandleByName("kernels");
    shd->SetVectorArray(device, hKernel, m_kernels, KernelSize);

    // TODO
    static const IZ_FLOAT radius = 5.0f;
    auto hRadius = shd->GetHandleByName("radius");
    shd->SetFloat(device, hRadius, radius);

    m_gbuffer.bindForSSAOPass(device);

    m_gbuffer.beginSSAOPass(device);

    m_screenFillPlane->Draw(device);

    m_gbuffer.endSSAOPass(device);

    device->LoadRenderState();
}

void TemporalReprojectionApp::renderBlurPass(izanagi::graph::CGraphicsDevice* device)
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

    auto* shd = m_shdBlurPass.m_program;

    device->SetShaderProgram(shd);

    auto hInvScr = shd->GetHandleByName("invScreen");
    izanagi::math::CVector4 invScr(1.0f / width, 1.0f / height, 0.0f, 1.0f);
    shd->SetVector(device, hInvScr, invScr);

    m_gbuffer.bindForBlurPass(device);

    m_gbuffer.beginBlurPass(device);

    m_screenFillPlane->Draw(device);

    m_gbuffer.endBlurPass(device);

    device->LoadRenderState();
}

void TemporalReprojectionApp::renderFinalPass(izanagi::graph::CGraphicsDevice* device)
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

    auto* shd = m_shdFinalPass.m_program;

    device->SetShaderProgram(shd);

    auto hInvScr = shd->GetHandleByName("invScreen");
    izanagi::math::CVector4 invScr(1.0f / width, 1.0f / height, 0.0f, 1.0f);
    shd->SetVector(device, hInvScr, invScr);

    m_gbuffer.bindForFinalPass(device);

    m_screenFillPlane->Draw(device);

    device->LoadRenderState();
}

void TemporalReprojectionApp::ReleaseInternal()
{
    SAFE_RELEASE(m_obj);

    m_shdGeometryPass.release();
    m_shdSSAOPass.release();
    m_shdBlurPass.release();
    m_shdFinalPass.release();

    SAFE_RELEASE(m_screenFillPlane);

    m_gbuffer.clear();
}
