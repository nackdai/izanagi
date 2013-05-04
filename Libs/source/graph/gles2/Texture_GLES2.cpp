#include "graph/GraphUtil.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/internal/ParamValueConverter.h"
//#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    // データからテクスチャ作成
    CTextureGLES2* CTextureGLES2::CreateTextureFromMemory(
        IMemoryAllocator* allocator,
        const void* data,
        IZ_UINT dataSize,
        E_GRAPH_PIXEL_FMT fmt)
    {
        
    }

    // コンストラクタ
    CTextureGLES2::CTextureGLES2()
    {
        m_Texture = IZ_NULL;

        m_TemporaryData = IZ_NULL;
    }

    // デストラクタ
    CTextureGLES2::~CTextureGLES2()
    {
        glDeleteTextures(1, &m_Texture);

        FREE(m_Allocator, m_TemporaryData);
    }

    // テクスチャ作成
    CTextureGLES2* CTextureGLES2::CreateTexture(
        IMemoryAllocator* allocator,
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE rscType)
    {
        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CTextureGLES2* instance = IZ_NULL;

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

        size_t size = sizeof(CTextureGLES2);

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
        instance = new (buf)CTextureGLES2;
        {
            buf += sizeof(CTextureGLES2);

            instance->AddRef();
            instance->m_Allocator = allocator;
        }

        IZ_ASSERT(CStdUtil::GetPtrDistance(top, buf) == size);

        // 本体作成
        result = instance->CreateBody_Texture(
                    width,
                    height,
                    mipLevel,
                    fmt,
                    rscType);
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
    IZ_BOOL CTextureGLES2::CreateBody_Texture(
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE rscType)
    {
        glGenTextures(1, &m_Texture);
        VRETURN(m_Texture > 0);

        glBindTexture(GL_TEXTURE_2D, m_Texture);

        SetTextureInfo(
            IZ_FALSE,
            width, height,
            mipLevel,
            fmt, rscType);

        return IZ_TRUE;
    }

    // テクスチャ情報取得
    void CTextureGLES2::SetTextureInfo(
        IZ_BOOL isRT,
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE rscType)
    {
        IZ_ASSERT(m_Texture != 0);

        m_TexInfo.width = width;
        m_TexInfo.height = height;
    
        m_TexInfo.level = mipLevel;
        m_TexInfo.fmt = fmt;

        m_TexInfo.typeRsc = rscType;

        m_TexInfo.is_rendertarget = isRT;
        m_TexInfo.is_dynamic = (rscType == E_GRAPH_RSC_USAGE_DYNAMIC);
        m_TexInfo.is_on_sysmem = IZ_FALSE;
        m_TexInfo.is_on_vram = isRT;
    }

    /**
    * ロック
    */
    IZ_UINT CTextureGLES2::Lock(
        UINT level,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        IZ_ASSERT(m_Texture != 0);

        if (level >= m_TexInfo.level) {
            // レベル指定オーバー
            IZ_ASSERT(IZ_FALSE);
            return 0;
        }

        IZ_UINT width = GetWidth();
        IZ_UINT bpp = CGraphUtil::GetBPP(GetPixelFormat());

        IZ_UINT pitch = width * bpp;

        // TODO
        // Align

        if (m_TemporaryData == IZ_NULL)
        {
            size_t size = pitch * GetHeight();
            m_TemporaryData = ALLOC(m_Allocator, size);
        }

        VRETURN(m_TemporaryData != IZ_NULL);

        *data = m_TemporaryData;

        return pitch;
    }

    /**
    * アンロック
    */
    IZ_BOOL CTextureGLES2::Unlock(IZ_UINT level)
    {
        if (!IsDynamic())
        {
            FREE(m_Allocator, m_TemporaryData);
            m_TemporaryData = IZ_NULL;
        }

        return IZ_TRUE;
    }

    IZ_BOOL CTextureGLES2::IsPrepared() const
    {
        return (m_Texture > 0);
    }

    // 本体解放
    IZ_BOOL CTextureGLES2::Disable()
    {
        glDeleteTextures(1, &m_Texture);
        m_Texture = 0;

        return IZ_TRUE;
    }

    // リセット
    IZ_BOOL CTextureGLES2::Restore()
    {
        // Nothing is done...

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
