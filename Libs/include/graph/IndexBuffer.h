#if !defined(__IZANAGI_GRAPH_INDEX_BUFFER_H__)
#define __IZANAGI_GRAPH_INDEX_BUFFER_H__

#include "graph/BufferBase.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    /** インデックスバッファ
     */
    class CIndexBuffer : public CBufferBase
    {
        friend class C2DRenderer;

    protected:
        CIndexBuffer()
        {
            m_Allocator = IZ_NULL;

            m_IdxNum = 0;
            m_Fmt = E_GRAPH_INDEX_BUFFER_FMT_FORCE_INT32;
            m_CreateType = E_GRAPH_RSC_TYPE_STATIC;
        }

        virtual ~CIndexBuffer() {};

    public:
        // インデックスサイズ取得
        IZ_UINT GetStride() const
        {
            IZ_UINT ret = (m_Fmt == E_GRAPH_INDEX_BUFFER_FMT_INDEX16
                        ? sizeof(IZ_UINT16) 
                        : sizeof(IZ_UINT32));
            return ret;
        }

        // インデックス数取得
        IZ_UINT GetIdxNum() const
        {
            return m_IdxNum;
        }

        // バッファサイズ取得
        IZ_UINT GetSize() const
        {
            IZ_UINT stride = GetStride();
            return m_IdxNum * stride;
        }

        // フォーマット取得
        E_GRAPH_INDEX_BUFFER_FMT GetFormat() const
        {
            return m_Fmt;
        }

    protected:
        IZ_UINT m_IdxNum;               // インデックス数
        E_GRAPH_INDEX_BUFFER_FMT m_Fmt; // フォーマット

        E_GRAPH_RSC_TYPE m_CreateType;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_INDEX_BUFFER_H__)
