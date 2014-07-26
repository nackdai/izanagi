#include "MaterialApp.h"

static izanagi::sample::CSampleMdlRenderHandler* s_MdlRenderHandler = IZ_NULL;

#define MSH_FILE_NAME    "data/Seymour.msh"
#define SKL_FILE_NAME    "data/Seymour.skl"
#define IMG_IDX        (0)
#define CAMERA_Z    (30.0f)

CMaterialApp::CMaterialApp()
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

CMaterialApp::~CMaterialApp()
{
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
IZ_BOOL CMaterialApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // Texture
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/ModelImage.img"));

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);

        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
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

        _SetShaderParam(m_Shd, "g_vMtrlDiffuse", &mtrl.vDiffuse, sizeof(mtrl.vDiffuse));

        _SetShaderParam(m_Shd, "g_vLitParallelColor", &m_ParallelLight.color, sizeof(m_ParallelLight.color));
        _SetShaderParam(m_Shd, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));
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

        for (IZ_UINT i = 0; i < COUNTOF(m_Mtrl); i++) {
            IZ_SPRINTF(tmp, sizeof(tmp), "data/Seymour_%d.mtrl\0", i);

            VRETURN(in.Open(tmp));

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

    // レンダーグラフ
    m_RenderGraph = izanagi::CRenderGraph::CreateRenderGraph(allocator, 5);
    VGOTO(result = (m_RenderGraph != IZ_NULL), __EXIT__);

    // レンダラ
    m_Renderer = izanagi::CSceneRenderer::CreateSceneRenderer(allocator);
    VGOTO(result = (m_Renderer != IZ_NULL), __EXIT__);

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 5.0f, CAMERA_Z, 1.0f),
        izanagi::math::CVector4(0.0f, 5.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    // L2W
    izanagi::math::SMatrix44::SetUnit(m_L2W);

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void CMaterialApp::ReleaseInternal()
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

    SAFE_RELEASE(s_MdlRenderHandler);
}

// 更新.
void CMaterialApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
    m_Mdl->Update();

    // レンダーグラフに登録
    m_RenderGraph->BeginRegister();
    {
        // 位置は原点なので
        m_RenderGraph->Register(
            GetCamera(),
            izanagi::math::CVector4(),
            m_Mdl);
    }
    m_RenderGraph->EndRegister();
}

// 描画.
void CMaterialApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    // シェーダパラメータセット
    {
        const izanagi::math::SMatrix44& mtxW2C = camera.GetParam().mtxW2C;
        _SetShaderParam(m_Shd, "g_mW2C", &mtxW2C, sizeof(mtxW2C));

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
    }

    // 描画
    m_RenderGraph->Render(
        device,
        m_Renderer,
        s_MdlRenderHandler);
}
