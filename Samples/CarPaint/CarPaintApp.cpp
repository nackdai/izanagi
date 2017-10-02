#include "CarPaintApp.h"
#include <imgui.h>

enum TexType {
    bg,
	filtered,
	flakes,
};

CarPaintApp::CarPaintApp()
{
}

CarPaintApp::~CarPaintApp()
{
}

// 初期化.
IZ_BOOL CarPaintApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    m_envbox = izanagi::sample::CSampleEnvBox::CreateSampleEnvBox(
        allocator,
        device);
    VGOTO(result = (m_envbox != IZ_NULL), __EXIT__);

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
		| izanagi::E_DEBUG_MESH_VTX_FORM_UV;

    // Sphere
    {
        m_Mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            5.0f,
            100, 100);
        VGOTO(result = (m_Mesh != IZ_NULL), __EXIT__);
    }

    // テクスチャ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/texture.img"), __EXIT__);

		m_tex = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);
        VGOTO(result = (m_tex != IZ_NULL), __EXIT__);
    }

    // imgui
    {
        m_imgui = izanagi::debugutil::ImGuiProc::init(allocator, device);
        IZ_ASSERT(m_imgui);

        izanagi::sys::CSysWindow::registerExtendMsgHandler(m_imgui);
    }

	static const char* shaders[CarPaintShadeType::Num] = {
		"shader/carpaint_multitone_fs.glsl",
		"shader/carpaint_env_fs.glsl",
		"shader/carpaint_multitone_env_fs.glsl",
		"shader/carpaint_flakes_fs.glsl",
		"shader/carpaint_fs.glsl",
	};

	//for (int i = 0; i < CarPaintShadeType::Num; i++) {
	for (int i = 0; i < 4; i++) {
		m_shdCarPaint[i].init(
			device,
			"shader/vs.glsl",
			shaders[i]);
	}
    m_shdEnvBox.init(
        device,
        "shader/equirect_vs.glsl",
        "shader/equirect_fs.glsl");

	m_paintColor[0].Set(1.0f, 0.0f, 0.0f);
	m_paintColor[1].Set(0.0f, 1.0f, 1.0f);
	m_paintColor[2].Set(1.0f, 1.0f, 0.0f);
	m_flakeColor.Set(1.0f, 1.0f, 1.0f);

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 10.0f, 30.0f, 1.0f),
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

    return IZ_TRUE;
}

// 解放.
void CarPaintApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Mesh);

    SAFE_RELEASE(m_envbox);

    SAFE_RELEASE(m_tex);

    SAFE_RELEASE(m_imgui);

	for (int i = 0; i < CarPaintShadeType::Num; i++) {
		m_shdCarPaint[i].release();
	}
    m_shdEnvBox.release();
}

// 更新.
void CarPaintApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    m_imgui->beginFrame();
    {
		static const char* items[] = { "MultiToneOnly", "Environment", "MultiToneWithEnv", "Flake" };
		int item_current = m_shaderType;
		ImGui::Combo("mode", &item_current, items, COUNTOF(items));
		m_shaderType = (CarPaintShadeType)item_current;

		ImGui::ColorEdit3("Color0", m_paintColor[0].v);
		ImGui::ColorEdit3("Color1", m_paintColor[1].v);
		ImGui::ColorEdit3("Color2", m_paintColor[2].v);

		ImGui::SliderFloat("normalScale", &m_normalScale, 0.0f, 1.0f);
		ImGui::SliderFloat("glossLevel", &m_glossLevel, 0.0f, 1.0f);
		ImGui::SliderFloat("brightnessFactor", &m_brightnessFactor, 0.0f, 1.0f);

		ImGui::SliderFloat("normalPerturbation", &m_normalPerturbation, -1.0f, 1.0f);
		ImGui::SliderFloat("microflakePerturbationA", &m_microflakePerturbationA, -1.0f, 1.0f);
		ImGui::SliderFloat("microflakePerturbation", &m_microflakePerturbation, 0.0f, 1.0f);
		ImGui::SliderFloat("flakeScale", &m_flakeScale, 0.0f, 50.0f);
		
		ImGui::ColorEdit3("FlakeColor", m_flakeColor.v);
    }
}

// 描画.
void CarPaintApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    static const IZ_BOOL isDrawTangentSpaceAxis = IZ_FALSE;

    izanagi::sample::CSampleCamera& camera = GetCamera();

    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    const auto& mtxW2C = camera.GetParam().mtxW2C;
    const auto& mtxW2V = camera.GetParam().mtxW2V;

	// Car Paint.
    {
		//device->SetTexture(0, m_tex->GetTexture(TexType::filtered));
		//device->SetTexture(1, m_tex->GetTexture(TexType::flakes));
		device->SetTexture(0, m_tex->GetTexture(TexType::flakes));

        auto* shd = m_shdCarPaint[m_shaderType].m_program;

        device->SetShaderProgram(shd);

		// パラメータ設定
        auto hL2W = shd->GetHandleByName("mtxL2W");
        shd->SetMatrix(device, hL2W, mtxL2W);

        auto hW2C = shd->GetHandleByName("mtxW2C");
        shd->SetMatrix(device, hW2C, mtxW2C);

        auto hEye = shd->GetHandleByName("eye");
        shd->SetVector(device, hEye, camera.GetParam().pos);

        auto hNormalScale = shd->GetHandleByName("normalScale");
        shd->SetFloat(device, hNormalScale, m_normalScale);

		auto hGlossLevel = shd->GetHandleByName("glossLevel");
		shd->SetFloat(device, hGlossLevel, m_glossLevel);

		auto hBrightnessFactor = shd->GetHandleByName("brightnessFactor");
		shd->SetFloat(device, hBrightnessFactor, m_brightnessFactor);

		auto hMaxLod = shd->GetHandleByName("MAX_REFLECTION_LOD");
		auto miplevel = m_tex->GetTexture(TexType::filtered)->GetMipMapNum() - 1;
		shd->SetFloat(device, hMaxLod, (float)miplevel);

		auto hNormalPerturbation = shd->GetHandleByName("normalPerturbation");
		shd->SetFloat(device, hNormalPerturbation, m_normalPerturbation);

		auto hMicroflakePerturbationA = shd->GetHandleByName("microflakePerturbationA");
		shd->SetFloat(device, hMicroflakePerturbationA, m_microflakePerturbationA);

		auto hMicroflakePerturbation = shd->GetHandleByName("microflakePerturbation");
		shd->SetFloat(device, hMicroflakePerturbation, m_microflakePerturbation);

		auto hFlakeScale = shd->GetHandleByName("flakeScale");
		shd->SetFloat(device, hFlakeScale, m_flakeScale);

        auto hPaintColor = shd->GetHandleByName("paintColor0");
        shd->SetVector(device, hPaintColor, m_paintColor[0]);

		hPaintColor = shd->GetHandleByName("paintColor1");
		shd->SetVector(device, hPaintColor, m_paintColor[1]);

		hPaintColor = shd->GetHandleByName("paintColor2");
		shd->SetVector(device, hPaintColor, m_paintColor[2]);

		auto hFlakeColor = shd->GetHandleByName("flakeColor");
		shd->SetVector(device, hFlakeColor, m_flakeColor);

        m_Mesh->Draw(device);
    }

	// BG
    {
        auto shd = m_shdEnvBox.m_program;

        device->SetShaderProgram(shd);

        izanagi::math::SMatrix44::SetScale(mtxL2W, 100.0f, 100.0f, 100.0f);

        // カメラの位置にあわせて移動する
        izanagi::math::SMatrix44::Trans(
            mtxL2W,
            mtxL2W,
            GetCamera().GetParam().pos);

        izanagi::sample::CSampleCamera& camera = GetCamera();

        device->SetTexture(0, m_tex->GetTexture(TexType::bg));

        device->SetShaderProgram(shd);

        auto hL2W = shd->GetHandleByName("mtxL2W");
        shd->SetMatrix(device, hL2W, mtxL2W);

        auto mtxW2C = camera.GetParam().mtxW2C;

        auto hW2C = shd->GetHandleByName("mtxW2C");
        shd->SetMatrix(device, hW2C, mtxW2C);

        auto hEye = shd->GetHandleByName("eye");
        shd->SetVector(device, hEye, camera.GetParam().pos);

        m_envbox->Render(device);
    }

    ImGui::Render();
}
