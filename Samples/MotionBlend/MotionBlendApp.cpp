#include "MotionBlendApp.h"
#include "CustomInterp.h"

static izanagi::sample::CSampleMdlRenderHandler* s_MdlRenderHandler = IZ_NULL;

#define MSH_FILE_NAME   "data/miku.msh"
#define SKL_FILE_NAME   "data/miku.skl"

static const char* ANM_FILE_NAME[] =
{
    "data/miku_walk.anm",
    "data/miku_run_with_gun.anm",
};

#define IMG_IDX        (0)
#define CAMERA_Z    (-30.0f)

CMotionBlendApp::CMotionBlendApp()
{
    m_Img = IZ_NULL;
    m_Mdl = IZ_NULL;
    m_Msh = IZ_NULL;
    m_Skl = IZ_NULL;
    m_Shd = IZ_NULL;

    m_Anm[0] = IZ_NULL;
    m_Anm[1] = IZ_NULL;

    FILL_ZERO(m_Mtrl, sizeof(m_Mtrl));

    m_RenderGraph = IZ_NULL;
    m_Renderer = IZ_NULL;

    m_AnmBlender = IZ_NULL;
}

CMotionBlendApp::~CMotionBlendApp()
{
}

// 初期化.
IZ_BOOL CMotionBlendApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // Texture
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/MikuImage.img"));

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);

        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // Animation
    for (IZ_UINT i = 0; i < COUNTOF(ANM_FILE_NAME); i++)
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(ANM_FILE_NAME[i]));

        m_Anm[i] = izanagi::CAnimation::CreateAnimation(
                    allocator,
                    &in);

        VGOTO(result = (m_Anm[i] != IZ_NULL), __EXIT__);
    }

    // 独自の補間計算を設定
    izanagi::CAnimation::SetUserFuncInterpScalar(CCustomInterp::InterpScalar);
    izanagi::CAnimation::SetUserFuncInterpVector(CCustomInterp::InterpVector);

    // アニメーションブレンド
    {
         m_AnmBlender = izanagi::IAnimationBlender::CreateAnmBlender<izanagi::CAnmLinearBlender>(
                        allocator);
        IZ_ASSERT(m_AnmBlender != IZ_NULL);

        m_AnmBlender->SetAnimation(
            0.5f,   // ブレンド率
            m_Anm[0],
            m_Anm[1]);
    }

    // Timeline
    {
        m_Timeline.Init(
            m_Anm[0]->GetAnimationTime(),
            0.0f);
        m_Timeline.EnableLoop(IZ_TRUE);
        m_Timeline.AutoReverse(IZ_FALSE);
        m_Timeline.Start();
        m_Timeline.Reset();
    }

    // Mesh
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(MSH_FILE_NAME));

        m_Msh = izanagi::CMesh::CreateMesh(
                    allocator,
                    device,
                    &in);
        
        VGOTO(result = (m_Msh != IZ_NULL), __EXIT__);
    }

    // Skelton
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(SKL_FILE_NAME));

        m_Skl = izanagi::CSkeleton::CreateSkeleton(
                    allocator,
                    &in);

        VGOTO(result = (m_Skl != IZ_NULL), __EXIT__);
    }

    // Shader
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/SkinShader.shd"));

        m_Shd = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);

        VGOTO(result = (m_Shd != IZ_NULL), __EXIT__);

        // ジョイントマトリクスを描画時に設定するためのハンドラ
        s_MdlRenderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<izanagi::sample::CSampleMdlRenderHandler>(allocator);
        s_MdlRenderHandler->SetShader(m_Shd);
        VGOTO(result = (s_MdlRenderHandler != IZ_NULL), __EXIT__);
    }

    // Model
    {
        m_Mdl = izanagi::CModel::CreateModel(
                    allocator,
                    m_Msh,
                    m_Skl,
                    s_MdlRenderHandler);

        VGOTO(result = (m_Mdl != IZ_NULL), __EXIT__);
    }

    // Material
    {
        izanagi::CFileInputStream in;
        IZ_CHAR tmp[32];

        for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++)
        {
            IZ_SPRINTF(tmp, sizeof(tmp), "data/miku_%d.mtrl\0", i);

            VRETURN(in.Open(tmp));

            m_Mtrl[i] = izanagi::CMaterial::CreateMaterial(allocator, &in);
            VGOTO((result = m_Mtrl[i] != IZ_NULL), __EXIT__);

            in.Finalize();
        }

        // シェーダ名を強制的に設定
        m_Shd->SetName("DefaultShader");

        for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++)
        {
            IZ_UINT num = m_Mtrl[i]->GetTexNum();

            IZ_PCSTR name = m_Mtrl[i]->GetTexInfoByIdx(num > 1 ? 1 : 0)->name.GetString();

            IZ_BOOL isToon = (::strncmp(name, "toon", 4) == 0);

            m_Mtrl[i]->SetTexture(name, m_Img->GetTexture(isToon ? 0 : 1));
            m_Mtrl[i]->SetShader(m_Shd);

            m_Mdl->SetMaterial(0, m_Mtrl[i]);
        }
    }

    // レンダーグラフ
    m_RenderGraph = izanagi::CRenderGraph::CreateRenderGraph(allocator, 5);
    VGOTO(result = (m_RenderGraph != IZ_NULL), __EXIT__);

    // レンダラ
    m_Renderer = izanagi::CSceneRenderer::CreateSceneRenderer(allocator);
    VGOTO(result = (m_Renderer != IZ_NULL), __EXIT__);

    // カメラ
    camera.Init(
        izanagi::math::CVector(0.0f, 5.0f, CAMERA_Z, 1.0f),
        izanagi::math::CVector(0.0f, 5.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
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
void CMotionBlendApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Mdl);
    SAFE_RELEASE(m_Msh);
    SAFE_RELEASE(m_Skl);
    SAFE_RELEASE(m_Shd);

    for (IZ_UINT i = 0; i < COUNTOF(m_Anm); i++)
    {
        SAFE_RELEASE(m_Anm[i]);
    }

    for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++)
    {
        SAFE_RELEASE(m_Mtrl[i]);
    }

    SAFE_RELEASE(m_RenderGraph);
    SAFE_RELEASE(m_Renderer);

    SAFE_RELEASE(m_AnmBlender);

    SAFE_RELEASE(s_MdlRenderHandler);
}

// 更新.
void CMotionBlendApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    // 時間更新
    IZ_FLOAT fElapsed = GetTimer(0).GetTime();
    fElapsed /= 1000.0f;

    IZ_FLOAT time = m_Timeline.GetTime();

    // アニメーション適用
    m_Mdl->ApplyAnimation(time, m_AnmBlender);

    m_Mdl->Update();

    m_Timeline.Advance(fElapsed * 30.0f);

    m_RenderGraph->BeginRegister();
    {
        // 位置は原点なので
        m_RenderGraph->Register(
            GetCamera(),
            izanagi::math::CVector(0.0f, 0.0f, 0.0f),
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
void CMotionBlendApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

#if 0
    IZ_UINT passCnt = m_Shd->Begin(0, IZ_FALSE);
    {
        IZ_ASSERT(passCnt >= 1);
        if (m_Shd->BeginPass(0)) {
            // シェーダ定数セット
            {
                _SetShaderParam(
                    m_Shd,
                    "g_mW2C",
                    (void*)&camera.GetParam().mtxW2C,
                    sizeof(camera.GetParam().mtxW2C));
            }

            // テクスチャセット
            //device->SetTexture(0, m_Img->GetTexture(IMG_IDX));

            m_Shd->CommitChanges();

            // モデル描画
            m_Mdl->Render();

            m_Shd->EndPass();
        }
    }
    m_Shd->End();
#else
    _SetShaderParam(
        m_Shd,
        "g_mW2C",
        (void*)&camera.GetParam().mtxW2C,
        sizeof(camera.GetParam().mtxW2C));

    m_RenderGraph->Render(
        device,
        m_Renderer,
        s_MdlRenderHandler);
#endif
}
