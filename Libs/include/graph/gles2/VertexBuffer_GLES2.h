#if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_GLES2_H__)
#define __IZANAGI_GRAPH_VERTEX_BUFFER_GLES2_H__

#include "izGLES2Defs.h"
#include "graph/VertexBuffer.h"

namespace izanagi
{
namespace graph
{
    // 頂点バッファ
    class CVertexBufferGLES2 : public CVertexBuffer
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // インスタンス作成
        static CVertexBufferGLES2* CreateVertexBuffer(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            IZ_UINT stride,
            IZ_UINT vtxNum,
            E_GRAPH_RSC_USAGE usage);

    private:
        inline CVertexBufferGLES2();
        virtual inline ~CVertexBufferGLES2();

    private:
        // 本体作成
        IZ_BOOL CreateBody(
            IZ_UINT stride,
            IZ_UINT vtxNum,
            E_GRAPH_RSC_USAGE usage);

    public:
        // ロック
        virtual IZ_BOOL Lock(
            IZ_UINT offset,
            IZ_UINT size,
            void** data,
            IZ_BOOL isReadOnly,
            IZ_BOOL isDiscard = IZ_FALSE);

        // アンロック
        virtual IZ_BOOL Unlock();

    public:
        virtual IZ_BOOL IsPrepared() const;

        virtual IZ_BOOL Disable();

        virtual IZ_BOOL Restore();

    private:
        CGraphicsDeviceGLES2* m_Device;

        // 本体
        GLuint m_VB;

        size_t m_Size;

        IZ_UINT m_LockOffset;
        IZ_UINT m_LockSize;
        void* m_TemporaryData;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_GLES2_H__)
