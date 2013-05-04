#include "graph/gles2/VertexBuffer_GLES2.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CVertexBufferGLES2* CVertexBufferGLES2::CreateVertexBuffer(
        CGraphicsDeviceGLES2* device,
        IMemoryAllocator* allocator,
        IZ_UINT stride,
        IZ_UINT vtxNum,
        E_GRAPH_RSC_USAGE usage)
    {
        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CVertexBufferGLES2* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CVertexBufferGLES2));
        if (!(result = (buf != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf)CVertexBufferGLES2;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;
            SAFE_REPLACE(instance->m_Device, device);
        }

        // 本体作成
        result = instance->CreateBody(
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

    // コンストラクタ
    CVertexBufferGLES2::CVertexBufferGLES2()
    {
        m_Device = IZ_NULL;

        m_VB = 0;

        m_IsInitialized = IZ_FALSE;

        m_Size = 0;

        m_LockOffset = 0;
        m_LockSize = 0;
        m_TemporaryData = IZ_NULL;
    }

    // デストラクタ
    CVertexBufferGLES2::~CVertexBufferGLES2()
    {
        SAFE_RELEASE(m_Device);

        ::glDeleteBuffers(1, &m_VB);
        
        FREE(m_Allocator, m_TemporaryData);
    }

    // 本体作成
    IZ_BOOL CVertexBufferGLES2::CreateBody(
        IZ_UINT stride,
        IZ_UINT vtxNum,
        E_GRAPH_RSC_USAGE usage)
    {
        ::glGenBuffers(1, &m_VB);
        VRETURN(m_VB > 0);

        IZ_BOOL ret = (m_VB > 0);

        ::glBindBuffer(
            GL_ARRAY_BUFFER,
            m_VB);

        GLsizeiptr size = vtxNum * stride;

        m_Stride = stride;
        m_VtxNum = vtxNum;

        m_Size = size;
    
        m_CreateType = usage;

        return ret;
    }

    void CVertexBufferGLES2::Initialize()
    {
        if (!m_IsInitialized) {
            GLenum glUsage = (m_CreateType == E_GRAPH_RSC_USAGE_STATIC
                ? GL_STATIC_DRAW
                : GL_DYNAMIC_DRAW);

            ::glBufferData(
                GL_ARRAY_BUFFER,
                m_Size,
                NULL,
                glUsage);

            m_IsInitialized = IZ_TRUE;
        }
    }

    // ロック
    IZ_BOOL CVertexBufferGLES2::Lock(
        IZ_UINT offset,
        IZ_UINT size,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        // NOTE
        // isReadOnly, isDiscard は無効

        VRETURN(size + offset <= m_Size);

        IZ_ASSERT(m_VB > 0);

        size = (size == 0 ? m_Size : size);

        if (m_TemporaryData == IZ_NULL) {
            m_TemporaryData = ALLOC(m_Allocator, size);
        }
        else if (size > m_LockSize) {
            m_TemporaryData = REALLOC(m_Allocator, m_TemporaryData, size);
        }

        VRETURN(m_TemporaryData != IZ_NULL);

        *data = m_TemporaryData;

        m_LockOffset = offset;
        m_LockSize = size;

        return IZ_TRUE;
    }

    /**
    * アンロック
    */
    IZ_BOOL CVertexBufferGLES2::Unlock()
    {
        IZ_ASSERT(m_VB > 0);
    
        IZ_BOOL isLocked = (m_LockSize > 0);

        if (isLocked) {
            CVertexBuffer* curVB = m_Device->GetRenderState().curVB;

            if (curVB != this) {
                ::glBindBuffer(GL_ARRAY_BUFFER, m_VB);

                // もしかしたら
                Initialize();
            }

            ::glBufferSubData(
                GL_ARRAY_BUFFER,
                m_LockOffset,
                m_LockSize,
                m_TemporaryData);

            // 元に戻す
            if (curVB != this) {
                ::glBindBuffer(
                    GL_ARRAY_BUFFER,
                    ((CVertexBufferGLES2*)curVB)->m_VB);
            }

            m_LockOffset = 0;
            m_LockSize = 0;
        }

        if (!IsDynamic()) {
            FREE(m_Allocator, m_TemporaryData);
            m_TemporaryData = IZ_NULL;
        }

        return isLocked;
    }

    IZ_BOOL CVertexBufferGLES2::IsPrepared() const
    {
        return (m_VB != 0);
    }

    IZ_BOOL CVertexBufferGLES2::Disable()
    {
        ::glDeleteBuffers(1, &m_VB);
        return IZ_TRUE;
    }

    // リセット
    IZ_BOOL CVertexBufferGLES2::Restore()
    {
        // Nothing is done...

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
