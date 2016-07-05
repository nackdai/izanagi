#if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_H__)
#define __IZANAGI_GRAPH_VERTEX_BUFFER_H__

#include "graph/BufferBase.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    /** 頂点バッファ
     */
    class CVertexBuffer : public CBufferBase
    {
        friend class C2DRenderer;

    protected:
        CVertexBuffer()
        {
            m_Allocator = IZ_NULL;

            m_Stride = 0;
            m_VtxNum = 0;
            m_CreateType = E_GRAPH_RSC_USAGE_STATIC;
        }

        virtual ~CVertexBuffer() {};

    public:
        // 頂点サイズ取得
        IZ_UINT GetStride() const
        {
            return m_Stride;
        }

        // 頂点数取得
        IZ_UINT GetVtxNum() const
        {
            return m_VtxNum;
        }

        // サイズ取得
        IZ_UINT GetSize() const
        {
            return m_Stride * m_VtxNum;
        }

        /** 動的リソースかどうか
         */
        IZ_BOOL IsDynamic() const
        {
            return (m_CreateType == E_GRAPH_RSC_USAGE_DYNAMIC);
        }

        virtual void overrideNativeResource(void* rsc)
        {
            // Usualy unsupoort.
            IZ_ASSERT(IZ_FALSE);
        }

        virtual void* getRawInterface()
        {
            // Usualy unsupoort.
            IZ_ASSERT(IZ_FALSE);
            return nullptr;
        }

    protected:
        IZ_UINT m_Stride;   // 頂点サイズ
        IZ_UINT m_VtxNum;   // 頂点数

        E_GRAPH_RSC_USAGE m_CreateType;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_H__)
