#include "IBLApp.h"

enum TexType {
    bg,
    diffuse,
    filtered,
    lut,
};

IBLApp::IBLApp()
{
}

IBLApp::~IBLApp()
{
}

// 初期化.
IZ_BOOL IBLApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    m_envbox = izanagi::sample::CSampleEnvBox::CreateSampleEnvBox(
        allocator,
        device);
    VGOTO(result = (m_envbox != IZ_NULL), __EXIT__);

    // Axis
    m_Axis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
                allocator,
                device);
    VGOTO(result = (m_Axis != IZ_NULL), __EXIT__);

    // Grid
    m_Grid = izanagi::CDebugMeshGrid::CreateDebugMeshGrid(
                allocator,
                device,
                IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                10, 10,
                5.0f);
    VGOTO(result = (m_Grid != IZ_NULL), __EXIT__);

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

    // Sphere
    {
        m_Mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            5.0f,
            10, 10);
        VGOTO(result = (m_Mesh != IZ_NULL), __EXIT__);
    }

    // テクスチャ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/texture.img"), __EXIT__);

        m_Img = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // シェーダ
    m_shdBasic.init(
        device,
        "shader/basic_vs.glsl",
        "shader/basic_fs.glsl");
    m_shdIdealSpec.init(
        device,
        "shader/ideal_spec_vs.glsl",
        "shader/ideal_spec_fs.glsl");
    m_shdPBR.init(
        device,
        "shader/pbr_vs.glsl",
        "shader/pbr_fs.glsl");
    m_shdEnvBox.init(
        device,
        "shader/equirect_vs.glsl",
        "shader/equirect_fs.glsl");

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
void IBLApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Mesh);

    SAFE_RELEASE(m_envbox);

    SAFE_RELEASE(m_Axis);
    SAFE_RELEASE(m_Grid);

    SAFE_RELEASE(m_Img);

    m_shdBasic.release();
    m_shdIdealSpec.release();
    m_shdPBR.release();
    m_shdEnvBox.release();
}

// 更新.
void IBLApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
}

// 描画.
void IBLApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    static const IZ_BOOL isDrawTangentSpaceAxis = IZ_FALSE;

    izanagi::sample::CSampleCamera& camera = GetCamera();

    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    const auto& mtxW2C = camera.GetParam().mtxW2C;
    const auto& mtxW2V = camera.GetParam().mtxW2V;

#if 1
    {
        auto* shd = m_shdBasic.m_program;

        device->SetShaderProgram(shd);

        // パラメータ設定
        auto hL2W = shd->GetHandleByName("mtxL2W");
        shd->SetMatrix(device, hL2W, mtxL2W);

        auto hW2C = shd->GetHandleByName("mtxW2C");
        shd->SetMatrix(device, hW2C, mtxW2C);

        auto hW2V = shd->GetHandleByName("mtxW2V");
        shd->SetMatrix(device, hW2V, mtxW2V);

        m_Grid->Draw(device);
        m_Axis->Draw(device);
    }

    {
        auto* shd = m_shdIdealSpec.m_program;

        device->SetShaderProgram(shd);

        device->SetTexture(0, m_Img->GetTexture(TexType::diffuse));
        device->SetTexture(1, m_Img->GetTexture(TexType::filtered));
        device->SetTexture(2, m_Img->GetTexture(TexType::lut));

        // パラメータ設定
        auto hL2W = shd->GetHandleByName("mtxL2W");
        shd->SetMatrix(device, hL2W, mtxL2W);

        auto hW2C = shd->GetHandleByName("mtxW2C");
        shd->SetMatrix(device, hW2C, mtxW2C);

        auto hEye = shd->GetHandleByName("eye");
        shd->SetVector(device, hEye, camera.GetParam().pos);

        auto hMaxLod = shd->GetHandleByName("MAX_REFLECTION_LOD");
        auto miplevel = m_Img->GetTexture(TexType::filtered)->GetMipMapNum() - 1;
        shd->SetFloat(device, hMaxLod, (float)miplevel);

        auto hRoughness = shd->GetHandleByName("roughness");
        shd->SetFloat(device, hRoughness, 0.5f);

        auto hMetalic = shd->GetHandleByName("metalic");
        shd->SetFloat(device, hMetalic, 0.5f);

        auto hF0 = shd->GetHandleByName("F0");
        shd->SetVector(device, hF0, izanagi::math::CVector4(0.5, 0.5, 0.5, 0.5));

        m_Mesh->Draw(device);
    }
#endif
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

        device->SetTexture(0, m_Img->GetTexture(TexType::bg));

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
}
