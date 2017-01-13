#include "SD_UnityChanApp.h"

SDUnityChanApp::SDUnityChanApp()
{
}

SDUnityChanApp::~SDUnityChanApp()
{
}

// 初期化.
IZ_BOOL SDUnityChanApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    auto result = initModel(allocator, device);

    // Animation.
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/SD_unitychan_motion.anm"));

        m_Anm = izanagi::CAnimation::CreateAnimation(
            allocator,
            &in);

        result = (m_Anm != IZ_NULL);
        IZ_ASSERT(result);
    }

    // Timeline
    {
        m_Timeline.Init(
            m_Anm->GetAnimationTime(),
            0.0f);
        m_Timeline.EnableLoop(IZ_TRUE);
        m_Timeline.AutoReverse(IZ_FALSE);
        m_Timeline.Start();
        m_Timeline.Reset();
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 5.0f, 100.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 5.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    // L2W
    izanagi::math::SMatrix44::SetUnit(m_L2W);

    return IZ_TRUE;
}

IZ_BOOL SDUnityChanApp::initModel(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL result = IZ_FALSE;

    static const char* imgPath = "data/SD_UnityChanImg.img";
    static const char* mshPath = "data/SD_unitychan.msh";
    static const char* sklPath = "data/SD_unitychan.skl";
    static const char* shdPath = "data/SkinShader.shd";

    // Texture
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(imgPath));

        m_Img = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);

        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // Mesh
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(mshPath));

        m_Msh = izanagi::CMesh::CreateMesh(
            allocator,
            device,
            &in);

        VGOTO(result = (m_Msh != IZ_NULL), __EXIT__);
    }

    // Skelton
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(sklPath));

        m_Skl = izanagi::CSkeleton::CreateSkeleton(
            allocator,
            &in);

        VGOTO(result = (m_Skl != IZ_NULL), __EXIT__);
    }

    // Shader
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(shdPath));

        m_Shd = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
            allocator,
            device,
            &in);

        VGOTO(result = (m_Shd != IZ_NULL), __EXIT__);

        // ジョイントマトリクスを描画時に設定するためのハンドラ
        m_MdlRenderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<izanagi::sample::CSampleMdlRenderHandler>(allocator);
        m_MdlRenderHandler->SetShader(m_Shd);
        VGOTO(result = (m_MdlRenderHandler != IZ_NULL), __EXIT__);
    }

    // Model
    {
        m_Mdl = izanagi::CModel::CreateModel(
            allocator,
            m_Msh,
            m_Skl,
            m_MdlRenderHandler);

        VGOTO(result = (m_Mdl != IZ_NULL), __EXIT__);
    }

    // レンダーグラフ
    m_RenderGraph = izanagi::CRenderGraph::CreateRenderGraph(allocator, 5);
    VGOTO(result = (m_RenderGraph != IZ_NULL), __EXIT__);

    // レンダラ
    m_Renderer = izanagi::CSceneRenderer::CreateSceneRenderer(allocator);
    VGOTO(result = (m_Renderer != IZ_NULL), __EXIT__);

    CreateMaterial(allocator);

__EXIT__:

    return result;
}

IZ_BOOL SDUnityChanApp::CreateMaterial(izanagi::IMemoryAllocator* allocator)
{
    static const char* mtrls[] = {
        "data/SD_unitychan_0.mtrl",
        "data/SD_unitychan_1.mtrl",
        "data/SD_unitychan_2.mtrl",
    };

    static const IZ_UINT IMG_IDX = 0;

    izanagi::CFileInputStream in;

    IZ_ASSERT(COUNTOF(mtrls) <= COUNTOF(m_Mtrl));

    for (IZ_UINT i = 0; i < COUNTOF(mtrls); i++) {
        VRETURN(in.Open(mtrls[i]));

        m_Mtrl[i] = izanagi::CMaterial::CreateMaterial(allocator, &in);
        VRETURN(m_Mtrl[i] != IZ_NULL);

        in.Finalize();
    }

    static const char* mtrlTexName[] = {
        "utc_all2.tga",
    };

    // 基本的には事前にマテリアル名とシェーダ名を一致させておくべきだが
    // シェーダ名を強制的に変更することもできる
    // シェーダ名を見てマテリアルをバインディングする
    m_Shd->SetName("Phong");

    for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
        // マテリアルに対応するテクスチャとシェーダを設定
        m_Mtrl[i]->SetTexture(mtrlTexName[0], m_Img->GetTexture(0));
        m_Mtrl[i]->SetShader(m_Shd);

        // メッシュにマテリアルを設定
        m_Mdl->GetMesh()->SetMaterial(0, m_Mtrl[i]);
    }

    return IZ_TRUE;
}

// 解放.
void SDUnityChanApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Mdl);
    SAFE_RELEASE(m_Msh);
    SAFE_RELEASE(m_Skl);
    SAFE_RELEASE(m_Shd);

    for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
        SAFE_RELEASE(m_Mtrl[i]);
    }

    SAFE_RELEASE(m_RenderGraph);
    SAFE_RELEASE(m_Renderer);

    SAFE_RELEASE(m_MdlRenderHandler);

    SAFE_RELEASE(m_Anm);
}

// 更新.
void SDUnityChanApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    if (m_enableStepAnm) {
        if (m_step) {
            m_Timeline.Advance(16.67f / 1000.0f);
        }
        m_step = IZ_FALSE;
    }
    else {
        // 時間更新
        IZ_FLOAT fElapsed = GetTimer(0).GetTime();
        fElapsed /= 1000.0f;
        m_Timeline.Advance(fElapsed);
    }

    //m_Timeline.Advance(0.5f);
    IZ_FLOAT t = m_Timeline.GetTime();

    // アニメーション適用
    m_Mdl->ApplyAnimation(t, m_Anm);

    m_Mdl->Update();

    // レンダーグラフに登録
    m_RenderGraph->BeginRegister();
    {
        // 位置は原点なので
        m_RenderGraph->Register(
            GetCamera(),
            izanagi::math::CVector4(0, 0, 0, 1),
            m_Mdl);
    }
    m_RenderGraph->EndRegister();
}

namespace {
    inline void _SetShaderParam(
        izanagi::shader::CShaderBasic* shader,
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
void SDUnityChanApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    // シェーダパラメータセット
    {
        const izanagi::math::SMatrix44& mtxW2C = camera.GetParam().mtxW2C;
        _SetShaderParam(m_Shd, "g_mW2C", &mtxW2C, sizeof(mtxW2C));
    }

    // 描画
    m_RenderGraph->Render(
        device,
        m_Renderer,
        m_MdlRenderHandler);


    if (device->Begin2D()) {
        izanagi::CDebugFont* debugFont = GetDebugFont();
        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        IZ_FLOAT t = m_Timeline.GetTime();

        debugFont->DBPrint(
            device,
            "%.3f \n",
            t);

        debugFont->End();

        device->End2D();
    }
}

IZ_BOOL SDUnityChanApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_SPACE) {
        m_enableStepAnm = !m_enableStepAnm;
    }

    if (m_enableStepAnm) {
        if (key == izanagi::sys::E_KEYBOARD_BUTTON_RETURN) {
            m_step = IZ_TRUE;
        }
    }

    return IZ_TRUE;
}
