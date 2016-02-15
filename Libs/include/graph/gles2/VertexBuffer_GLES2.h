#if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_GLES2_H__)
#define __IZANAGI_GRAPH_VERTEX_BUFFER_GLES2_H__

#include "graph/GraphDefs.h"
#include "graph/VertexBuffer.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;
    class VtxBufferOperator;

    // 頂点バッファ
    class CVertexBufferGLES2 : public CVertexBuffer
    {
        friend class VtxBufferOperator;
        friend class CGraphicsDeviceGLES2;
        friend class CGraphicsDeviceOGL;

    private:
        // インスタンス作成
        static CVertexBufferGLES2* CreateVertexBuffer(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            IZ_UINT stride,
            IZ_UINT vtxNum,
            E_GRAPH_RSC_USAGE usage);

    protected:
        inline CVertexBufferGLES2();
        virtual inline ~CVertexBufferGLES2();

    protected:
        // 本体作成
        IZ_BOOL CreateBody(
            CGraphicsDevice* device,
            IZ_UINT stride,
            IZ_UINT vtxNum,
            E_GRAPH_RSC_USAGE usage);

        void Initialize(CGraphicsDevice* device);

    public:
        // ロック
        virtual IZ_BOOL Lock(
            CGraphicsDevice* device,
            IZ_UINT offset,
            IZ_UINT size,
            void** data,
            IZ_BOOL isReadOnly,
            IZ_BOOL isDiscard = IZ_FALSE);

        // アンロック
        virtual IZ_BOOL Unlock(CGraphicsDevice* device);

    public:
        virtual IZ_BOOL IsPrepared() const;

        virtual IZ_BOOL Disable();

        virtual IZ_BOOL Restore();

    private:
        GLuint GetRawInterface() { return m_VB; }

    protected:
        CGraphicsDeviceGLES2* m_Device;

        // 本体
        GLuint m_VB;

        IZ_BOOL m_IsInitialized;

        size_t m_Size;

        IZ_UINT m_AllocSize;

        IZ_UINT m_LockOffset;
        IZ_UINT m_LockSize;
        void* m_TemporaryData;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_GLES2_H__)
