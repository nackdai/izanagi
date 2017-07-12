#include "IBLApp.h"

IBLApp::IBLApp()
{
    m_Mesh = nullptr;

    m_Axis = IZ_NULL;
    m_Grid = IZ_NULL;
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

    // シェーダ
    m_shd.init(
        device,
        "shader/basic_vs.glsl",
        "shader/basic_fs.glsl");

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

    SAFE_RELEASE(m_Axis);
    SAFE_RELEASE(m_Grid);

    m_shd.release();
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

    auto* shd = m_shd.m_program;

    device->SetShaderProgram(shd);

    {
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
        // パラメータ設定
        auto hL2W = shd->GetHandleByName("mtxL2W");
        shd->SetMatrix(device, hL2W, mtxL2W);

        auto hW2C = shd->GetHandleByName("mtxW2C");
        shd->SetMatrix(device, hW2C, mtxW2C);

        auto hW2V = shd->GetHandleByName("mtxW2V");
        shd->SetMatrix(device, hW2V, mtxW2V);

        m_Mesh->Draw(device);
    }
}
