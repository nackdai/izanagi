#include "graph/dx9/RenderTarget_DX9.h"
#include "graph/dx9/Surface_DX9.h"
#include "graph/internal/ParamValueConverter.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    // コンストラクタ
    CRenderTargetDX9::CRenderTargetDX9()
    {
        m_Texture = IZ_NULL;
        m_Surface = IZ_NULL;
        m_Next = IZ_NULL;
    }

    // デストラクタ
    CRenderTargetDX9::~CRenderTargetDX9()
    {
        m_Device->RemoveRenderTarget(this);

        SAFE_RELEASE(m_Surface);
        SAFE_RELEASE(m_Texture);
        SAFE_RELEASE(m_Device);
    }

    // レンダーターゲット作成
    CRenderTargetDX9* CRenderTargetDX9::CreateRenderTarget(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        IZ_UINT width, 
        IZ_UINT height,
        E_GRAPH_PIXEL_FMT fmt)
    {
        IZ_ASSERT(device != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CRenderTargetDX9* instance = IZ_NULL;

        size_t size = sizeof(CRenderTargetDX9) + sizeof(CSurfaceDX9);

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, size);
        result = (buf != IZ_NULL);
        VGOTO(result, __EXIT__);

        IZ_UINT8* top = buf;

        // インスタンス作成
        instance = new (buf) CRenderTargetDX9;
        {
            buf += sizeof(CRenderTargetDX9);

            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_Device, device);

            instance->m_Surface = reinterpret_cast<CSurfaceDX9*>(buf);

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

    // レンダーターゲット作成
    CRenderTargetDX9* CRenderTargetDX9::CreateRenderTarget(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        CSurfaceDX9* surface)
    {
        IZ_ASSERT(device != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CRenderTargetDX9* instance = IZ_NULL;

        size_t size = sizeof(CRenderTargetDX9) + sizeof(CSurfaceDX9);

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, size);
        result = (buf != IZ_NULL);
        VGOTO(result, __EXIT__);

        IZ_UINT8* top = buf;

        // インスタンス作成
        instance = new (buf) CRenderTargetDX9;
        {
            buf += sizeof(CRenderTargetDX9);

            instance->AddRef();

            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_Device, device);

            SAFE_REPLACE(instance->m_Surface, surface);
        }

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
    IZ_BOOL CRenderTargetDX9::CreateBody_RenderTarget(
        IZ_UINT width, 
        IZ_UINT height,
        E_GRAPH_PIXEL_FMT fmt)
    {
        IZ_ASSERT(m_Device != IZ_NULL);

        D3D_DEVICE* pD3DDev = m_Device->GetRawInterface();

        D3DFORMAT dx9Fmt = IZ_GET_TARGET_PIXEL_FMT(fmt);

        // NOTE
        // poolはD3DPOOL_DEFAULTしか許されていない

        // 本体作成
        HRESULT hr;
        hr = pD3DDev->CreateTexture(
                width,                  // width
                height,                 // height
                1,                      // mip levels
                D3DUSAGE_RENDERTARGET,  // usage
                dx9Fmt,                 // format
                D3DPOOL_DEFAULT,        // pool
                &m_Texture,
                NULL);

        VRETURN(SUCCEEDED(hr));

        // テクスチャ情報取得
        SetDesc();

        return IZ_TRUE;
    }

    // テクスチャ情報取得
    void CRenderTargetDX9::SetDesc()
    {
        if (m_Texture != IZ_NULL)
        {
            D3DSURFACE_DESC desc;

            m_Texture->GetLevelDesc(0, &desc);

            m_TexInfo.width = desc.Width;
            m_TexInfo.height = desc.Height;
    
            m_TexInfo.level = static_cast<IZ_UINT8>(m_Texture->GetLevelCount());
            m_TexInfo.fmt = IZ_GET_ABST_PIXEL_FMT(desc.Format);

            m_TexInfo.is_rendertarget = (desc.Usage == D3DUSAGE_RENDERTARGET);
            m_TexInfo.is_dynamic = ((desc.Usage & D3DUSAGE_DYNAMIC) > 0);
            m_TexInfo.is_on_sysmem = (desc.Pool == D3DPOOL_SYSTEMMEM);
            m_TexInfo.is_on_vram = (desc.Pool == D3DPOOL_DEFAULT);

            m_TexInfo.usage = E_GRAPH_RSC_USAGE_STATIC;
            if (m_TexInfo.is_dynamic) {
                if (m_TexInfo.is_on_vram) {
                    m_TexInfo.usage = E_GRAPH_RSC_USAGE_STATIC_DYNAMIC;
                }
                else {
                    m_TexInfo.usage = E_GRAPH_RSC_USAGE_DYNAMIC;
                }
            }
        }
        else if (m_Surface != IZ_NULL) {
            m_TexInfo.width = m_Surface->GetWidth();
            m_TexInfo.height = m_Surface->GetHeight();
    
            m_TexInfo.level = 1;
            m_TexInfo.fmt = m_Surface->GetPixelFormat();

            m_TexInfo.is_rendertarget = IZ_TRUE;

            // TODO
            // 本当？
            m_TexInfo.is_dynamic = IZ_FALSE;
            m_TexInfo.is_on_sysmem = IZ_FALSE;
            m_TexInfo.is_on_vram = IZ_TRUE;
            m_TexInfo.usage = E_GRAPH_RSC_USAGE_STATIC;
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }
    }

    // サーフェス作成
    IZ_BOOL CRenderTargetDX9::CreateSurface()
    {
        // サーフェス作成
        if (m_Surface != IZ_NULL) {
            m_Surface = CSurfaceDX9::CreateSurface(m_Allocator);
            IZ_BOOL result = (m_Surface != IZ_NULL);

            if (result) {
                result = m_Surface->Reset(this, 0);
                VRETURN(result);
            }
            else {
                IZ_ASSERT(IZ_FALSE);
                return IZ_FALSE;
            }
        }

        return IZ_TRUE;
    }

    IZ_BOOL CRenderTargetDX9::IsPrepared() const
    {
        return (m_Texture != IZ_NULL);
    }

    // 本体解放
    IZ_BOOL CRenderTargetDX9::Disable()
    {
        if (m_Surface != IZ_NULL) {
            m_Surface->ReleaseResource();
        }

        SAFE_RELEASE(m_Texture);

        return IZ_TRUE;
    }

    // リセット
    IZ_BOOL CRenderTargetDX9::Restore()
    {
        IZ_BOOL ret = IZ_TRUE;

        // RenderTargetは強制的にリセット

        SAFE_RELEASE(m_Texture);

        ret = CreateBody_RenderTarget(
                GetWidth(),
                GetHeight(),
                GetPixelFormat());
        IZ_ASSERT(ret);

        if (ret) {
            // サーフェスもリセット
            ret = m_Surface->Reset(this, 0);
            IZ_ASSERT(ret);
        }

        return ret;
    }

    // サーフェス取得
    CSurfaceDX9* CRenderTargetDX9::GetSurface()
    {
        return m_Surface;
    }

}   // namespace graph
}   // namespace izanagi
