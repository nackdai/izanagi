#include "graph/GraphUtil.h"
#include "graph/gles2/CubeTexture_GLES2.h"
#include "graph/ParamValueConverter.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    // テクスチャ作成
    CCubeTexture* CCubeTextureGLES2::CreateCubeTexture(
        CGraphicsDevice* device,
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
                                    &CCubeTextureGLES2::CreateTextureImpl);

        return instance;
    }

    CCubeTexture* CCubeTextureGLES2::CreateInternal(
        CGraphicsDevice* device,
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

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CCubeTextureGLES2* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CCubeTextureGLES2));
        if (!(result = (buf != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf)CCubeTextureGLES2;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;

            CGraphicsDeviceGLES2* glesDevice = reinterpret_cast<CGraphicsDeviceGLES2*>(device);
            SAFE_REPLACE(instance->m_Device, glesDevice);
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
    CCubeTextureGLES2::CCubeTextureGLES2()
    {
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
    CCubeTextureGLES2::~CCubeTextureGLES2()
    {
        SAFE_RELEASE(m_Device);

        if (m_Texture > 0) {
            CALL_GLES2_API(::glDeleteTextures(1, &m_Texture));
        }

        FREE(m_Allocator, m_TemporaryData);
    }

    // テクスチャ作成
    IZ_BOOL CCubeTextureGLES2::CreateTextureImpl(
        CGraphicsDevice* device,
        IZ_PCSTR path,
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt)
    {
        IZ_ASSERT(device != IZ_NULL);
        IZ_ASSERT(width == height);

        CALL_GLES2_API(::glGenTextures(1, &m_Texture));

        VRETURN(m_Texture > 0);

        // テクスチャ情報取得
        SetTextureInfo(
            width, height,
            mipLevel,
            fmt,
            E_GRAPH_RSC_USAGE_STATIC);

        return IZ_TRUE;
    }

    // テクスチャ情報取得
    void CCubeTextureGLES2::SetTextureInfo(
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE usage)
    {
        IZ_ASSERT(m_Texture > 0);

        m_TexInfo.width = width;
        m_TexInfo.height = height;
    
        m_TexInfo.level = mipLevel;
        m_TexInfo.fmt = fmt;

        m_TexInfo.is_rendertarget = IZ_FALSE;
        m_TexInfo.is_dynamic = IsDynamic();
        m_TexInfo.is_on_sysmem = IZ_FALSE;
    }

    void CCubeTextureGLES2::Initialize()
    {
        if (!m_IsInitialized) {
            GLuint width = GetWidth();
            GLuint height = GetHeight();

            for (IZ_UINT face = 0; face < E_GRAPH_CUBE_TEX_FACE_NUM; face++) {
                for (IZ_UINT level = 0; level < m_TexInfo.level; level++) {
                    GLuint w = width >> level;
                    GLuint h = height >> level;

                    CALL_GLES2_API(
                        ::glTexImage2D(
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                            level,
                            m_GLFormat,
                            w, h,
                            0,
                            m_GLFormat,
                            m_GLType,
                            IZ_NULL));
                }
            }

            m_IsInitialized = IZ_TRUE;
        }
    }

    /**
    * ロック
    */
    IZ_UINT CCubeTextureGLES2::Lock(
        E_GRAPH_CUBE_TEX_FACE face,
        IZ_UINT level,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        IZ_ASSERT(m_Texture > 0);

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
        m_LockedFace = face;

        *data = m_TemporaryData;

        return pitch;
    }

    /**
    * アンロック
    */
    IZ_BOOL CCubeTextureGLES2::Unlock(
        E_GRAPH_CUBE_TEX_FACE face,
        IZ_UINT level)
    {
        VRETURN(m_LockedLevel == level);
        VRETURN(m_LockedFace == face);

        IZ_BOOL isLocked = (m_LockedSize > 0);

        if (isLocked) {
            CBaseTexture* curTex = m_Device->GetTexture(0);
            GLenum glFace = IZ_GET_TARGET_CUBE_FACE(face);

            if (curTex != this) {
                CALL_GLES2_API(::glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture));
                
                Initialize();
            }
            
            IZ_UINT width = GetWidth(level);
            IZ_UINT height = GetHeight(level);

            CALL_GLES2_API(
                ::glTexSubImage2D(
                    glFace,
                    level,
                    0, 0,
                    width, height,
                    m_GLFormat,
                    m_GLType,
                    m_TemporaryData));

            // 元に戻す
            if (curTex != this) {
                if (curTex == IZ_NULL) {
                    CALL_GLES2_API(::glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
                }
                else {
                    CALL_GLES2_API(
                        ::glBindTexture(
                            curTex->GetTexType() == E_GRAPH_TEX_TYPE_PLANE ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP,
                            ((CCubeTextureGLES2*)curTex)->m_Texture));
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

    TEX_HANDLE CCubeTextureGLES2::GetTexHandle()
    {
        return m_Texture;
    }
}   // namespace graph
}   // namespace izanagi
