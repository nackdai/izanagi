#include "StateBase.h"
#include "izSampleKit.h"
#include "StateManager.h"

CStateBase::CStateBase(izanagi::sample::CSampleApp* app)
: m_App(app)
{
    m_Img = IZ_NULL;
    m_Mdl = IZ_NULL;
    m_Msh = IZ_NULL;
    m_Skl = IZ_NULL;
    m_Shd = IZ_NULL;

    memset(m_Mtrl, 0, sizeof(m_Mtrl));

    m_RenderGraph = IZ_NULL;
    m_Renderer = IZ_NULL;
}

// 初期化.
IZ_BOOL CStateBase::Init()
{
    // Nothing is done...
    return IZ_TRUE;
}

// 更新
IZ_BOOL CStateBase::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device)
{
    m_App->GetCamera().Update();
    m_Mdl->Update();

    // レンダーグラフに登録
    m_RenderGraph->BeginRegister();
    {
        // 位置は原点なので
        m_RenderGraph->Register(
            m_App->GetCamera(),
            izanagi::math::CVector4(),
            m_Mdl);
    }
    m_RenderGraph->EndRegister();

    return IZ_TRUE;
}

// 終了.
IZ_BOOL CStateBase::Destroy()
{
    return Leave();
}

IZ_BOOL CStateBase::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    State cur = CStateManager::GetInstance().GetCurrentState();

    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        CStateManager::GetInstance().ChangeState(cur + 1);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        CStateManager::GetInstance().ChangeState(cur - 1);
    }

    return IZ_TRUE;
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

// 初期化.
IZ_BOOL CStateBase::InitObject(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT cameraZ,
    IZ_PCSTR imgPath,
    IZ_PCSTR mshPath,
    IZ_PCSTR sklPath,
    IZ_PCSTR shdPath)
{
    IZ_BOOL result = IZ_TRUE;

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

    // ライトパラメータ
    {
        // Ambient Light Color
        izanagi::SAmbientLightParam ambient;
        ambient.color.Set(0.0f, 0.0f, 0.0f);

        // Parallel Light Color
        m_ParallelLight.color.Set(1.0f, 1.0f, 1.0f);

        // Parallel Light Direction
        m_ParallelLight.vDir.Set(-1.0f, -1.0f, -1.0f);
        izanagi::math::SVector4::Normalize(m_ParallelLight.vDir, m_ParallelLight.vDir);

        // マテリアル
        izanagi::SMaterialParam mtrl;
        {
            mtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
            mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
            mtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
        }
#if 0
        _SetShaderParam(m_Shd, "g_vMtrlDiffuse", &mtrl.vDiffuse, sizeof(mtrl.vDiffuse));

        _SetShaderParam(m_Shd, "g_vLitParallelColor", &m_ParallelLight.color, sizeof(m_ParallelLight.color));
        _SetShaderParam(m_Shd, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));
#endif
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

    // Material
#if 0
    {
        izanagi::CFileInputStream in;
        IZ_CHAR tmp[32];

        IZ_ASSERT(mtrlPath.size() <= COUNTOF(m_Mtrl));

        for (IZ_UINT i = 0; i < mtrlPath.size(); i++) {
            VRETURN(in.Open(mtrlPath[i]));

            m_Mtrl[i] = izanagi::CMaterial::CreateMaterial(allocator, &in);
            VGOTO((result = m_Mtrl[i] != IZ_NULL), __EXIT__);

            in.Finalize();
        }

        // 基本的には事前にマテリアル名とシェーダ名を一致させておくべきだが
        // シェーダ名を強制的に変更することもできる
        // シェーダ名を見てマテリアルをバインディングする
        m_Shd->SetName("DefaultShader");

        for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
            // マテリアルに対応するテクスチャとシェーダを設定
            m_Mtrl[i]->SetTexture("boy_10.tga", m_Img->GetTexture(IMG_IDX));
            m_Mtrl[i]->SetShader(m_Shd);

            // メッシュにマテリアルを設定
            m_Mdl->GetMesh()->SetMaterial(0, m_Mtrl[i]);
        }
    }
#else
    result = CreateMaterial(allocator);
    VGOTO(result, __EXIT__);
#endif

    // レンダーグラフ
    m_RenderGraph = izanagi::CRenderGraph::CreateRenderGraph(allocator, 5);
    VGOTO(result = (m_RenderGraph != IZ_NULL), __EXIT__);

    // レンダラ
    m_Renderer = izanagi::CSceneRenderer::CreateSceneRenderer(allocator);
    VGOTO(result = (m_Renderer != IZ_NULL), __EXIT__);

    // カメラ
    m_App->GetCamera().Init(
        izanagi::math::CVector4(0.0f, 5.0f, cameraZ, 1.0f),
        izanagi::math::CVector4(0.0f, 5.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    m_App->GetCamera().Update();

    // L2W
    izanagi::math::SMatrix44::SetUnit(m_L2W);

__EXIT__:
    if (!result) {
        ReleaseObject();
    }

    return IZ_TRUE;
}

void CStateBase::ReleaseObject()
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
}

void CStateBase::RenderName(
    izanagi::graph::CGraphicsDevice* device,
    const char* name)
{
    izanagi::CDebugFont* debugFont = m_App->GetDebugFont();

    if (device->Begin2D()) {
        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        debugFont->DBPrint(
            device,
            "%s\n",
            name);

        debugFont->End();

        device->End2D();
    }
}

// 描画.
IZ_BOOL CStateBase::Render(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = m_App->GetCamera();

    // シェーダパラメータセット
    {
        const izanagi::math::SMatrix44& mtxW2C = camera.GetParam().mtxW2C;
        _SetShaderParam(m_Shd, "g_mW2C", &mtxW2C, sizeof(mtxW2C));

#if 0
        {
            // ライトの方向をローカル座標に変換する

            // ライトの方向はワールド座標なので World -> Localマトリクスを計算する
            izanagi::math::SMatrix44 mtxW2L;
            izanagi::math::SMatrix44::Inverse(mtxW2L, m_L2W);

            // World -> Local
            izanagi::math::SVector4 parallelLightLocalDir;
            izanagi::math::SMatrix44::ApplyXYZ(
                parallelLightLocalDir,
                m_ParallelLight.vDir,
                m_L2W);

            _SetShaderParam(
                m_Shd,
                "g_vLitParallelDir",
                (void*)&parallelLightLocalDir,
                sizeof(parallelLightLocalDir));

            // L2V = L2W * W2V の逆行列を計算する
            izanagi::math::SMatrix44 mtxV2L;
            izanagi::math::SMatrix44::Mul(mtxV2L, m_L2W, camera.GetParam().mtxW2V);
            izanagi::math::SMatrix44::Inverse(mtxV2L, mtxV2L);

            // ビュー座標系における視点は常に原点
            izanagi::math::CVector4 eyePos(0.0f, 0.0f, 0.0f, 1.0f);

            // 視点のローカル座標を計算する
            izanagi::math::SMatrix44::Apply(eyePos, eyePos, mtxV2L);

            _SetShaderParam(
                m_Shd,
                "g_vEye",
                (void*)&eyePos,
                sizeof(eyePos));
        }
#endif
    }

    // 描画
    m_RenderGraph->Render(
        device,
        m_Renderer,
        m_MdlRenderHandler);

    return IZ_TRUE;
}
