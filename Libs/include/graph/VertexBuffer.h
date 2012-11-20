#if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_H__)
#define __IZANAGI_GRAPH_VERTEX_BUFFER_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
	/** 頂点バッファ
     */
	class CVertexBuffer : public CObject
    {
        friend class C2DRenderer;

	protected:
		CVertexBuffer()
        {
            m_Allocator = IZ_NULL;

		    m_Stride = 0;
		    m_VtxNum = 0;
		    m_CreateType = E_GRAPH_RSC_TYPE_STATIC;
        }

        virtual ~CVertexBuffer() {};

		NO_COPIABLE(CVertexBuffer);

	    IZ_DEFINE_INTERNAL_RELEASE();

	public:
		// ロック
        PURE_VIRTUAL(
		    IZ_BOOL Lock(
			    IZ_UINT offset,
			    IZ_UINT size,
			    void** data,
			    IZ_BOOL isReadOnly,
			    IZ_BOOL isDiscard = IZ_FALSE));

		// アンロック
		PURE_VIRTUAL(IZ_BOOL Unlock());

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

    protected:
        virtual void ReleaseResource() {}
        virtual IZ_BOOL Reset() { return IZ_TRUE; }

	protected:
		IMemoryAllocator* m_Allocator;

		IZ_UINT m_Stride;	// 頂点サイズ
		IZ_UINT m_VtxNum;	// 頂点数

		E_GRAPH_RSC_TYPE m_CreateType;
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_H__)
