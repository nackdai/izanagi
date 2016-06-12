#include <atomic>
#include "graph/ogl/IndexBuffer_OGL.h"
#include "graph/ogl/VertexBuffer_OGL.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CIndexBufferOGL* CIndexBufferOGL::CreateIndexBuffer(
        CGraphicsDeviceGLES2* device,
        IMemoryAllocator* allocator,
        IZ_UINT idxNum,
        E_GRAPH_INDEX_BUFFER_FMT fmt,
        E_GRAPH_RSC_USAGE usage)
    {
        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CIndexBufferOGL* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CIndexBufferOGL));
        if (!(result = (buf != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf)CIndexBufferOGL;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_Device, device);
        }

        // 本体作成
        result = instance->CreateBody(
            idxNum,
            fmt,
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
    IZ_BOOL CIndexBufferOGL::Lock(
        CGraphicsDevice* device,
        IZ_UINT offset,
        IZ_UINT size,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        IZ_ASSERT(!CVertexBufferOGL::s_isLocked);

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
            m_IB));

        CALL_GL_API(tmp = ::glMapBufferRange(
            GL_COPY_WRITE_BUFFER,
            offset,
            lockSize,
            GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        IZ_ASSERT(tmp != nullptr);
#else
        CALL_GL_API(tmp = ::glMapNamedBufferRange(
            m_IB,
            offset,
            lockSize,
            GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        IZ_ASSERT(tmp != nullptr);
#endif

        *data = tmp;

        m_isLocked = IZ_TRUE;

        CVertexBufferOGL::s_isLocked = IZ_TRUE;

        return IZ_TRUE;
    }

    /**
    * アンロック
    */
    IZ_BOOL CIndexBufferOGL::Unlock(CGraphicsDevice* device)
    {
        IZ_ASSERT(CVertexBufferOGL::s_isLocked);

        IZ_ASSERT(m_IB > 0);

        IZ_BOOL isLocked = m_isLocked;
    
        if (isLocked) {
            GLboolean result = GL_FALSE;
#if 0
            CALL_GL_API(result = glUnmapBuffer(GL_COPY_WRITE_BUFFER));

            CALL_GL_API(glBindBuffer(
                GL_COPY_WRITE_BUFFER,
                0));
#else
            CALL_GL_API(result = glUnmapNamedBuffer(m_IB));
#endif

            m_prevIB = 0;
            m_isLocked = IZ_FALSE;
        }

        CVertexBufferOGL::s_isLocked = IZ_FALSE;

        return isLocked;
    }
}   // namespace graph
}   // namespace izanagi
