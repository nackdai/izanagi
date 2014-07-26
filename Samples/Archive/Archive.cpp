#include "Archive.h"
#include "data/Archive.h"

static izanagi::sample::CSampleMdlRenderHandler* s_MdlRenderHandler = IZ_NULL;

CArchiveApp::CArchiveApp()
{
    m_Img = IZ_NULL;
    m_Mdl = IZ_NULL;
    m_Shd = IZ_NULL;
}

CArchiveApp::~CArchiveApp()
{
}

// 初期化.
IZ_BOOL CArchiveApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    {
        VRETURN(m_Input.Open("data/Archive.arc"));

        m_Archive = izanagi::resource::CArchive::CreateArchive(
            allocator,
            &m_Input);
        IZ_ASSERT(m_Archive != IZ_NULL);
    }

    izanagi::CMesh* msh = IZ_NULL;
    izanagi::CSkeleton* skl = IZ_NULL;

    // Texture
    {
        izanagi::resource::SArchiveFileDesc desc;
        m_Archive->SeekByKey(Archive::ModelImage, &desc);

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

    // Model
    {
        izanagi::resource::SArchiveFileDesc desc;
        m_Archive->SeekByKey(Archive::Seymour, &desc);

        void* buf = ALLOC(allocator, desc.size);
        m_Archive->Read(buf, desc);

        izanagi::CMemoryInputStream in(buf, desc.size);

        m_Mdl = izanagi::CModel::CreateModel(
            allocator,
            device,
            &in,
            s_MdlRenderHandler);

        FREE(allocator, buf);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
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
void CArchiveApp::ReleaseInternal()
{
    SAFE_RELEASE(s_MdlRenderHandler);

    SAFE_RELEASE(m_Archive);

    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Mdl);
    SAFE_RELEASE(m_Shd);

    m_Input.Close();
}

// 更新.
void CArchiveApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
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
void CArchiveApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
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
