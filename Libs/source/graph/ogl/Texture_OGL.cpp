#include "graph/ogl/Texture_OGL.h"
#include "graph/ogl/GraphicsDevice_OGL.h"
#include "graph/GraphUtil.h"

namespace izanagi
{
namespace graph
{
    // データからテクスチャ作成
    CTextureOGL* CTextureOGL::CreateTextureFromMemory(
        CGraphicsDeviceOGL* device,
        IMemoryAllocator* allocator,
        const void* data,
        IZ_UINT dataSize,
        E_GRAPH_PIXEL_FMT fmt)
    {
        // TODO
        return IZ_FALSE;
    }

    // テクスチャ作成
    CTextureOGL* CTextureOGL::CreateTexture(
        CGraphicsDeviceOGL* device,
        IMemoryAllocator* allocator,
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE rscType)
    {
        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CTextureOGL* instance = IZ_NULL;

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

        size_t size = sizeof(CTextureOGL);

        if (mipLevel <= maxMipLevel) {
            mipLevel = (mipLevel == 0 ? maxMipLevel : mipLevel);
        }
        else {
            mipLevel = 1;
        }

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, size);
        IZ_UINT8* top = buf;

        result = (buf != IZ_NULL);
        VGOTO(result, __EXIT__);

        // インスタンス作成
        instance = new (buf)CTextureOGL;
        {
            buf += sizeof(CTextureOGL);

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

    /**
    * アンロック
    */
    IZ_BOOL CTextureOGL::Unlock(IZ_UINT level)
    {
        VRETURN(m_LockedLevel == level);

        IZ_BOOL isLocked = (m_LockedSize > 0);

        if (isLocked) {
            IZ_UINT width = GetWidth(level);
            IZ_UINT height = GetHeight(level);

            auto fmt = GetPixelFormat();

            if (CGraphUtil::IsCompressedPixelFormat(fmt)) {
                // TODO
                // DXTではmipmapを許さない.
                IZ_ASSERT(level == 0);

                CALL_GL_API(::glCompressedTextureSubImage2D(
                    m_Texture,
                    level,
                    0, 0,
                    width, height,
                    m_GLFormat,
                    (GLsizei)m_LockedSize,
                    m_TemporaryData));
            }
            else {
                CALL_GL_API(::glTextureSubImage2D(
                    m_Texture,
                    level,
                    0, 0,
                    width, height,
                    m_GLFormat,
                    m_GLType,
                    m_TemporaryData));
            }

            m_LockedSize = 0;
        }

#if 0
        if (!IsDynamic()) {
            FREE(m_Allocator, m_TemporaryData);
            m_TemporaryData = IZ_NULL;
        }
#endif

        m_LockedLevel = -1;

        return IZ_TRUE;
    }

    IZ_BOOL CTextureOGL::Write(
        IZ_UINT level,
        void* data,
        IZ_UINT x, IZ_UINT y,
        IZ_UINT width, IZ_UINT height)
    {
        IZ_ASSERT(m_Texture != 0);

        // Check not locked.
        IZ_ASSERT(m_LockedSize == 0);

        IZ_ASSERT(x < width);
        IZ_ASSERT(y < height);

        IZ_UINT w = GetWidth(level);
        IZ_UINT h = GetHeight(level);

        VRETURN(width <= w);
        VRETURN(height <= h);

        CALL_GL_API(::glTextureSubImage2D(
            m_Texture,
            level,
            x, y,
            width, height,
            m_GLFormat,
            m_GLType,
            data));

        return IZ_TRUE;
    }

    void CTextureOGL::overrideNativeResource(void* rsc, E_GRAPH_OVERRIDE_NATIVE_RSC_BEHAVIOUR behaviour)
    {
        GLuint tex = (rsc ? (GLuint)rsc : 0);

        if (behaviour == E_GRAPH_OVERRIDE_NATIVE_RSC_BEHAVIOUR::Replace) {
            if (tex > 0) {
                if (m_Texture > 0) {
                    CALL_GL_API(::glDeleteBuffers(1, &m_Texture));
                }
            }
        }

        m_Texture = tex;
    }
}   // namespace graph
}   // namespace izanagi
