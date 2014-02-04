#include "graph/dx9/CubeTexture_DX9.h"
#include "graph/ParamValueConverter.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    // テクスチャ作成
    CCubeTexture* CCubeTextureDX9::CreateCubeTexture(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt)
    {
        CCubeTexture* instance = CreateInternal(
                                    device,
                                    allocator,
                                    IZ_NULL,
                                    width,
                                    height,
                                    mipLevel,
                                    fmt,
                                    &CCubeTextureDX9::CreateTextureImpl);

        return instance;
    }

    CCubeTexture* CCubeTextureDX9::CreateInternal(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        IZ_PCSTR path,
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        CreateTexFunc pCreateTexFunc)
    {
        IZ_ASSERT(device != IZ_NULL);
        IZ_ASSERT(pCreateTexFunc != IZ_NULL);

        D3D_DEVICE* d3dDev = device->GetRawInterface();

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CCubeTextureDX9* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CCubeTextureDX9));
        if (!(result = (buf != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf)CCubeTextureDX9;
        {
            instance->m_Allocator = allocator;
            instance->AddRef();
        }

        // 初期化
        result = (instance->*pCreateTexFunc)(
                    device,
                    path,
                    width,
                    height,
                    mipLevel,
                    fmt);
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
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

    // コンストラクタ
    CCubeTextureDX9::CCubeTextureDX9()
    {
        m_Texture = IZ_NULL;
    }

    // デストラクタ
    CCubeTextureDX9::~CCubeTextureDX9()
    {
        SAFE_RELEASE(m_Texture);
    }

    // テクスチャ作成
    IZ_BOOL CCubeTextureDX9::CreateTextureImpl(
        CGraphicsDeviceDX9* device,
        IZ_PCSTR path,
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt)
    {
        IZ_ASSERT(device != IZ_NULL);
        IZ_ASSERT(width == height);

        D3D_DEVICE* d3dDev = device->GetRawInterface();

        D3DFORMAT d3dFmt = IZ_GET_TARGET_PIXEL_FMT(fmt);

        HRESULT hr;
        hr = D3DXCreateCubeTexture(
                d3dDev,
                width,              // size
                mipLevel,           // mip levels
                0,                  // usage
                d3dFmt,             // format
                D3DPOOL_MANAGED,    // pool
                &m_Texture);

        VRETURN(SUCCEEDED(hr));

        // テクスチャ情報取得
        GetTextureInfo();

        return IZ_TRUE;
    }

    // テクスチャ情報取得
    void CCubeTextureDX9::GetTextureInfo()
    {
        IZ_ASSERT(m_Texture != IZ_NULL);

        D3DSURFACE_DESC sDesc;

        m_Texture->GetLevelDesc(0, &sDesc);

        m_TexInfo.width = sDesc.Width;
        m_TexInfo.height = sDesc.Height;
    
        m_TexInfo.level = static_cast<IZ_UINT8>(m_Texture->GetLevelCount());
        m_TexInfo.fmt = IZ_GET_ABST_PIXEL_FMT(sDesc.Format);

        m_TexInfo.is_rendertarget = (sDesc.Usage == D3DUSAGE_RENDERTARGET);
        m_TexInfo.is_dynamic = ((sDesc.Usage & D3DUSAGE_DYNAMIC) > 0);
        m_TexInfo.is_on_sysmem = (sDesc.Pool == D3DPOOL_SYSTEMMEM);
    }

    /**
    * ロック
    */
    IZ_UINT CCubeTextureDX9::Lock(
        E_GRAPH_CUBE_TEX_FACE face,
        IZ_UINT level,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        IZ_ASSERT(m_Texture != IZ_NULL);

#if 0
        if (!IsDynamic() && !IsOnSysMem()) {
            // 動的テクスチャ以外ではロック不可
            // ただし、システムメモリ上にあるときはＯＫ
            IZ_ASSERT(IZ_FALSE);
            return 0;
        }
#endif

        if (level >= m_TexInfo.level) {
            // レベル指定オーバー
            IZ_ASSERT(IZ_FALSE);
            return 0;
        }

        DWORD flag = 0;
        if (isReadOnly) {
            flag = D3DLOCK_READONLY;
        }
        else if (IsDynamic()) {
            if (isDiscard) {
                flag = D3DLOCK_DISCARD;
            }
            else {
                flag = D3DLOCK_NOOVERWRITE;
            }
        }

        D3DLOCKED_RECT rect;
        HRESULT hr = m_Texture->LockRect(
                        IZ_GET_TARGET_CUBE_FACE(face),
                        level,
                        &rect,
                        NULL,
                        flag);
        IZ_ASSERT(SUCCEEDED(hr));

        IZ_UINT ret = 0;

        if (SUCCEEDED(hr)) {
            *data = rect.pBits;
            ret = rect.Pitch;
        }

        return ret;
    }

    /**
    * アンロック
    */
    IZ_BOOL CCubeTextureDX9::Unlock(
        E_GRAPH_CUBE_TEX_FACE face,
        IZ_UINT level)
    {
        IZ_ASSERT(m_Texture != IZ_NULL);

        HRESULT hr = m_Texture->UnlockRect(
                        IZ_GET_TARGET_CUBE_FACE(face),
                        level);
        IZ_BOOL ret = SUCCEEDED(hr);
        IZ_ASSERT(ret);

        return ret;
    }

    TEX_HANDLE CCubeTextureDX9::GetTexHandle()
    {
        return m_Texture;
    }
}   // namespace graph
}   // namespace izanagi
