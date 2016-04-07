#include "graph/gles2/VertexBuffer_GLES2.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    class VtxBufferOperator
    {
    public:
        VtxBufferOperator(CGraphicsDevice* device, GLuint targetVB)
        {
            auto vb = (CVertexBufferGLES2*)device->GetRenderState().curVB[0];

            m_prevVB = (vb != IZ_NULL ? vb->m_VB : 0);

            CALL_GL_API(::glBindBuffer(GL_ARRAY_BUFFER, targetVB));
        }
        ~VtxBufferOperator()
        {
            CALL_GL_API(::glBindBuffer(GL_ARRAY_BUFFER, m_prevVB));
        }

    private:
        GLuint m_prevVB{ 0 };
    };

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

    // コンストラクタ
    CVertexBufferGLES2::CVertexBufferGLES2()
    {
        m_Device = IZ_NULL;

        m_VB = 0;

        m_IsInitialized = IZ_FALSE;

        m_Size = 0;

        m_AllocSize = 0;

        m_LockOffset = 0;
        m_LockSize = 0;
        m_TemporaryData = IZ_NULL;
    }

    // デストラクタ
    CVertexBufferGLES2::~CVertexBufferGLES2()
    {
        SAFE_RELEASE(m_Device);

        if (m_VB > 0) {
            CALL_GL_API(::glDeleteBuffers(1, &m_VB));
        }
        
        FREE(m_Allocator, m_TemporaryData);
    }

    // 本体作成
    IZ_BOOL CVertexBufferGLES2::CreateBody(
        CGraphicsDevice* device,
        IZ_UINT stride,
        IZ_UINT vtxNum,
        E_GRAPH_RSC_USAGE usage)
    {
        CALL_GL_API(::glGenBuffers(1, &m_VB));
        VRETURN(m_VB > 0);

        IZ_BOOL ret = (m_VB > 0);

        GLsizeiptr size = vtxNum * stride;

        m_Stride = stride;
        m_VtxNum = vtxNum;

        m_Size = size;
    
        m_CreateType = usage;

        Initialize(device);

        return ret;
    }

    void CVertexBufferGLES2::Initialize(CGraphicsDevice* device)
    {
        if (!m_IsInitialized) {
            VtxBufferOperator vbOp(device, m_VB);

            GLenum glUsage = (m_CreateType == E_GRAPH_RSC_USAGE_STATIC
                ? GL_STATIC_DRAW
                : GL_DYNAMIC_DRAW);

            CALL_GL_API(
                ::glBufferData(
                    GL_ARRAY_BUFFER,
                    m_Size,
                    NULL,
                    glUsage));

            m_IsInitialized = IZ_TRUE;
        }
    }

    // ロック
    IZ_BOOL CVertexBufferGLES2::Lock(
        CGraphicsDevice* device,
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
    IZ_BOOL CVertexBufferGLES2::Unlock(CGraphicsDevice* device)
    {
        IZ_ASSERT(m_VB > 0);
    
        IZ_BOOL isLocked = (m_LockSize > 0);

        if (isLocked) {
            CVertexBuffer* curVB = m_Device->GetRenderState().curVB[0];

            if (curVB != this) {
                // もしかしたら
                Initialize(device);
            }

            VtxBufferOperator vbOp(device, m_VB);

            IZ_UINT8* tmp = reinterpret_cast<IZ_UINT8*>(m_TemporaryData);
            tmp += m_LockOffset;

            CALL_GL_API(
                ::glBufferSubData(
                    GL_ARRAY_BUFFER,
                    m_LockOffset,
                    m_LockSize,
                    tmp));

            // 元に戻す
            if (curVB != this) {
                if (curVB == IZ_NULL) {
                    CALL_GL_API(::glBindBuffer(GL_ARRAY_BUFFER, 0));
                }
                else {
                    CALL_GL_API(
                        ::glBindBuffer(
                            GL_ARRAY_BUFFER,
                            ((CVertexBufferGLES2*)curVB)->m_VB));
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

    IZ_BOOL CVertexBufferGLES2::IsPrepared() const
    {
        return (m_VB != 0);
    }

    IZ_BOOL CVertexBufferGLES2::Disable()
    {
        CALL_GL_API(::glDeleteBuffers(1, &m_VB));
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
