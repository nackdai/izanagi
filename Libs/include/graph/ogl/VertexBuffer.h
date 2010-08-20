#if !defined(__URANUS_GRAPH_VERTEX_BUFFER_H__)
#define __URANUS_GRAPH_VERTEX_BUFFER_H__

#include "graph/GraphDefs.h"
#include "GraphicsDevice.h"
#include "std/StdObject.h"

namespace uranus {
	class IMemoryAllocator;

	// 頂点バッファ
	class CVertexBuffer : public CObject {
		friend class CGraphicsDevice;
		friend class C2DRenderer;

	private:
		// インスタンス作成
		static CVertexBuffer* CreateVertexBuffer(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_UINT nStride,
			UN_UINT nVtxNum,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

	private:
		CVertexBuffer();
		~CVertexBuffer();

		CVertexBuffer(const CVertexBuffer& rhs);
		const CVertexBuffer& operator=(const CVertexBuffer& rhs);

	private:
		// 解放
		inline void InternalRelease();

		inline void ReleaseResource();

		// 本体作成
		UN_BOOL CreateBody(
			UN_UINT nStride,
			UN_UINT nVtxNum,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

	public:
		// ロック
		UN_BOOL Lock(
			UN_UINT nOffset,
			UN_UINT nSize,
			void** data,
			UN_BOOL bIsReadOnly,
			UN_BOOL bIsDiscard = UN_FALSE);

		// アンロック
		UN_BOOL Unlock();

	private:
		// リセット
		UN_BOOL Reset();

	public:
		// 頂点サイズ取得
		inline UN_UINT GetStride() const;

		// 頂点数取得
		inline UN_UINT GetVtxNum() const;

		// サイズ取得
		inline UN_UINT GetSize() const;

	private:
		// 動的リソースかどうか
		UN_BOOL IsDynamic() const { return (m_nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC); }

	public:
		UN_UINT GetRawInterface() { return m_nVBO; }

	private:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_pAllocator;

		// 本体
		UN_UINT m_nVBO;

		UN_UINT m_nStride;	// 頂点サイズ
		UN_UINT m_nVtxNum;	// 頂点数

		// ロック情報
		struct {
			void* data;
			UN_UINT offset;
			UN_UINT size;

			void Clear()
			{
				data = UN_NULL;
				offset = 0;
				size = 0;
			}

			UN_BOOL IsLock() const { return (data != UN_NULL); }
		} m_LockInfo;

		E_GRAPH_RSC_CREATE_TYPE m_nCreateType;
	};

	// inline *************************************

	// 解放
	void CVertexBuffer::InternalRelease()
	{
		delete this;

		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	void CVertexBuffer::ReleaseResource()
	{
		// Nothing is done.
		UN_ASSERT(UN_FALSE);
	}

	// 頂点サイズ取得
	UN_UINT CVertexBuffer::GetStride() const
	{
		return m_nStride;
	}

	// 頂点数取得
	UN_UINT CVertexBuffer::GetVtxNum() const
	{
		return m_nVtxNum;
	}

	// サイズ取得
	UN_UINT CVertexBuffer::GetSize() const
	{
		return m_nVtxNum * m_nStride;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_VERTEX_BUFFER_H__)
