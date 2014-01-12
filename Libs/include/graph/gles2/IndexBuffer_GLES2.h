#if !defined(__IZANAGI_GRAPH_INDEX_BUFFER_GLES2_H__)
#define __IZANAGI_GRAPH_INDEX_BUFFER_GLES2_H__

#include "graph/GraphDefs.h"
#include "graph/IndexBuffer.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;

    // インデックスバッファ
    class CIndexBufferGLES2 : public CIndexBuffer
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // インスタンス作成
        static CIndexBufferGLES2* CreateIndexBuffer(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            IZ_UINT idxNum,
            E_GRAPH_INDEX_BUFFER_FMT fmt,
            E_GRAPH_RSC_USAGE usage);

    private:
        inline CIndexBufferGLES2();
        virtual inline ~CIndexBufferGLES2();

    private:
        // 本体作成
        IZ_BOOL CreateBody(
            IZ_UINT idxNum,
            E_GRAPH_INDEX_BUFFER_FMT fmt,
            E_GRAPH_RSC_USAGE usage);

        void Initialize();

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

    public:
        GLuint GetRawInterface() { return m_IB; }

    private:
        CGraphicsDeviceGLES2* m_Device;

        // 本体
        GLuint m_IB;

        IZ_BOOL m_IsInitialized;

        size_t m_Size;

        IZ_UINT m_AllocSize;

        IZ_UINT m_LockOffset;
        IZ_UINT m_LockSize;
        void* m_TemporaryData;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_INDEX_BUFFER_GLES2_H__)
