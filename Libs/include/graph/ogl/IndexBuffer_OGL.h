#if !defined(__IZANAGI_GRAPH_INDEX_BUFFER_OGL_H__)
#define __IZANAGI_GRAPH_INDEX_BUFFER_OGL_H__

#include "graph/gles2/IndexBuffer_GLES2.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;

    // 頂点バッファ
    class CIndexBufferOGL : public CIndexBufferGLES2
    {
        friend class CGraphicsDeviceOGL;

    private:
        // インスタンス作成
        static CIndexBufferOGL* CreateIndexBuffer(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            IZ_UINT idxNum,
            E_GRAPH_INDEX_BUFFER_FMT fmt,
            E_GRAPH_RSC_USAGE usage);

    private:
        CIndexBufferOGL() {}
        virtual ~CIndexBufferOGL() {}

    public:
        void Initialize(CGraphicsDevice* device)
        {
            CIndexBufferGLES2::Initialize(device);
        }

        // ロック
        virtual IZ_BOOL Lock(
            CGraphicsDevice* device,
            IZ_UINT offset,
            IZ_UINT size,
            void** data,
            IZ_BOOL isReadOnly,
            IZ_BOOL isDiscard = IZ_FALSE) override;

        // アンロック
        virtual IZ_BOOL Unlock(CGraphicsDevice* device) override;

        GLuint GetRawInterface()
        {
            return m_IB;
        }

    private:
        GLuint m_prevIB{ 0 };
        IZ_BOOL m_isLocked{ IZ_FALSE };
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_INDEX_BUFFER_OGL_H__)
