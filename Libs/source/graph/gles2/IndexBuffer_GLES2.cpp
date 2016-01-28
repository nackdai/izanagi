#include "graph/gles2/IndexBuffer_GLES2.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/ParamValueConverter.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CIndexBufferGLES2* CIndexBufferGLES2::CreateIndexBuffer(
        CGraphicsDeviceGLES2* device,
        IMemoryAllocator* allocator,
        IZ_UINT idxNum,
        E_GRAPH_INDEX_BUFFER_FMT fmt,
        E_GRAPH_RSC_USAGE usage)
    {
        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* but = IZ_NULL;
        CIndexBufferGLES2* instance = IZ_NULL;

        // メモリ確保
        but = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CIndexBufferGLES2));
        if (!(result = (but != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (but)CIndexBufferGLES2;
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
            else if (but != IZ_NULL) {
                allocator->Free(but);
            }
        }

        return instance;
    }

    // コンストラクタ
    CIndexBufferGLES2::CIndexBufferGLES2()
    {
        m_Device = IZ_NULL;

        m_IB = 0;

        m_IsInitialized = IZ_FALSE;

        m_Size = 0;

        m_AllocSize = 0;

        m_LockOffset = 0;
        m_LockSize = 0;
        m_TemporaryData = IZ_NULL;
    }

    // デストラクタ
    CIndexBufferGLES2::~CIndexBufferGLES2()
    {
        SAFE_RELEASE(m_Device);

        CALL_GL_API(::glDeleteBuffers(1, &m_IB));
        
        FREE(m_Allocator, m_TemporaryData);
    }

    // 本体作成
    IZ_BOOL CIndexBufferGLES2::CreateBody(
        IZ_UINT idxNum,
        E_GRAPH_INDEX_BUFFER_FMT fmt,
        E_GRAPH_RSC_USAGE usage)
    {
        CALL_GL_API(::glGenBuffers(1, &m_IB));
        VRETURN(m_IB > 0);

        IZ_BOOL ret = (m_IB > 0);

        CALL_GL_API(
            ::glBindBuffer(
                GL_ELEMENT_ARRAY_BUFFER,
                m_IB));

        GLsizeiptr size = idxNum;
        size *= (fmt == E_GRAPH_INDEX_BUFFER_FMT_INDEX16 ? sizeof(GLushort) : sizeof(GLuint));

        m_IdxNum = idxNum;
        m_Fmt = fmt;
        m_Size = size;
    
        m_CreateType = usage;

        return ret;
    }

    void CIndexBufferGLES2::Initialize()
    {
        if (!m_IsInitialized) {
            GLenum glUsage = (m_CreateType == E_GRAPH_RSC_USAGE_STATIC
                ? GL_STATIC_DRAW
                : GL_DYNAMIC_DRAW);

            CALL_GL_API(
                ::glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER,
                    m_Size,
                    NULL,
                    glUsage));

            m_IsInitialized = IZ_TRUE;
        }
    }

    // ロック
    IZ_BOOL CIndexBufferGLES2::Lock(
        IZ_UINT offset,
        IZ_UINT size,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        // NOTE
        // isReadOnly, isDiscard は無効

        VRETURN(size + offset <= m_Size);

        IZ_ASSERT(m_IB > 0);

        size = (size == 0 ? m_Size : size);

        if (m_TemporaryData == IZ_NULL) {
            m_TemporaryData = ALLOC(m_Allocator, size);
            m_AllocSize = size;
        }
        else if (size > m_AllocSize) {
            m_TemporaryData = REALLOC(m_Allocator, m_TemporaryData, size);
            m_AllocSize = size;
        }

        VRETURN(m_TemporaryData != IZ_NULL);

        IZ_UINT8* tmp = reinterpret_cast<IZ_UINT8*>(m_TemporaryData);
        tmp += offset;

        *data = tmp;

        m_LockOffset = offset;
        m_LockSize = size;

        if (m_LockOffset > 0
            && m_LockSize + m_LockOffset > m_Size)
        {
            m_LockSize = m_Size - m_LockOffset;
        }

        return IZ_TRUE;
    }

    /**
    * アンロック
    */
    IZ_BOOL CIndexBufferGLES2::Unlock()
    {
        IZ_ASSERT(m_IB > 0);
    
        IZ_BOOL isLocked = (m_LockSize > 0);

        if (isLocked) {
            CIndexBuffer* curIB = m_Device->GetRenderState().curIB;

            if (curIB != this) {
                CALL_GL_API(::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB));

                // もしかしたら
                Initialize();
            }

            IZ_UINT8* tmp = reinterpret_cast<IZ_UINT8*>(m_TemporaryData);
            tmp += m_LockOffset;

            CALL_GL_API(
                ::glBufferSubData(
                    GL_ELEMENT_ARRAY_BUFFER,
                    m_LockOffset,
                    m_LockSize,
                    tmp));

            // 元に戻す
            if (curIB != this) {
                if (curIB == IZ_NULL) {
                    CALL_GL_API(::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
                }
                else {
                    CALL_GL_API(
                        ::glBindBuffer(
                            GL_ELEMENT_ARRAY_BUFFER,
                            ((CIndexBufferGLES2*)curIB)->m_IB));
                }
            }

            m_LockOffset = 0;
            m_LockSize = 0;
        }

        if (!IsDynamic()) {
            FREE(m_Allocator, m_TemporaryData);
            m_TemporaryData = IZ_NULL;
            m_AllocSize = 0;
        }

        return isLocked;
    }

    IZ_BOOL CIndexBufferGLES2::IsPrepared() const
    {
        return (m_IB != 0);
    }

    IZ_BOOL CIndexBufferGLES2::Disable()
    {
        CALL_GL_API(::glDeleteBuffers(1, &m_IB));
        return IZ_TRUE;
    }

    // リセット
    IZ_BOOL CIndexBufferGLES2::Restore()
    {
        // Nothing is done...

        return IZ_TRUE;
    }

}   // namespace graph
}   // namespace izanagi
