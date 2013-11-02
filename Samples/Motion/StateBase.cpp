#include "StateBase.h"
#include "izSampleKit.h"
#include "StateManager.h"
#include "MotionApp.h"

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

/////////////////////////////////////////////////

CStateBase::CStateBase(izanagi::sample::CSampleApp* app)
: m_App(app)
{
}

// 初期化.
IZ_BOOL CStateBase::Init()
{
    // Nothing is done...
    return IZ_TRUE;
}

// 更新
IZ_BOOL CStateBase::Update()
{
    // 時間更新
    IZ_FLOAT fElapsed = GetElapesedTime();

    m_Timeline.Advance(fElapsed);
    //m_Timeline.Advance(0.5f);
    IZ_FLOAT time = m_Timeline.GetTime();

    // アニメーション適用
    m_Mdl->ApplyAnimation(time, m_Anm);

    m_Mdl->Update();

    return IZ_TRUE;
}

IZ_FLOAT CStateBase::GetElapesedTime()
{
    IZ_FLOAT fElapsed = m_App->GetTimer(0).GetTime();
    fElapsed /= 1000.0f;

    return fElapsed;
}

// 終了.
IZ_BOOL CStateBase::Destroy()
{
    return Leave();
}

IZ_BOOL CStateBase::OnKeyDown(IZ_UINT nChar)
{
    State cur = CStateManager::GetInstance().GetCurrentState();

    if (nChar == VK_UP) {
        CStateManager::GetInstance().ChangeState(cur + 1);
    }
    else if (nChar == VK_DOWN) {
        CStateManager::GetInstance().ChangeState(cur - 1);
    }

    return IZ_TRUE;
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

void CStateBase::SetShaderParam(
    izanagi::shader::CShaderBasic* shader,
    const char* name,
    const void* value,
    IZ_UINT bytes)
{
    izanagi::shader::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
    if (handle > 0) {
        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
    else
    {
        IZ_PRINTF("Can not find shader parameter [%s]\n", name);
    }
}

IZ_BOOL CStateBase::Render(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = m_App->GetCamera();

    IZ_UINT passCnt = m_Shd->Begin(0, IZ_FALSE);
    {
        IZ_ASSERT(passCnt >= 1);
        if (m_Shd->BeginPass(GetShaderPassIdx())) {
            // シェーダ定数セット
            {
                SetShaderParam(
                    m_Shd,
                    "g_mW2C",
                    (void*)&camera.GetParam().mtxW2C,
                    sizeof(camera.GetParam().mtxW2C));
            }

            // テクスチャセット
            device->SetTexture(0, m_Img->GetTexture(GetTexIdx()));

            m_Shd->CommitChanges();

            // モデル描画
            m_Mdl->Render(device);

            m_Shd->EndPass();
        }
    }
    m_Shd->End();

    return IZ_TRUE;
}

IZ_BOOL CStateBase::InitObject(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT cameraZ,
    IZ_PCSTR imgPath,
    IZ_PCSTR mshPath,
    IZ_PCSTR sklPath,
    IZ_PCSTR anmPath,
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

    // Animation
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(anmPath));

        m_Anm = izanagi::CAnimation::CreateAnimation(
                    allocator,
                    &in);

        VGOTO(result = (m_Anm != IZ_NULL), __EXIT__);
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
        m_MdlRenderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<CSampleMdlRenderHandler>(allocator);
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

    // Timeline
    {
        m_Timeline.Init(
            m_Anm->GetAnimationTime(),
            0.0f);
        m_Timeline.SetIsLoop(IZ_TRUE);
        m_Timeline.SetIsReverse(IZ_FALSE);
        m_Timeline.Start();
        m_Timeline.Reset();
    }

    // カメラ
    m_App->GetCamera().Init(
        izanagi::math::CVector(0.0f, 5.0f, cameraZ, 1.0f),
        izanagi::math::CVector(0.0f, 5.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        1000.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
    m_App->GetCamera().Update();

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
    SAFE_RELEASE(m_Anm);
    SAFE_RELEASE(m_Shd);

    SAFE_RELEASE(m_MdlRenderHandler);
}
