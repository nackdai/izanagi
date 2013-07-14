#include "graph/gles2/RenderTarget_GLES2.h"
#include "graph/ParamValueConverter.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    // コンストラクタ
    CRenderTargetGLES2::CRenderTargetGLES2()
    {
        m_Texture = 0;
    }

    // デストラクタ
    CRenderTargetGLES2::~CRenderTargetGLES2()
    {
        if (m_Texture > 0) {
            CALL_GLES2_API(::glDeleteTextures(1, &m_Texture));
        }
    }

    // レンダーターゲット作成
    CRenderTargetGLES2* CRenderTargetGLES2::CreateRenderTarget(
        CGraphicsDeviceGLES2* device,
        IMemoryAllocator* allocator,
        IZ_UINT width, 
        IZ_UINT height,
        E_GRAPH_PIXEL_FMT fmt)
    {
        IZ_ASSERT(device != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CRenderTargetGLES2* instance = IZ_NULL;

        size_t size = sizeof(CRenderTargetGLES2);

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, size);
        result = (buf != IZ_NULL);
        VGOTO(result, __EXIT__);

        IZ_UINT8* top = buf;

        // インスタンス作成
        instance = new (buf) CRenderTargetGLES2;
        {
            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_Device, device);

            instance->AddRef();
        }

        // 本体作成
        result = instance->CreateBody_RenderTarget(
                    width, height,
                    fmt);
        VGOTO(result, __EXIT__);

        // サーフェス作成
        result = instance->CreateSurface();
        VGOTO(result, __EXIT__);

    __EXIT__:
        if (!result) {
            if (instance != IZ_NULL) {
                SAFE_RELEASE(instance);
            }
            else if (buf != IZ_NULL) {
                allocator->Free(buf);
            }
        }

        return instance;
    }

    // 本体作成（レンダーターゲット）
    IZ_BOOL CRenderTargetGLES2::CreateBody_RenderTarget(
        IZ_UINT width, 
        IZ_UINT height,
        E_GRAPH_PIXEL_FMT fmt)
    {
        IZ_ASSERT(m_Device != IZ_NULL);
        return IZ_TRUE;
    }

    // テクスチャ情報取得
    void CRenderTargetGLES2::SetDesc()
    {
    }

    // サーフェス作成
    IZ_BOOL CRenderTargetGLES2::CreateSurface()
    {
        return IZ_TRUE;
    }

    IZ_BOOL CRenderTargetGLES2::IsPrepared() const
    {
        return (m_Texture != IZ_NULL);
    }

    // 本体解放
    IZ_BOOL CRenderTargetGLES2::Disable()
    {
        return IZ_TRUE;
    }

    // リセット
    IZ_BOOL CRenderTargetGLES2::Restore()
    {
        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
