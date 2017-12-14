#include "UpscaleGeometryRenderingApp.h"
#include "izGraph.h"
#include "izSystem.h"

// NOTE
// http://d.hatena.ne.jp/hanecci/20131103/p1
// https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-136
// http://www.slideshare.net/HEXADRIVE/final-fantasy-hd-hd

UpscaleGeometryRenderingApp::UpscaleGeometryRenderingApp()
{
}

UpscaleGeometryRenderingApp::~UpscaleGeometryRenderingApp()
{
}

// 開始
IZ_BOOL UpscaleGeometryRenderingApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    m_scene.init(allocator, device, camera);

    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    m_shdGeometryPass.init(
        device,
        "shader/vs_geometry.glsl",
        "shader/ps_geometry.glsl");
    m_shdColorPass.init(
        device,
        "shader/vs_default.glsl",
        "shader/ps_default.glsl");
    m_shdFinalPass.init(
        device,
        "shader/vs_fillscreen.glsl",
        "shader/ps_final.glsl");
    
    m_gbuffer.init(allocator, device);

    m_screenFillPlane = izanagi::CDebugMeshScreenFillPlane::create(
        allocator,
        device,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff));
    IZ_ASSERT(m_screenFillPlane != IZ_NULL);

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, 30.0f, 1.0f),
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

IZ_BOOL UpscaleGeometryRenderingApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_RETURN) {
		int type = (int)m_type;
		type += 1;

		type = type >= Type::Num ? 0 : type;

		m_type = (Type)type;
    }
    return IZ_TRUE;
}

// 更新.
void UpscaleGeometryRenderingApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
}

// 描画.
void UpscaleGeometryRenderingApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (m_type == Type::Upscale) {
        renderColorPass(device);
        renderGeometryPass(device);
        renderFinalPass(device);

        device->SetTexture(0, IZ_NULL);
        m_gbuffer.drawBuffers(device);
    }
	else if (m_type == Type::Reference) {
        const auto& camera = GetCamera();

        const auto& mtxW2C = camera.GetParam().mtxW2C;

        auto* shd = m_shdColorPass.m_program;

        device->SetShaderProgram(shd);

        auto hW2C = shd->GetHandleByName("mtxW2C");
        shd->SetMatrix(device, hW2C, mtxW2C);

        m_scene.render(device, shd);
    }
	else {
		renderColorPass(device);

		if (device->Begin2D()) {
			auto clr = m_gbuffer.getBuffer(GBuffer::Type::Color);
			device->SetTexture(0, clr);

			auto width = device->GetBackBufferWidth();
			auto height = device->GetBackBufferHeight();

			device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

			device->Draw2DSprite(
				izanagi::CFloatRect(0.0f, 1.0f, 1.0f, 0.0f),
				izanagi::CIntRect(0, 0, width, height));

			device->End2D();
		}
	}
}

void UpscaleGeometryRenderingApp::renderColorPass(izanagi::graph::CGraphicsDevice* device)
{
    const auto& camera = GetCamera();

    const auto& mtxW2C = camera.GetParam().mtxW2C;

    auto* shd = m_shdColorPass.m_program;

    device->SetShaderProgram(shd);

    auto hW2C = shd->GetHandleByName("mtxW2C");
    shd->SetMatrix(device, hW2C, mtxW2C);

    auto bgColor = GetBgColor();

    m_gbuffer.beginColorPass(device, bgColor);

    m_scene.render(device, shd);

    m_gbuffer.endColorPass(device);
}

void UpscaleGeometryRenderingApp::renderGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
	device->SaveRenderState();

	device->SetRenderState(
		izanagi::graph::E_GRAPH_RS_ALPHABLENDENABLE,
		IZ_FALSE);

    const auto& camera = GetCamera();

    const auto& mtxW2C = camera.GetParam().mtxW2C;

    auto* shd = m_shdGeometryPass.m_program;

    device->SetShaderProgram(shd);

    auto hW2C = shd->GetHandleByName("mtxW2C");
    shd->SetMatrix(device, hW2C, mtxW2C);

	{
		m_gbuffer.beginGeometryLowResPass(device);

		m_scene.render(device, shd);

		m_gbuffer.endGeometryLowResPass(device);
	}

	{
		m_gbuffer.beginGeometryHiResPass(device);

		m_scene.render(device, shd);

		m_gbuffer.endGeometryHiResPass(device);
	}

	device->LoadRenderState();
}

void UpscaleGeometryRenderingApp::renderFinalPass(izanagi::graph::CGraphicsDevice* device)
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

void UpscaleGeometryRenderingApp::ReleaseInternal()
{
    m_scene.release();

    m_shdGeometryPass.release();
    m_shdColorPass.release();
    m_shdFinalPass.release();

    SAFE_RELEASE(m_screenFillPlane);

    m_gbuffer.release();
}
