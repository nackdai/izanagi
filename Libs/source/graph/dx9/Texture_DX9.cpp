#include "graph/dx9/Texture_DX9.h"
#include "graph/ParamValueConverter.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    namespace {
        struct SFuncCreateTextureFromFile {
            IZ_BOOL operator()(
                D3D_TEXTURE** pTex,
                CGraphicsDeviceDX9* device,
                IZ_PCSTR path,
                IZ_UINT nDummy,
                E_GRAPH_PIXEL_FMT fmt)
            {
                D3D_DEVICE* pD3DDev = device->GetRawInterface();

                D3DFORMAT dx9Fmt = (fmt >= E_GRAPH_PIXEL_FMT_NUM
                                    ? D3DFMT_FROM_FILE
                                    : IZ_GET_TARGET_PIXEL_FMT(fmt));

                HRESULT hr;
                hr = D3DXCreateTextureFromFileEx(
                        pD3DDev,
                        path,
                        D3DX_DEFAULT,       // width
                        D3DX_DEFAULT,       // height
                        D3DFMT_FROM_FILE,   // mip levels
                        0,                  // usage
                        dx9Fmt,             // format
                        D3DPOOL_MANAGED,    // pool
                        D3DX_FILTER_LINEAR, // filter
                        D3DX_FILTER_LINEAR, // mip filter
                        0,                  // color key
                        NULL,
                        NULL,
                        pTex);

                IZ_BOOL ret = SUCCEEDED(hr);
                IZ_ASSERT(ret);

                return ret;
            }
        };

        struct SFuncCreateTextureFromMemory {
            IZ_BOOL operator()(
                D3D_TEXTURE** pTex,
                CGraphicsDeviceDX9* device,
                const void* data,
                IZ_UINT dataSize,
                E_GRAPH_PIXEL_FMT fmt)
            {
                D3D_DEVICE* pD3DDev = device->GetRawInterface();

                D3DFORMAT dx9Fmt = (fmt >= E_GRAPH_PIXEL_FMT_NUM
                                    ? D3DFMT_FROM_FILE
                                    : IZ_GET_TARGET_PIXEL_FMT(fmt));

                HRESULT hr;
                hr = D3DXCreateTextureFromFileInMemoryEx (
                        pD3DDev,
                        data,
                        dataSize,
                        D3DX_DEFAULT,       // width
                        D3DX_DEFAULT,       // height
                        D3DFMT_FROM_FILE,   // mip levels
                        0,                  // usage
                        dx9Fmt,             // format
                        D3DPOOL_MANAGED,    // pool
                        D3DX_FILTER_LINEAR, // filter
                        D3DX_FILTER_LINEAR, // mip filter
                        0,                  // color key
                        NULL,
                        NULL,
                        pTex);

                IZ_BOOL ret = SUCCEEDED(hr);
                IZ_ASSERT(ret);

                return ret;
            }
        };
    }   // namespace

    // ファイルからテクスチャ作成
    CTextureDX9* CTextureDX9::CreateTextureFromFile(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        IZ_PCSTR path,
        E_GRAPH_PIXEL_FMT fmt)
    {
        CTextureDX9* ret = CreateBody_From(
                            device,
                            allocator,
                            path,
                            0,
                            SFuncCreateTextureFromFile(),
                            fmt);
        return ret;
    }

    // データからテクスチャ作成
    CTextureDX9* CTextureDX9::CreateTextureFromMemory(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        const void* data,
        IZ_UINT dataSize,
        E_GRAPH_PIXEL_FMT fmt)
    {
        CTextureDX9* ret = CreateBody_From(
                            device, allocator,
                            data, dataSize,
                            SFuncCreateTextureFromMemory(),
                            fmt);
        return ret;
    }

    // コンストラクタ
    CTextureDX9::CTextureDX9()
    {
        m_Texture = IZ_NULL;
        m_Next = IZ_NULL;
    }

    // デストラクタ
    CTextureDX9::~CTextureDX9()
    {
        m_Device->RemoveTexture(this);
        
        SAFE_RELEASE(m_Texture);
        SAFE_RELEASE(m_Device);
    }

    // 本体作成
    // 入力から作成
    template <typename _T, typename _Func>
    CTextureDX9* CTextureDX9::CreateBody_From(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        _T tInput,
        IZ_UINT inputSize,
        _Func& func,
        E_GRAPH_PIXEL_FMT fmt)
    {
        IZ_ASSERT(device != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CTextureDX9* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CTextureDX9));
        result = (buf != IZ_NULL);
        VGOTO(result, __EXIT__);

        // インスタンス作成
        instance = new (buf)CTextureDX9;
        {
            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_Device, device);

            instance->AddRef();
        }
    
        // 本体作成
        {
            // 本体作成
            result = func(
                        &instance->m_Texture,
                        device,
                        tInput,
                        inputSize,
                        fmt);
            IZ_ASSERT(result);

            instance->GetTextureInfo();
        }

        if (!result) {
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

    // テクスチャ作成
    CTextureDX9* CTextureDX9::CreateTexture(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE rscType,
        IZ_BOOL isOnSysMem/*= IZ_FALSE*/)
    {
        IZ_ASSERT(device != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CTextureDX9* instance = IZ_NULL;

        // 最大のミップレベルを計算
        IZ_UINT maxMipLevel = 1;
        {
            IZ_UINT nW = width;
            IZ_UINT nH = height;
            while ((nW > 1) && (nH > 1)) {
                nW >>= 1;
                nH >>= 1;
                ++maxMipLevel;
            }
        }

        size_t size = sizeof(CTextureDX9);

        if (mipLevel <= maxMipLevel) {
            mipLevel = (mipLevel == 0 ? maxMipLevel : mipLevel);
        }
        else {
            mipLevel = 1;
        }

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, size);
        result = (buf != IZ_NULL);
        VGOTO(result, __EXIT__);

        IZ_UINT8* top = buf;

        // インスタンス作成
        instance = new (buf)CTextureDX9;
        {
            buf += sizeof(CTextureDX9);

            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_Device, device);

            instance->AddRef();
        }

        IZ_ASSERT(CStdUtil::GetPtrDistance(top, buf) == size);

        // 本体作成
        result = instance->CreateBody_Texture(
                    width,
                    height,
                    mipLevel,
                    fmt,
                    rscType,
                    isOnSysMem);
        if (!result) {
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

    // 本体作成（テクスチャ）
    IZ_BOOL CTextureDX9::CreateBody_Texture(
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE rscType,
        IZ_BOOL isOnSysMem)
    {
        IZ_ASSERT(m_Device != IZ_NULL);

        D3D_DEVICE* pD3DDev = m_Device->GetRawInterface();
    
        IZ_DWORD nUsage = 0;
        if (!isOnSysMem) {
            nUsage = (rscType & E_GRAPH_RSC_USAGE_STATIC
                        ? 0
                        : D3DUSAGE_DYNAMIC);
        }

        D3DPOOL pool = D3DPOOL_SYSTEMMEM;
        if (!isOnSysMem) {
            pool = (rscType & E_GRAPH_RSC_USAGE_DYNAMIC
                        ? D3DPOOL_DEFAULT
                        : D3DPOOL_MANAGED);
        }

        D3DFORMAT dx9Fmt = IZ_GET_TARGET_PIXEL_FMT(fmt);

        // 本体作成
        HRESULT hr;
        hr = pD3DDev->CreateTexture(
                width,          // width
                height,     // height
                mipLevel,       // mip levels
                nUsage,         // usage
                dx9Fmt,         // format
                pool,           // pool
                &m_Texture,
                NULL);

        VRETURN(SUCCEEDED(hr));

        // テクスチャ情報取得
        GetTextureInfo();

        return IZ_TRUE;
    }

    // テクスチャ情報取得
    void CTextureDX9::GetTextureInfo()
    {
        IZ_ASSERT(m_Texture != IZ_NULL);

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

    /**
    * ロック
    */
    IZ_UINT CTextureDX9::Lock(
        IZ_UINT level,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        // NOTE
        // 動的テクスチャにおいて
        // ミップマップの場合、最上位レベルでのみ LOCK_DISCARD を使える
        // 最上位レベルをロックするだけですべてのレベルが破棄される

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
        if (IsDynamic()) {
            // READONLY不可
            VRETURN(!isReadOnly);

            if (isDiscard) {
                flag = D3DLOCK_DISCARD;
            }
            else {
                flag = D3DLOCK_NOOVERWRITE;
            }
        }
        else if (isReadOnly) {
            flag = D3DLOCK_READONLY;
        }

        D3DLOCKED_RECT rect;
        HRESULT hr = m_Texture->LockRect(
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
    IZ_BOOL CTextureDX9::Unlock(IZ_UINT level)
    {
        IZ_ASSERT(m_Texture != IZ_NULL);

        HRESULT hr = m_Texture->UnlockRect(level);
        IZ_BOOL ret = SUCCEEDED(hr);
        IZ_ASSERT(ret);

        return ret;
    }

    IZ_BOOL CTextureDX9::IsPrepared() const
    {
        return (m_Texture != IZ_NULL);
    }

    // 本体解放
    IZ_BOOL CTextureDX9::Disable()
    {
        SAFE_RELEASE(m_Texture);
        return IZ_TRUE;
    }

    // リセット
    IZ_BOOL CTextureDX9::Restore()
    {
        IZ_BOOL ret = IZ_TRUE;

        if (IsOnVram()) {
            SAFE_RELEASE(m_Texture);

            ret = CreateBody_Texture(
                    GetWidth(),
                    GetHeight(),
                    GetMipMapNum(),
                    GetPixelFormat(),
                    E_GRAPH_RSC_USAGE_DYNAMIC,
                    IsOnSysMem());
            IZ_ASSERT(ret);
        }

        return ret;
    }
}   // namespace graph
}   // namespace izanagi
