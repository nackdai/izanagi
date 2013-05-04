#if !defined(__IZANAGI_GRAPH_INDEX_BUFFER_DX9_H__)
#define __IZANAGI_GRAPH_INDEX_BUFFER_DX9_H__

#include "izD3DDefs.h"
#include "graph/IndexBuffer.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceDX9;

    // インデックスバッファ
    class CIndexBufferDX9 : public CIndexBuffer
    {
        friend class CGraphicsDeviceDX9;

    private:
        // インスタンス作成
        static CIndexBufferDX9* CreateIndexBuffer(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            IZ_UINT idxNum,
            E_GRAPH_INDEX_BUFFER_FMT fmt,
            E_GRAPH_RSC_USAGE usage);

    private:
        inline CIndexBufferDX9();
        virtual inline ~CIndexBufferDX9();

    private:
        // 本体作成
        IZ_BOOL CreateBody(
            CGraphicsDeviceDX9* device,
            IZ_UINT idxNum,
            E_GRAPH_INDEX_BUFFER_FMT fmt,
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
        // 動的リソースかどうか
        IZ_BOOL IsDynamic() const
        {
            return (m_CreateType == E_GRAPH_RSC_USAGE_DYNAMIC);
        }

    public:
        D3D_IB* GetRawInterface() { return m_IB; }

    private:
        CGraphicsDeviceDX9* m_Device;

        // 本体
        D3D_IB* m_IB;

        CIndexBufferDX9* m_Next;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_INDEX_BUFFER_DX9_H__)
