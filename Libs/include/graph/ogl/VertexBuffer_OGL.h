#if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_OGL_H__)
#define __IZANAGI_GRAPH_VERTEX_BUFFER_OGL_H__

#include <atomic>
#include "graph/gles2/VertexBuffer_GLES2.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;

    // 頂点バッファ
    class CVertexBufferOGL : public CVertexBufferGLES2
    {
        friend class CGraphicsDeviceOGL;
        friend class CIndexBufferOGL;

        // glMapBufferRange - glUnmapBuffer の間は他の glMapBufferRange - glUnmapBuffer を呼べないので、そのチェック用.
        static std::atomic<IZ_BOOL> s_isLocked;

    private:
        // インスタンス作成
        static CVertexBufferOGL* CreateVertexBuffer(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            IZ_UINT stride,
            IZ_UINT vtxNum,
            E_GRAPH_RSC_USAGE usage);

    private:
        CVertexBufferOGL() {}
        virtual ~CVertexBufferOGL() {}

    public:
        void Initialize(CGraphicsDevice* device)
        {
            CVertexBufferGLES2::Initialize(device);
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
            return m_VB;
        }

        virtual void overrideNativeResource(void* rsc, E_GRAPH_OVERRIDE_NATIVE_RSC_BEHAVIOUR behaviour) override;

        virtual void* getRawInterface() override
        {
            return &m_VB;
        }

    private:
        GLuint m_prevVB{ 0 };
        IZ_BOOL m_isLocked{ IZ_FALSE };
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_OGL_H__)
