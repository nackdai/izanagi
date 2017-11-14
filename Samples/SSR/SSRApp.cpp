#include "SSRApp.h"
#include "izGraph.h"
#include "izSystem.h"
#include <imgui.h>

static float maxDistance = 1000.0f;
static float zThickness = 1.0f;
static int maxSteps = 1000;
static float stride = 5.0f;

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
        izanagi::math::CVector4(0.0f, 20.0f, -100.0f, 1.0f),
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
		ImGui::SliderFloat("zThickness", &zThickness, 0.0f, 100.0f);
		ImGui::SliderInt("maxSteps", &maxSteps, 1, 100);
		ImGui::SliderFloat("stride", &stride, 1, 10);
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
	auto hIsWriteColor = shd->GetHandleByName("isWriteColor");

	// Sphere.
	{
		izanagi::math::CMatrix44 mtxL2W;
		mtxL2W.SetTrans(izanagi::math::CVector4(0.0f, 10.0f, 10.0f));

		shd->SetMatrix(device, hL2W, mtxL2W);
		shd->SetBool(device, hIsWriteColor, IZ_TRUE);

		m_sphere->Draw(device);
	}

	// Cube.
	{
		izanagi::math::CMatrix44 mtxL2W;
		mtxL2W.SetTrans(izanagi::math::CVector4(20.0f, 10.0f, 10.0f));

		shd->SetMatrix(device, hL2W, mtxL2W);
		shd->SetBool(device, hIsWriteColor, IZ_TRUE);

		m_cube->Draw(device);
	}

	device->SaveRenderState();

	// Plane.
	{
		izanagi::math::CMatrix44 mtxL2W;
		shd->SetMatrix(device, hL2W, mtxL2W);

		// NOTE
		// あとでSSRとして描画するので、カラーは出力しない.
		shd->SetBool(device, hIsWriteColor, IZ_FALSE);

		// NOTE
		// G-Bufferに深度値のみを出力したい.
		// あとでSSRとして描画するので、Z-bufferへの深度値書き込みをしない.
		device->SetRenderState(
			izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
			IZ_FALSE);

		m_plane->Draw(device);
	}

	device->LoadRenderState();

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

using vec3 = izanagi::math::CVector3;
using vec4 = izanagi::math::CVector4;

float squaredLength(vec3 a, vec3 b)
{
	a -= b;
	return a.Dot(a);
}

bool intersectsDepthBuffer(float z, float minZ, float maxZ)
{
	/*
	* Based on how far away from the camera the depth is,
	* adding a bit of extra thickness can help improve some
	* artifacts. Driving this value up too high can cause
	* artifacts of its own.
	*/

	// 指定範囲内（レイの始点と終点）に z があれば、それはレイにヒットしたとみなせる.
	z += zThickness;
	return (maxZ >= z) && (minZ - zThickness <= z);
}

void SSRApp::renderSSRPass(izanagi::graph::CGraphicsDevice* device)
{
	const auto& camera = GetCamera();

	const izanagi::math::CMatrix44 mtxW2C = camera.GetParam().mtxW2C;
	const izanagi::math::CMatrix44 mtxW2V = camera.GetParam().mtxW2V;
	const izanagi::math::CMatrix44 mtxV2C = camera.GetParam().mtxV2C;

	izanagi::math::CMatrix44 mtxC2V(mtxV2C);
	mtxC2V.Inverse();

	izanagi::math::CMatrix44 mtxV2W(mtxW2V);
	mtxV2W.Inverse();

	const auto& camPos = camera.GetParam().pos;
	float nearPlaneZ = camera.GetParam().cameraNear;

#if 0
	auto rt = m_gbuffer.getBuffer(1);

	float* data;
	auto pitch = rt->Lock(0, (void**)&data, IZ_TRUE);
	rt->Unlock(0);

	vec3 normal = vec3(0, 1, 0);
	//float depth = 141.535f;

	izanagi::math::CMatrix44 mtxTex;
	mtxTex._00 = 0.5;
	mtxTex._11 = 0.5;
	mtxTex._30 = 0.5;
	mtxTex._31 = 0.5;

	for (int y = 0; y < 720; y++) {
		for (int x = 0; x < 1280; x++) {
			vec4 D = mtxW2C * vec4(10, 10, 10, 1);
			float d = D.w;

			D /= D.w;

			D.x *= d;
			D.y *= d;
			D.z = 0;
			D.w = 1;

			vec4 vpos = mtxC2V * D;
			vpos.z = d;

			vec4 wpos = mtxV2W * vpos;

			int yy = 719 - y;
			//auto yy = y;

			if (x == 258 && yy == 417) {
				int hoge = 0;
			}

			float depth = data[yy * 1280 + x];

			if (depth == 0) {
				continue;
			}

			// Screen coordinate.
			vec4 pos = vec4(x / 1280.0f, yy / 720.0f, 0, 1);

			// [0, 1] -> [-1, 1]
			pos.x = pos.x * 2.0 - 1.0;
			pos.y = pos.y * 2.0 - 1.0;

			// Screen-space -> Clip-space
			pos.x *= depth;
			pos.y *= depth;

			// Clip-space -> View-space
			pos = mtxC2V * pos;
			pos.z = depth;

#if 0
			// Ray origin is camera origin.
			vec3 rayOrg = camPos.xyz;

			// View-space -> World-space.
			vec3 worldPos = (mtxV2W * vec4(pos.xyz, 1)).xyz;

			// Compute ray direction.
			// From ray origin to world position.
			vec3 rayDir = worldPos - rayOrg;
			rayDir.Normalize();

			// Compute reflection vector.
			vec3 refDir = rayDir - 2.0 * normal.Dot(rayDir) * normal;
			refDir.Normalize();

			// Reflection vector origin is world position.
			vec3 refOrg = worldPos;

			// Transform to view coordinate.
			refOrg = (mtxW2V * vec4(refOrg, 1)).xyz;
			refDir = (mtxW2V * vec4(refDir, 0)).xyz;
			refDir.Normalize();
#else
			vec3 rayDir = pos.xyz;
			rayDir.Normalize();

			vec3 vsNml = (mtxW2V * vec4(normal, 0)).xyz;

			vec3 refDir = rayDir - 2.0 * vsNml.Dot(rayDir) * vsNml;
			refDir.Normalize();

			vec3 refOrg = pos.xyz;
			refOrg += vsNml * 0.2f;

			vec3 worldPos = (mtxV2W * vec4(refOrg, 1)).xyz;
#endif

			vec3 hitPixel = vec3(0, 0, 0);
			vec3 hitPoint = vec3(0, 0, 0);

			auto csOrig = refOrg;
			auto csDir = refDir;

			// Clip to the near plane.
			float rayLength = (csOrig.z + csDir.z * maxDistance) < nearPlaneZ
				? (nearPlaneZ - csOrig.z) / csDir.z
				: maxDistance;

			vec3 csEndPoint = csOrig + csDir * rayLength;

			// Project into homogeneous clip space.
			vec4 H0 = mtxV2C * vec4(csOrig, 1);
			vec4 H1 = mtxV2C * vec4(csEndPoint, 1);

			//H0 = mtxTex * H0;
			//H1 = mtxTex * H1;

			float k0 = 1.0 / H0.w;
			float k1 = 1.0 / H1.w;

			// The interpolated homogeneous version of the camera-space points.
			vec3 Q0 = csOrig * k0;
			vec3 Q1 = csEndPoint * k1;

			// Screen space point.
			vec3 P0 = vec3(H0.x * k0, H0.y * k0, 0);
			vec3 P1 = vec3(H1.x * k1, H1.y * k1, 0);

			// [-1, 1] -> [0, 1]
			P0 = P0 * 0.5 + 0.5;
			P1 = P1 * 0.5 + 0.5;

			P0.x *= 1280;
			P0.y *= 720;

			P1.x *= 1280;
			P1.y *= 720;

			//P1.x = izanagi::math::CMath::Clamp(P1.x, 0.0f, 1280.0f);
			//P1.y = izanagi::math::CMath::Clamp(P1.y, 0.0f, 720.0f);

			// If the line is degenerate, make it cover at least one pixel to avoid handling zero-pixel extent as a special case later.
			// 2点間の距離がある程度離れるようにする.
			P1 += squaredLength(P0, P1) < 0.0001
				? vec3(0.01, 0.01, 0)
				: vec3(0, 0, 0);
			vec3 delta = P1 - P0;

			// Permute so that the primary iteration is in x to collapse all quadrant-specific DDA cases later.
			bool permute = false;
			if (abs(delta.x) < abs(delta.y))
			{
				permute = true;

				std::swap(delta.x, delta.y);
				std::swap(P0.x, P0.y);
				std::swap(P1.x, P1.y);
			}

			float stepDir = 0;
			if (delta.x < 0) {
				stepDir = -1;
			}
			else if (delta.x > 0) {
				stepDir = 1;
			}

			float invdx = stepDir / delta.x;

			// Track the derivatives of Q and k.
			vec3 dQ = (Q1 - Q0) * invdx;
			float dk = (k1 - k0) * invdx;

			// y is slope.
			// slope = (y1 - y0) / (x1 - x0)
			vec3 dP = vec3(stepDir, delta.y * invdx, 0);

			// Adjust end condition for iteration direction
			float end = P1.x * stepDir;

			int stepCount = 0;

			float prevZMaxEstimate = csOrig.z;

			float rayZMin = prevZMaxEstimate;
			float rayZMax = prevZMaxEstimate;

			float sceneZMax = rayZMax + 100.0f;

			dP *= stride;
			dQ *= stride;
			dk *= stride;

			vec4 PQk = vec4(P0.x, P0.y, Q0.z, k0);
			vec4 dPQk = vec4(dP.x, dP.y, dQ.z, dk);
			vec3 Q = Q0;

			for (;
				((PQk.x * stepDir) <= end)	// 終点に到達したか.
				&& (stepCount < maxSteps)	// 最大処理数に到達したか.
				&& !intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax)	// レイが衝突したか.
				//&& ((rayZMax < sceneZMax - zThickness) || (rayZMin > sceneZMax))
				&& (sceneZMax != 0.0);	// 何もないところに到達してないか.
				++stepCount)
			{
				// 前回のZの最大値が次の最小値になる.
				rayZMin = prevZMaxEstimate;

				// 次のZの最大値を計算する.
				// ただし、1/2 pixel分 余裕を持たせる.
				// Qはw成分で除算されていて、そこに1/wで除算するので、元（View座標系）に戻ることになる.
				rayZMax = (PQk.z + dPQk.z * 0.5) / (PQk.w + dPQk.w * 0.5);

				// 次に向けて最大値を保持.
				prevZMaxEstimate = rayZMax;

				if (rayZMin > rayZMax) {
					// 念のため.
					float tmp = rayZMin;
					rayZMin = rayZMax;
					rayZMax = tmp;
				}

				hitPixel = permute ? vec3(PQk.y, PQk.x, 0) : vec3(PQk.x, PQk.y, 0);

				// シーン内の現時点での深度値を取得.
				//sceneZMax = texelFetch(s1, ivec2(hitPixel), 0).r;
				int ix = hitPixel.x;
				//int iy = 720 - hitPixel.y;
				int iy = hitPixel.y;

				if (ix >= 1280 || iy >= 720 || ix < 0 || iy < 0) {
					break;
				}
				sceneZMax = data[iy * 1280 + ix];

				PQk += dPQk;
			}

			// Advance Q based on the number of steps
			Q.x += dQ.x * stepCount;
			Q.y += dQ.y * stepCount;

			// Qはw成分で除算されていて、そこに1 / wで除算するので、元（View座標系）に戻ることになる.
			hitPoint = Q * (1.0f / PQk.w);

			//auto isect = (rayZMax >= sceneZMax - zThickness) && (rayZMin < sceneZMax);
			auto isect = intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax);

			if (isect) {
				IZ_PRINTF("isect [%d][%d]\n", x, y);
			}
		}
	}

	int xxx = 0;
#else
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

	auto hStride = shd->GetHandleByName("stride");
	shd->SetFloat(device, hStride, stride);

	//m_gbuffer.beginDebug(device);

	m_gbuffer.bindForSSRPass(device);

	m_plane->Draw(device);

	//m_gbuffer.endDebug(device);
#endif
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
