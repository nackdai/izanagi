#include "StateBase.h"
#include "izSampleKit.h"
#include "StateManager.h"
#include "ModelApp.h"

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
    m_Mdl->Update();

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

    IZ_UINT passCnt = m_Shd->Begin(device, 0, IZ_FALSE);
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

            m_Shd->CommitChanges(device);

            // モデル描画
            m_Mdl->Render(device);

            m_Shd->EndPass();
        }
    }
    m_Shd->End(device);

    return IZ_TRUE;
}

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

    // Model
    {
        m_Mdl = izanagi::CModel::CreateModel(
                    allocator,
                    m_Msh,
                    m_Skl,
                    m_MdlRenderHandler);

        VGOTO(result = (m_Mdl != IZ_NULL), __EXIT__);
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
    SAFE_RELEASE(m_Shd);

    SAFE_RELEASE(m_MdlRenderHandler);
}
