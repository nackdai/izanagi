#include "SpringArmedCameraApp.h"

SpringArmedCameraApp::SpringArmedCameraApp()
{
}

SpringArmedCameraApp::~SpringArmedCameraApp()
{
}

// 初期化.
IZ_BOOL SpringArmedCameraApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_FALSE;

    // レンダーグラフ
    m_RenderGraph = izanagi::CRenderGraph::CreateRenderGraph(allocator, 5);
    VGOTO(result = (m_RenderGraph != IZ_NULL), __EXIT__);

    // レンダラ
    m_Renderer = izanagi::CSceneRenderer::CreateSceneRenderer(allocator);
    VGOTO(result = (m_Renderer != IZ_NULL), __EXIT__);

    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/BasicShader.shd"), __EXIT__);

        m_shd = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
            allocator,
            device,
            &in);
        VGOTO(result = (m_shd != IZ_NULL), __EXIT__);
    }

    m_grid = izanagi::CDebugMeshGrid::CreateDebugMeshGrid(
        allocator,
        device,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
        100, 100,
        10.0f);
    VGOTO(result = (m_grid != IZ_NULL), __EXIT__);

    // カメラ
    m_camera.Init(
        izanagi::math::CVector4(0.0f, 160.0f, -160.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 80.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    m_camera.Update(0.0f);

    result = m_player.init(
        allocator,
        device,
        m_camera);

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void SpringArmedCameraApp::ReleaseInternal()
{
    m_player.release();

    SAFE_RELEASE(m_RenderGraph);
    SAFE_RELEASE(m_Renderer);

    SAFE_RELEASE(m_shd);
    SAFE_RELEASE(m_grid);
} 

// 更新.
void SpringArmedCameraApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    IZ_FLOAT elapsed = GetTimer(0).GetTime();
    //elapsed /= 1000.0f;
    elapsed = 16.67f / 1000.0f;

    m_player.update(device, m_camera, elapsed);

    m_camera.Update(elapsed);

    // レンダーグラフに登録
    m_RenderGraph->BeginRegister();
    {
        m_player.prepareToRender(
            m_camera,
            *m_RenderGraph);
    }
    m_RenderGraph->EndRegister();
}

namespace {
    inline void _SetShaderParam(
        izanagi::shader::IShader* shader,
        const char* name,
        const void* value,
        IZ_UINT bytes)
    {
        izanagi::shader::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
        IZ_ASSERT(handle != 0);

        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
}

// 描画.
void SpringArmedCameraApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    m_player.render(
        device, 
        m_camera,
        m_RenderGraph,
        m_Renderer);

    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    // テクスチャなし
    m_shd->Begin(device, 1, IZ_FALSE);
    {
        if (m_shd->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_shd,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            _SetShaderParam(
                m_shd,
                "g_mW2C",
                (void*)&m_camera.GetParam().mtxW2C,
                sizeof(m_camera.GetParam().mtxW2C));

            // シェーダ設定
            m_shd->CommitChanges(device);

            m_grid->Draw(device);

            m_shd->EndPass();
        }
    }
    m_shd->End(device);

    if (device->Begin2D()) {
        izanagi::CDebugFont* debugFont = GetDebugFont();

        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        izanagi::math::CVector4 pos(m_camera.GetParam().pos);
        izanagi::math::CVector4 at(m_camera.GetParam().ref);
        izanagi::math::CVector4 ply(m_player.position());
        const auto& v = m_camera.getVelocity();

        debugFont->DBPrint(
            device,
            "pos [%.2f][%.2f][%.2f]\n",
            pos.x, pos.y, pos.z);
        debugFont->DBPrint(
            device,
            "at  [%.2f][%.2f][%.2f]\n",
            at.x, at.y, at.z);
        debugFont->DBPrint(
            device,
            "ply [%.2f][%.2f][%.2f]\n",
            ply.x, ply.y, ply.z);
        debugFont->DBPrint(
            device,
            "v   [%.2f][%.2f][%.2f]\n",
            v.x, v.y, v.z);

        debugFont->End();

        device->End2D();
    }
}

IZ_BOOL SpringArmedCameraApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    m_player.OnKeyDown(key);
    return IZ_TRUE;
}

void SpringArmedCameraApp::OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    m_player.OnKeyUp(key);
}