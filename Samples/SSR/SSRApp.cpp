#include "SSRApp.h"
#include "izGraph.h"
#include "izSystem.h"

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
			30.0f, 30.0f);
		VGOTO(result = (m_plane != IZ_NULL), __EXIT__);
	}

    m_shdGeometryPass.init(
        device,
        "shader/geometry_vs.glsl",
        "shader/geometry_ps.glsl");
#if 0
    m_shdSSRPass.init(
        device,
        "shader/ssr_vs.glsl",
        "shader/ssr_ps.glsl");
#endif

    m_gbuffer.init(allocator, device);

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

// 更新.
void SSRApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
}

// 描画.
void SSRApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
	renderToGBufferPass(device);
	renderToScreenPass(device);
    //renderSSRPass(device);

    device->SetTexture(0, IZ_NULL);
    m_gbuffer.drawBuffers(device);
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
			izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
			izanagi::CIntRect(0, 0, width, height));
	}
	device->End2D();
}

void SSRApp::renderSSRPass(izanagi::graph::CGraphicsDevice* device)
{
}

void SSRApp::ReleaseInternal()
{
    SAFE_RELEASE(m_sphere);
	SAFE_RELEASE(m_cube);
	SAFE_RELEASE(m_plane);

    m_shdGeometryPass.release();
    m_shdSSRPass.release();

    m_gbuffer.clear();
}
