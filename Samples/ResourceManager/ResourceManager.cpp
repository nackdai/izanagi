#include "ResourceManager.h"

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

CResourceManagerApp::CResourceManagerApp()
{
    m_RscMgr = IZ_NULL;
}

CResourceManagerApp::~CResourceManagerApp()
{
}

// 初期化.
IZ_BOOL CResourceManagerApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    m_RscMgr = izanagi::resource::CResourceManager::CreateResourceManager(allocator);
    IZ_ASSERT(m_RscMgr != IZ_NULL);

    izanagi::CMesh* msh = IZ_NULL;
    izanagi::CSkeleton* skl = IZ_NULL;

    // Texture
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/ModelImage.img"));

        izanagi::CImage* img = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);


        VGOTO(result = (img != IZ_NULL), __EXIT__);

        m_RscMgr->Add(
            img,
            IZ_CC3('i', 'm', 'g'),
            izanagi::CKey("ModelImage.img"));

        // ResourceManagerに登録したのでもういらない
        SAFE_RELEASE(img);
    }

    // Mesh
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/Seymour.msh"));

        msh = izanagi::CMesh::CreateMesh(
            allocator,
            device,
            &in);
        
        VGOTO(result = (msh != IZ_NULL), __EXIT__);
    }

    // Skelton
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/Seymour.skl"));

        skl = izanagi::CSkeleton::CreateSkeleton(
            allocator,
            &in);

        VGOTO(result = (skl != IZ_NULL), __EXIT__);
    }

    // Shader
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/SkinShader.shd"));

        izanagi::shader::CShaderBasic* shd = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);

        VGOTO(result = (shd != IZ_NULL), __EXIT__);

        // ジョイントマトリクスを描画時に設定するためのハンドラ
        s_MdlRenderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<CSampleMdlRenderHandler>(allocator);
        s_MdlRenderHandler->SetShader(shd);
        VGOTO(result = (s_MdlRenderHandler != IZ_NULL), __EXIT__);

        m_RscMgr->Add(
            shd,
            IZ_CC3('s', 'h', 'd'),
            izanagi::CKey("SkinShader.shd"));

        // ResourceManagerに登録したのでもういらない
        SAFE_RELEASE(shd);
    }

    // Model
    {
        izanagi::CModel* mdl = izanagi::CModel::CreateModel(
            allocator,
            msh,
            skl,
            s_MdlRenderHandler);

        VGOTO(result = (mdl != IZ_NULL), __EXIT__);

        // Modelに登録したのでもういらない
        SAFE_RELEASE(msh);
        SAFE_RELEASE(skl);

        m_RscMgr->Add(
            mdl,
            IZ_CC3('m', 'd', 'l'),
            izanagi::CKey("Seymour.mdl"));

        // ResourceManagerに登録したのでもういらない
        SAFE_RELEASE(mdl);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector(0.0f, 5.0f, 30.0f, 1.0f),
        izanagi::math::CVector(0.0f, 5.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void CResourceManagerApp::ReleaseInternal()
{
    SAFE_RELEASE(s_MdlRenderHandler);

    m_RscMgr->RemoveAll();
    SAFE_RELEASE(m_RscMgr);
}

// 更新.
void CResourceManagerApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
    
    izanagi::CObject* obj = m_RscMgr->Find(
        IZ_CC3('m', 'd', 'l'),
        izanagi::CKey("Seymour.mdl"));
    if (obj != IZ_NULL) {
        izanagi::CModel* mdl = (izanagi::CModel*)obj;
        mdl->Update();
    }
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
void CResourceManagerApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CModel* mdl = IZ_NULL;
    {
        izanagi::CObject* obj = m_RscMgr->Find(
            IZ_CC3('m', 'd', 'l'),
            izanagi::CKey("Seymour.mdl"));
        if (obj != IZ_NULL) {
            mdl = (izanagi::CModel*)obj;
        }
    }

    izanagi::CImage* img = IZ_NULL;
    {
        izanagi::CObject* obj = m_RscMgr->Find(
            IZ_CC3('i', 'm', 'g'),
            izanagi::CKey("ModelImage.img"));
        if (obj != IZ_NULL) {
            img = (izanagi::CImage*)obj;
        }
    }
    
    izanagi::shader::CShaderBasic* shd = IZ_NULL;
    {
        izanagi::CObject* obj = m_RscMgr->Find(
            IZ_CC3('s', 'h', 'd'),
            izanagi::CKey("SkinShader.shd"));
        if (obj != IZ_NULL) {
            shd = (izanagi::shader::CShaderBasic*)obj;
        }
    }

    izanagi::sample::CSampleCamera& camera = GetCamera();

    IZ_UINT passCnt = shd->Begin(0, IZ_FALSE);
    {
        IZ_ASSERT(passCnt >= 1);
        if (shd->BeginPass(0)) {
            // シェーダ定数セット
            {
                _SetShaderParam(
                    shd,
                    "g_mW2C",
                    (void*)&camera.GetParam().mtxW2C,
                    sizeof(camera.GetParam().mtxW2C));
            }

            // テクスチャセット
            device->SetTexture(0, img->GetTexture(0));

            shd->CommitChanges();

            // モデル描画
            mdl->Render();

            shd->EndPass();
        }
    }
    shd->End();
}
