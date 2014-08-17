#include "ArchiveTarball.h"

static izanagi::sample::CSampleMdlRenderHandler* s_MdlRenderHandler = IZ_NULL;

CArchiveTarballApp::CArchiveTarballApp()
{
    m_Img = IZ_NULL;
    m_Mdl = IZ_NULL;
    m_Msh = IZ_NULL;
    m_Skl = IZ_NULL;
    m_Shd = IZ_NULL;
}

CArchiveTarballApp::~CArchiveTarballApp()
{
}

// 初期化.
IZ_BOOL CArchiveTarballApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    {
        VRETURN(m_Input.Open("asset.tar"));

        m_Archive = izanagi::resource::CArchiveTar::CreateArchive(
            allocator,
            &m_Input);
        IZ_ASSERT(m_Archive != IZ_NULL);
    }

    izanagi::CMesh* msh = IZ_NULL;
    izanagi::CSkeleton* skl = IZ_NULL;

    // Texture
    {
        izanagi::resource::SArchiveFileDesc desc;
        m_Archive->SeekByPath("asset/ModelImage.img", &desc);

        void* buf = ALLOC(allocator, desc.size);
        m_Archive->Read(buf, desc);

        izanagi::CMemoryInputStream in(buf, desc.size);

        m_Img = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);

        FREE(allocator, buf);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
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

    // Mesh
    {
        izanagi::resource::SArchiveFileDesc desc;
        m_Archive->SeekByPath("asset/Seymour.msh", &desc);

        void* buf = ALLOC(allocator, desc.size);
        m_Archive->Read(buf, desc);

        izanagi::CMemoryInputStream in(buf, desc.size);

        m_Msh = izanagi::CMesh::CreateMesh(
                    allocator,
                    device,
                    &in);
        
        FREE(allocator, buf);
        VGOTO(result = (m_Msh != IZ_NULL), __EXIT__);
    }

    // Skelton
    {
        izanagi::resource::SArchiveFileDesc desc;
        m_Archive->SeekByPath("asset/Seymour.skl", &desc);

        void* buf = ALLOC(allocator, desc.size);
        m_Archive->Read(buf, desc);

        izanagi::CMemoryInputStream in(buf, desc.size);

        m_Skl = izanagi::CSkeleton::CreateSkeleton(
                    allocator,
                    &in);

        FREE(allocator, buf);
        VGOTO(result = (m_Skl != IZ_NULL), __EXIT__);
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

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 5.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 5.0f, 0.0f, 1.0f),
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
void CArchiveTarballApp::ReleaseInternal()
{
    SAFE_RELEASE(s_MdlRenderHandler);

    SAFE_RELEASE(m_Archive);

    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Mdl);
    SAFE_RELEASE(m_Msh);
    SAFE_RELEASE(m_Skl);
    SAFE_RELEASE(m_Shd);

    m_Input.Close();
}

// 更新.
void CArchiveTarballApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    m_Mdl->Update();
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
void CArchiveTarballApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    IZ_UINT passCnt = m_Shd->Begin(device, 0, IZ_FALSE);
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
            device->SetTexture(0, m_Img->GetTexture(0));

            m_Shd->CommitChanges(device);

            // モデル描画
            m_Mdl->Render(device);

            m_Shd->EndPass();
        }
    }
    m_Shd->End(device);
}
