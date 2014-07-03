#include "PhotoGalleryApp.h"
#include "Seat.h"
#include "PhotoItem.h"
#include "LoadTextureJob.h"

PhotoGalleryApp::PhotoGalleryApp()
{
    m_Shader = IZ_NULL;
    m_Seat = IZ_NULL;

    m_Item = IZ_NULL;
}

PhotoGalleryApp::~PhotoGalleryApp()
{
}

// 初期化.
IZ_BOOL PhotoGalleryApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/BasicShader.shd"), __EXIT__);

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    m_Seat = Seat::Create(allocator, device);
    VGOTO(result = (m_Seat != IZ_NULL), __EXIT__);

    m_Item = PhotoItem::Create(allocator, device);
    VGOTO(result = (m_Item != IZ_NULL), __EXIT__);

    // カメラ
    camera.Init(
        izanagi::math::CVector(0.0f, 10.0f, 30.0f, 1.0f),
        izanagi::math::CVector(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    TextureLoader::Instance().Init(allocator);

    TextureLoader::Instance().EnqueueLoadinRequest(
        device,
        "data/test.jpg",
        m_Item);

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void PhotoGalleryApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Seat);

    SAFE_RELEASE(m_Item);

    TextureLoader::Instance().Terminate();
    izanagi::threadmodel::CJobQueue::TerminateJobQueue();
}

// 更新.
void PhotoGalleryApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    izanagi::threadmodel::CJobQueue::UpdateQueues();
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
void PhotoGalleryApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    static const IZ_BOOL isDrawTangentSpaceAxis = IZ_FALSE;

    izanagi::sample::CSampleCamera& camera = GetCamera();

    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::SetUnit(mtxL2W);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            // シェーダ設定
            m_Shader->CommitChanges(device);

            m_Item->RenderFront(device);

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    m_Shader->Begin(device, 1, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            // シェーダ設定
            m_Shader->CommitChanges(device);

            m_Item->RenderTopAndSide(device);

            m_Shader->EndPass();
        }
    }
}
