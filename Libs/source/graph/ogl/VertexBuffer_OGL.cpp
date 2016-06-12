#include "graph/ogl/VertexBuffer_OGL.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    std::atomic<IZ_BOOL> CVertexBufferOGL::s_isLocked(IZ_FALSE);

    // インスタンス作成
    CVertexBufferOGL* CVertexBufferOGL::CreateVertexBuffer(
        CGraphicsDeviceGLES2* device,
        IMemoryAllocator* allocator,
        IZ_UINT stride,
        IZ_UINT vtxNum,
        E_GRAPH_RSC_USAGE usage)
    {
        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CVertexBufferOGL* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CVertexBufferOGL));
        if (!(result = (buf != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf)CVertexBufferOGL;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_Device, device);
        }

        // 本体作成
        result = instance->CreateBody(
            device,
            stride,
            vtxNum,
            usage);
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

    // ロック
    IZ_BOOL CVertexBufferOGL::Lock(
        CGraphicsDevice* device,
        IZ_UINT offset,
        IZ_UINT size,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        IZ_ASSERT(!s_isLocked);

        // NOTE
        // Just only read or write, can't both read and write.

        IZ_ASSERT(!isReadOnly);

        IZ_UINT lockSize = (size > 0
            ? size
            : GetSize());

        IZ_ASSERT(lockSize + offset <= GetSize());

        void* tmp = nullptr;

#if 0
        CALL_GL_API(::glBindBuffer(
            GL_COPY_WRITE_BUFFER,
            m_VB));

        CALL_GL_API(tmp = ::glMapBufferRange(
            GL_COPY_WRITE_BUFFER,
            offset,
            lockSize,
            GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        IZ_ASSERT(tmp != nullptr);
#else
        CALL_GL_API(tmp = ::glMapNamedBufferRange(
            m_VB,
            offset,
            lockSize,
            GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        IZ_ASSERT(tmp != nullptr);
#endif

        *data = tmp;

        m_isLocked = IZ_TRUE;

        s_isLocked = IZ_TRUE;

        return IZ_TRUE;
    }

    /**
    * アンロック
    */
    IZ_BOOL CVertexBufferOGL::Unlock(CGraphicsDevice* device)
    {
        IZ_ASSERT(s_isLocked);

        IZ_ASSERT(m_VB > 0);

        IZ_BOOL isLocked = m_isLocked;
    
        if (isLocked) {
#if 0
            CALL_GL_API(glUnmapBuffer(GL_COPY_WRITE_BUFFER));

            CALL_GL_API(glBindBuffer(
                GL_COPY_WRITE_BUFFER,
                0));
#else
            CALL_GL_API(glUnmapNamedBuffer(m_VB));
#endif

            m_prevVB = 0;
            m_isLocked = IZ_FALSE;
        }

        s_isLocked = IZ_FALSE;

        return isLocked;
    }

    void CVertexBufferOGL::overrideNativeResource(void* rsc)
    {
        GLuint vb = (rsc ? *(GLuint*)rsc : 0);

        if (vb > 0) {
            if (m_VB > 0) {
                CALL_GL_API(::glDeleteBuffers(1, &m_VB));
            }
        }

        m_VB = vb;
    }
}   // namespace graph
}   // namespace izanagi
