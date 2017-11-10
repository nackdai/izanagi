#include "SSRApp.h"
#include "izGraph.h"
#include "izSystem.h"
#include <imgui.h>

static float maxDistance = 1000.0f;
static float zThickness = 1.0f;
static int maxSteps = 25;

SSRApp::SSRApp()
{
}

SSRApp::~SSRApp()
{
}

// 開始
IZ_BOOL SSRApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    // TODO
    izanagi::math::CMathRand::Init(0);

	{
		IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
			| izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
			| izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

		m_sphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
			allocator,
			device,
			flag,
			IZ_COLOR_RGBA(0xff, 0, 0, 0xff),
			5.0f,
			10, 10);
		VGOTO(result = (m_sphere != IZ_NULL), __EXIT__);

		m_cube = izanagi::CDebugMeshBox::CreateDebugMeshBox(
			allocator,
			device,
			flag,
			IZ_COLOR_RGBA(0, 0xff, 0, 0xff),
			10.0f, 10.0f, 10.0f);
		VGOTO(result = (m_cube != IZ_NULL), __EXIT__);

		m_plane = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
			allocator,
			device,
			flag,
			IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
			10, 10,
			300.0f, 300.0f);
		VGOTO(result = (m_plane != IZ_NULL), __EXIT__);
	}

	// imgui
	{
		m_imgui = izanagi::debugutil::ImGuiProc::init(allocator, device);
		IZ_ASSERT(m_imgui);

		izanagi::sys::CSysWindow::registerExtendMsgHandler(m_imgui);
	}

    m_shdGeometryPass.init(
        device,
        "shader/geometry_vs.glsl",
        "shader/geometry_ps.glsl");
#if 1
    m_shdSSRPass.init(
        device,
        "shader/ssr_vs.glsl",
        "shader/ssr_ps.glsl");
#endif

    m_gbuffer.init(allocator, device);

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, 100.0f, 1.0f),
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
void SSRApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

	m_imgui->beginFrame();
	{
		ImGui::SliderFloat("zThickness", &zThickness, 0.0f, 10.0f);
		ImGui::SliderInt("maxSteps", &maxSteps, 1, 100);
	}
}

// 描画.
void SSRApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
	renderToGBufferPass(device);
	renderToScreenPass(device);
    renderSSRPass(device);

    device->SetTexture(0, IZ_NULL);
    m_gbuffer.drawBuffers(device);

	// imgui.
	ImGui::Render();
}

void SSRApp::renderToGBufferPass(izanagi::graph::CGraphicsDevice* device)
{
    const auto& camera = GetCamera();

	const auto& mtxW2C = camera.GetParam().mtxW2C;
	const auto& mtxW2V = camera.GetParam().mtxW2V;

    auto* shd = m_shdGeometryPass.m_program;

    device->SetShaderProgram(shd);

	auto hW2C = shd->GetHandleByName("mtxW2C");
	shd->SetMatrix(device, hW2C, mtxW2C);
	
    m_gbuffer.beginGeometryPass(device);

	auto hL2W = shd->GetHandleByName("mtxL2W");

	// Sphere.
	{
		izanagi::math::CMatrix44 mtxL2W;
		mtxL2W.SetTrans(izanagi::math::CVector4(0.0f, 10.0f, 0.0f));

		shd->SetMatrix(device, hL2W, mtxL2W);

		m_sphere->Draw(device);
	}

	// Cube.
	{
		izanagi::math::CMatrix44 mtxL2W;
		mtxL2W.SetTrans(izanagi::math::CVector4(20.0f, 10.0f, 0.0f));

		shd->SetMatrix(device, hL2W, mtxL2W);

		m_cube->Draw(device);
	}

    m_gbuffer.endGeometryPass(device);
}

void SSRApp::renderToScreenPass(izanagi::graph::CGraphicsDevice* device)
{
	auto width = device->GetBackBufferWidth();
	auto height = device->GetBackBufferHeight();

	device->Begin2D();
	{
		auto rt = m_gbuffer.getBuffer(0);

		device->SetTexture(0, rt);

		device->Draw2DSprite(
			izanagi::CFloatRect(0.0f, 1.0f, 1.0f, 0.0f),
			izanagi::CIntRect(0, 0, width, height));
	}
	device->End2D();
}

void SSRApp::renderSSRPass(izanagi::graph::CGraphicsDevice* device)
{
	const auto& camera = GetCamera();

	const auto& mtxW2C = camera.GetParam().mtxW2C;
	const auto& mtxW2V = camera.GetParam().mtxW2V;
	const auto& mtxV2C = camera.GetParam().mtxV2C;

	izanagi::math::CMatrix44 mtxC2V(mtxV2C);
	mtxC2V.Inverse();

	izanagi::math::CMatrix44 mtxV2W(mtxW2V);
	mtxV2W.Inverse();

	const auto& camPos = camera.GetParam().pos;
	float nearPlaneZ = camera.GetParam().cameraNear;

	auto* shd = m_shdSSRPass.m_program;

	device->SetShaderProgram(shd);

	auto hL2W = shd->GetHandleByName("mtxL2W");
	izanagi::math::CMatrix44 mtxL2W;
	shd->SetMatrix(device, hL2W, mtxL2W);

	auto hW2C = shd->GetHandleByName("mtxW2C");
	shd->SetMatrix(device, hW2C, mtxW2C);

	auto hW2V = shd->GetHandleByName("mtxW2V");
	shd->SetMatrix(device, hW2V, mtxW2V);

	auto hV2C = shd->GetHandleByName("mtxV2C");
	shd->SetMatrix(device, hV2C, mtxV2C);

	auto hC2V = shd->GetHandleByName("mtxC2V");
	shd->SetMatrix(device, hC2V, mtxC2V);

	auto hV2W = shd->GetHandleByName("mtxV2W");
	shd->SetMatrix(device, hV2W, mtxV2W);

	auto hCamPos = shd->GetHandleByName("camPos");
	shd->SetVector(device, hCamPos, camPos);

	auto hNearPlaneZ = shd->GetHandleByName("nearPlaneZ");
	shd->SetFloat(device, hNearPlaneZ, nearPlaneZ);

	auto hMaxDistnace = shd->GetHandleByName("maxDistance");
	shd->SetFloat(device, hMaxDistnace, maxDistance);

	auto hZThickness = shd->GetHandleByName("zThickness");
	shd->SetFloat(device, hZThickness, zThickness);
	
	auto hMaxSteps = shd->GetHandleByName("maxSteps");
	shd->SetInt(device, hMaxSteps, maxSteps);

	m_gbuffer.bindForSSRPass(device);

	m_plane->Draw(device);
}

void SSRApp::ReleaseInternal()
{
    SAFE_RELEASE(m_sphere);
	SAFE_RELEASE(m_cube);
	SAFE_RELEASE(m_plane);

    m_shdGeometryPass.release();
    m_shdSSRPass.release();

    m_gbuffer.clear();

	SAFE_RELEASE(m_imgui);
}
