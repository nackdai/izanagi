#include "graph/GraphUtil.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/ParamValueConverter.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    // データからテクスチャ作成
    CTextureGLES2* CTextureGLES2::CreateTextureFromMemory(
        CGraphicsDeviceGLES2* device,
        IMemoryAllocator* allocator,
        const void* data,
        IZ_UINT dataSize,
        E_GRAPH_PIXEL_FMT fmt)
    {
        // TODO
        return IZ_FALSE;
    }

    // コンストラクタ
    CTextureGLES2::CTextureGLES2()
    {
        m_Device = IZ_NULL;

        m_Texture = 0;

        m_IsInitialized = IZ_FALSE;

        m_Size = 0;

        m_LockedSize = 0;
        m_LockedLevel = -1;

        m_TemporaryData = IZ_NULL;

        m_GLFormat = GL_RGBA;
        m_GLType = GL_UNSIGNED_BYTE;
    }

    // デストラクタ
    CTextureGLES2::~CTextureGLES2()
    {
        SAFE_RELEASE(m_Device);

        ::glDeleteTextures(1, &m_Texture);

        FREE(m_Allocator, m_TemporaryData);
    }

    // テクスチャ作成
    CTextureGLES2* CTextureGLES2::CreateTexture(
        CGraphicsDeviceGLES2* device,
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

            SAFE_REPLACE(instance->m_Device, device);
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
        ::glGenTextures(1, &m_Texture);
        VRETURN(m_Texture > 0);

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
        E_GRAPH_RSC_USAGE usage)
    {
        IZ_ASSERT(m_Texture != 0);

        m_TexInfo.width = width;
        m_TexInfo.height = height;
    
        m_TexInfo.level = mipLevel;
        m_TexInfo.fmt = fmt;

        m_TexInfo.usage = usage;

        m_TexInfo.is_rendertarget = isRT;
        m_TexInfo.is_dynamic = (usage == E_GRAPH_RSC_USAGE_DYNAMIC);
        m_TexInfo.is_on_sysmem = IZ_FALSE;
        m_TexInfo.is_on_vram = isRT;

        IZ_UINT bpp = CGraphUtil::GetBPP(fmt);

        m_Size = width * height * bpp;

        CTargetParamValueConverter::ConvAbstractToTarget_PixelFormat(
            fmt,
            m_GLFormat,
            m_GLType);
    }

    void CTextureGLES2::Initialize()
    {
        if (!m_IsInitialized) {
            GLuint width = GetWidth();
            GLuint height = GetHeight();

            for (IZ_UINT i = 0; i < m_TexInfo.level; i++) {
                GLuint w = width >> i;
                GLuint h = height >> i;

                ::glTexImage2D(
                    GL_TEXTURE_2D,
                    i,
                    m_GLFormat,
                    w, h,
                    0,
                    m_GLFormat,
                    m_GLType,
                    IZ_NULL);
            }

            m_IsInitialized = IZ_TRUE;
        }
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

        // レベル指定
        VRETURN_VAL(level < m_TexInfo.level, 0);

        IZ_UINT width = GetWidth(level);
        IZ_UINT bpp = CGraphUtil::GetBPP(GetPixelFormat());

        IZ_UINT pitch = width * bpp;

        // TODO
        // Align

        size_t size = pitch * GetHeight(level);

        if (m_TemporaryData == IZ_NULL) {
            m_TemporaryData = ALLOC(m_Allocator, size);
        }
        else if (size > m_Size) {
            m_TemporaryData = REALLOC(m_Allocator, m_TemporaryData, size);
        }

        VRETURN(m_TemporaryData != IZ_NULL);

        m_LockedSize = size;
        m_LockedLevel = level;

        *data = m_TemporaryData;

        return pitch;
    }

    /**
    * アンロック
    */
    IZ_BOOL CTextureGLES2::Unlock(IZ_UINT level)
    {
        VRETURN(m_LockedLevel == level);

        IZ_BOOL isLocked = (m_LockedSize > 0);

        if (isLocked) {
            CBaseTexture* curTex = m_Device->GetTexture(0);

            if (curTex != this) {
                ::glBindTexture(GL_TEXTURE_2D, m_Texture);
                
                Initialize();
            }
            
            IZ_UINT width = GetWidth(level);
            IZ_UINT height = GetHeight(level);

            ::glTexSubImage2D(
                GL_TEXTURE_2D,
                level,
                0, 0,
                width, height,
                m_GLFormat,
                m_GLType,
                m_TemporaryData);

            // 元に戻す
            if (curTex != this) {
                if (curTex == IZ_NULL) {
                    ::glBindTexture(GL_TEXTURE_2D, 0);
                }
                else {
                    ::glBindTexture(
                        curTex->GetTexType() == E_GRAPH_TEX_TYPE_PLANE ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP,
                        ((CTextureGLES2*)curTex)->m_Texture);
                }
            }

            m_LockedSize = 0;
        }

        if (!IsDynamic()) {
            FREE(m_Allocator, m_TemporaryData);
            m_TemporaryData = IZ_NULL;
        }

        m_LockedLevel = -1;

        return IZ_TRUE;
    }

    IZ_BOOL CTextureGLES2::IsPrepared() const
    {
        return (m_Texture > 0);
    }

    // 本体解放
    IZ_BOOL CTextureGLES2::Disable()
    {
        ::glDeleteTextures(1, &m_Texture);
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
