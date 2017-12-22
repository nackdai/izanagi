#include "TAAApp.h"
#include "izGraph.h"
#include "izSystem.h"
#include <imgui.h>

TAAApp::TAAApp()
{
}

TAAApp::~TAAApp()
{
}

IZ_BOOL TAAApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::graph::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	IZ_BOOL result = IZ_TRUE;

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
	m_shdTAAPass.init(
		device,
		"shader/vs_fillscreen.glsl",
		"shader/ps_taa.glsl");
	m_shdFinalPass.init(
		device,
		"shader/vs_fillscreen.glsl",
		"shader/ps_final.glsl");

	m_obj = izanagi::sample::ObjModel::create(
		allocator,
		device,
		"../../Media/teapot.obj");

	for (int i = 0; i < COUNTOF(m_rt); i++) {
		m_rt[i] = device->CreateRenderTarget(
			width, height,
			izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
	}

	m_gbuffer.init(allocator, device);

	m_screenFillPlane = izanagi::CDebugMeshScreenFillPlane::create(
		allocator,
		device,
		IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff));
	IZ_ASSERT(m_screenFillPlane != IZ_NULL);

	// imgui
	{
		m_imgui = izanagi::debugutil::ImGuiProc::init(allocator, device);
		IZ_ASSERT(m_imgui);

		izanagi::sys::CSysWindow::registerExtendMsgHandler(m_imgui);
	}

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
void TAAApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
	GetCamera().Update();

	m_imgui->beginFrame();
	{
		ImGui::SliderFloat("Alpha", &m_param.lerpRatio, 0.0f, 1.0f);
		ImGui::SliderFloat("Color-Box Sigma", &m_param.colorBoxSigma, 0.0f, 1.0f);

		ImGui::Checkbox("Enable TAA", &m_param.enableTAA);
		ImGui::Checkbox("Show TAA Diff", &m_param.showDiff);
	}
}

// 描画.
void TAAApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
	const auto& camera = GetCamera();

	auto width = device->GetBackBufferWidth();
	auto height = device->GetBackBufferHeight();

	static const float offset_tbl[8][2] = {
		// http://en.wikipedia.org/wiki/Halton_sequence
		{ 1.0f / 2.0f, 1.0f / 3.0f },
		{ 1.0f / 4.0f, 2.0f / 3.0f },
		{ 3.0f / 4.0f, 1.0f / 9.0f },
		{ 1.0f / 8.0f, 4.0f / 9.0f },
		{ 5.0f / 8.0f, 7.0f / 9.0f },
		{ 3.0f / 8.0f, 2.0f / 9.0f },
		{ 7.0f / 8.0f, 5.0f / 9.0f },
		{ 1.0f / 16.0f, 8.0f / 9.0f },
	};

	int idx = m_frame % 8;
	izanagi::math::SMatrix44::SetUnit(m_mtxOffset);
	izanagi::math::SMatrix44::GetTrans(
		m_mtxOffset,
		offset_tbl[idx][0] / width,
		offset_tbl[idx][1] / height,
		0.0f);

	m_mtxL2W.SetScale(100, 100, 100);

	if (m_frame == 0) {
		m_mtxPrevL2W = m_mtxL2W;
		m_mtxPrevW2C = camera.GetParam().mtxW2C;
	}

	renderGeometryPass(device);
	renderColorPass(device);
	renderTAAPass(device);
	renderFinalPass(device);

	device->SetTexture(0, IZ_NULL);

	m_gbuffer.drawBuffers(device);

	// imgui.
	ImGui::Render();

	m_curRt = 1 - m_curRt;

	m_mtxPrevL2W = m_mtxL2W;
	m_mtxPrevW2C = camera.GetParam().mtxW2C;

	m_frame++;
}

void TAAApp::renderGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
	const auto& camera = GetCamera();	

	const auto& mtxW2C = camera.GetParam().mtxW2C;

	auto* shd = m_shdGeometryPass.m_program;

	device->SetShaderProgram(shd);

	// Current frame matrix.
	{
		auto hL2W = shd->GetHandleByName("mtxL2W");
		shd->SetMatrix(device, hL2W, m_mtxL2W);

		auto hW2C = shd->GetHandleByName("mtxW2C");
		shd->SetMatrix(device, hW2C, mtxW2C);
	}

	// Previous frame matrix.
	{
		auto hL2W = shd->GetHandleByName("mtxPrevL2W");
		shd->SetMatrix(device, hL2W, m_mtxPrevL2W);

		auto hW2C = shd->GetHandleByName("mtxPrevW2C");
		shd->SetMatrix(device, hW2C, m_mtxPrevW2C);
	}

	auto hOffset = shd->GetHandleByName("mtxOffset");
	shd->SetMatrix(device, hOffset, m_mtxOffset);

	auto width = device->GetBackBufferWidth();
	auto height = device->GetBackBufferHeight();

	auto hInvScr = shd->GetHandleByName("invScreen");
	shd->SetVector(device, hInvScr, izanagi::math::CVector4(1.0f / width, 1.0f / height, 0.0f, 0.0f));

	m_gbuffer.beginGeometryPass(device);

	m_obj->render(device);

	m_gbuffer.endGeometryPass(device);
}

void TAAApp::renderColorPass(izanagi::graph::CGraphicsDevice* device)
{
	const auto& camera = GetCamera();

	izanagi::math::CVector4 color(1.0f, 0.0f, 0.0f, 1.0f);

	const auto& mtxW2C = camera.GetParam().mtxW2C;

	auto* shd = m_shdColorPass.m_program;

	device->SetShaderProgram(shd);

	auto hL2W = shd->GetHandleByName("mtxL2W");
	shd->SetMatrix(device, hL2W, m_mtxL2W);

	auto hW2C = shd->GetHandleByName("mtxW2C");
	shd->SetMatrix(device, hW2C, mtxW2C);

	auto hOffset = shd->GetHandleByName("mtxOffset");
	shd->SetMatrix(device, hOffset, m_mtxOffset);

	auto hClr = shd->GetHandleByName("color");
	shd->SetVector(device, hClr, color);

	m_gbuffer.beginColorPass(device);

	m_obj->render(device);

	m_gbuffer.endColorPass(device);
}

void TAAApp::renderTAAPass(izanagi::graph::CGraphicsDevice* device)
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

	auto* shd = m_shdTAAPass.m_program;

	device->SetShaderProgram(shd);

	auto hInvScr = shd->GetHandleByName("invScreen");
	shd->SetVector(device, hInvScr, izanagi::math::CVector4(1.0f / width, 1.0f / height, 0.0f, 0.0f));

	auto hClrBoxSigma = shd->GetHandleByName("colorBoxSigma");
	shd->SetFloat(device, hClrBoxSigma, m_param.colorBoxSigma);

	auto hLerpRatio = shd->GetHandleByName("lerpRatio");
	shd->SetFloat(device, hLerpRatio, m_param.lerpRatio);

	auto hEnableTAA = shd->GetHandleByName("enableTAA");
	shd->SetBool(device, hEnableTAA, m_param.enableTAA);

	auto hShowDiff = shd->GetHandleByName("showDiff");
	shd->SetBool(device, hShowDiff, m_param.showDiff);

	m_gbuffer.bindForTAAPass(device);

	device->SetTexture(4, m_rt[1 - m_curRt]);

	izanagi::graph::CRenderTarget* rt[] = {
		m_rt[m_curRt],
	};

	device->BeginScene(rt, 1, izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL);

	m_screenFillPlane->Draw(device);

	device->EndScene();

	device->LoadRenderState();
}

void TAAApp::renderFinalPass(izanagi::graph::CGraphicsDevice* device)
{
	device->SaveRenderState();

	device->SetRenderState(
		izanagi::graph::E_GRAPH_RS_ZENABLE,
		IZ_FALSE);
	device->SetRenderState(
		izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
		IZ_FALSE);

	auto* shd = m_shdFinalPass.m_program;

	device->SetShaderProgram(shd);

	auto width = device->GetBackBufferWidth();
	auto height = device->GetBackBufferHeight();

	auto hInvScr = shd->GetHandleByName("invScreen");
	shd->SetVector(device, hInvScr, izanagi::math::CVector4(1.0f / width, 1.0f / height, 0.0f, 0.0f));

	device->SetTexture(0, m_rt[m_curRt]);

	m_screenFillPlane->Draw(device);

	device->LoadRenderState();
}

void TAAApp::ReleaseInternal()
{
	SAFE_RELEASE(m_obj);

	m_shdGeometryPass.release();
	m_shdColorPass.release();
	m_shdTAAPass.release();
	m_shdFinalPass.release();

	SAFE_RELEASE(m_screenFillPlane);

	for (int i = 0; i < COUNTOF(m_rt); i++) {
		SAFE_RELEASE(m_rt[i]);
	}

	SAFE_RELEASE(m_imgui);

	m_gbuffer.clear();
}
