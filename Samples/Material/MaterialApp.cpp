#include "MaterialApp.h"

/////////////////////////////////////////////////

// ジョイントマトリクスを描画時に設定するためのハンドラ
class CSampleMdlRenderHandler : public izanagi::IMshRenderHandler {
    // NOTE
    // IMshRenderHandlerが持つインスタンス作成用メソッドに対して
    // コンストラクタを見せるために必要
    friend class izanagi::IMshRenderHandler;

private:
    CSampleMdlRenderHandler()
    {
        m_pShader = IZ_NULL;
    }

    ~CSampleMdlRenderHandler()
    {
        SAFE_RELEASE(m_pShader);
    }

public:
    virtual void BeginRenderMesh();
    virtual void EndRenderMesh();

    virtual void SetJointMatrix(
        IZ_UINT nIdx,
        const izanagi::math::SMatrix& mtx);

    virtual void CommitChanges();

public:
    void SetShader(izanagi::shader::IShader* pShader)
    {
        SAFE_REPLACE(m_pShader, pShader);
    }

    izanagi::shader::IShader* GetShader() { return m_pShader; }

private:
    izanagi::shader::IShader* m_pShader;

    IZ_UINT m_nCnt;
    izanagi::math::SMatrix m_Mtx[48];

    izanagi::shader::IZ_SHADER_HANDLE m_Handle;
};

void CSampleMdlRenderHandler::BeginRenderMesh()
{
    m_nCnt = 0;

    izanagi::math::SMatrix::SetUnit(m_Mtx[0]);
    izanagi::math::SMatrix::SetUnit(m_Mtx[1]);
    izanagi::math::SMatrix::SetUnit(m_Mtx[2]);
    izanagi::math::SMatrix::SetUnit(m_Mtx[3]);

    m_Handle = 0;
}

void CSampleMdlRenderHandler::EndRenderMesh()
{
}

void CSampleMdlRenderHandler::SetJointMatrix(
    IZ_UINT nIdx,
    const izanagi::math::SMatrix& mtx)
{
    izanagi::math::SMatrix::Copy(m_Mtx[m_nCnt], mtx);
    m_nCnt++;
}

void CSampleMdlRenderHandler::CommitChanges()
{
    if (m_Handle == 0) {
        m_Handle = m_pShader->GetParameterByName("vJointMatrix");
        IZ_ASSERT(m_Handle > 0);
    }

    m_pShader->SetParamValue(
        m_Handle,
        m_Mtx,
        sizeof(izanagi::math::SMatrix) * m_nCnt);

    m_pShader->CommitChanges();
}

static CSampleMdlRenderHandler* s_MdlRenderHandler = IZ_NULL;

/////////////////////////////////////////////////

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
        s_MdlRenderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<CSampleMdlRenderHandler>(allocator);
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
        izanagi::math::SVector::Normalize(m_ParallelLight.vDir, m_ParallelLight.vDir);

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
        izanagi::math::CVector(0.0f, 5.0f, CAMERA_Z, 1.0f),
        izanagi::math::CVector(0.0f, 5.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
    camera.Update();

    // L2W
    izanagi::math::SMatrix::SetUnit(m_L2W);

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
            izanagi::math::CVector(),
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
        const izanagi::math::SMatrix& mtxW2C = camera.GetParam().mtxW2C;
        _SetShaderParam(m_Shd, "g_mW2C", &mtxW2C, sizeof(mtxW2C));

        {
            // ライトの方向をローカル座標に変換する

            // ライトの方向はワールド座標なので World -> Localマトリクスを計算する
            izanagi::math::SMatrix mtxW2L;
            izanagi::math::SMatrix::Inverse(mtxW2L, m_L2W);

            // World -> Local
            izanagi::math::SVector parallelLightLocalDir;
            izanagi::math::SMatrix::ApplyXYZ(
                parallelLightLocalDir,
                m_ParallelLight.vDir,
                m_L2W);

            _SetShaderParam(
                m_Shd,
                "g_vLitParallelDir",
                (void*)&parallelLightLocalDir,
                sizeof(parallelLightLocalDir));

            // L2V = L2W * W2V の逆行列を計算する
            izanagi::math::SMatrix mtxV2L;
            izanagi::math::SMatrix::Mul(mtxV2L, m_L2W, camera.GetParam().mtxW2V);
            izanagi::math::SMatrix::Inverse(mtxV2L, mtxV2L);

            // ビュー座標系における視点は常に原点
            izanagi::math::CVector eyePos(0.0f, 0.0f, 0.0f, 1.0f);

            // 視点のローカル座標を計算する
            izanagi::math::SMatrix::Apply(eyePos, eyePos, mtxV2L);

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
