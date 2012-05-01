#if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_H__)
#define __IZANAGI_GRAPH_VERTEX_BUFFER_H__

#include "graph/GraphDefs.h"
#include "GraphicsDevice.h"
#include "std/StdObject.h"

namespace izanagi {
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
			IZ_UINT nStride,
			IZ_UINT nVtxNum,
			E_GRAPH_RSC_TYPE nCreateType);

	private:
		inline CVertexBuffer();
		inline ~CVertexBuffer();

		CVertexBuffer(const CVertexBuffer& rhs);
		const CVertexBuffer& operator=(const CVertexBuffer& rhs);

	private:
		// 解放
		inline void InternalRelease();

		inline void ReleaseResource();

		// 本体作成
		IZ_BOOL CreateBody(
			IZ_UINT nStride,
			IZ_UINT nVtxNum,
			E_GRAPH_RSC_TYPE nCreateType);

	public:
		// ロック
		IZ_BOOL Lock(
			IZ_UINT nOffset,
			IZ_UINT nSize,
			void** data,
			IZ_BOOL bIsReadOnly,
			IZ_BOOL bIsDiscard = IZ_FALSE);

		// アンロック
		IZ_BOOL Unlock();

	private:
		// リセット
		IZ_BOOL Reset();

	public:
		// 頂点サイズ取得
		inline IZ_UINT GetStride() const;

		// 頂点数取得
		inline IZ_UINT GetVtxNum() const;

		// サイズ取得
		inline IZ_UINT GetSize() const;

	private:
		// 動的リソースかどうか
		IZ_BOOL IsDynamic() const { return (m_nCreateType == E_GRAPH_RSC_TYPE_DYNAMIC); }

	public:
		D3D_VB* GetRawInterface() { return m_pVB; }

	private:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_Allocator;

		// 本体
		D3D_VB* m_pVB;

		IZ_UINT m_nStride;	// 頂点サイズ
		IZ_UINT m_nVtxNum;	// 頂点数

		E_GRAPH_RSC_TYPE m_nCreateType;

		CVertexBuffer* m_pNext;
	};

	// inline *************************************

	// コンストラクタ
	CVertexBuffer::CVertexBuffer()
	{
		m_pDevice = IZ_NULL;
		m_Allocator = IZ_NULL;

		m_pVB = IZ_NULL;

		m_nStride = 0;
		m_nVtxNum = 0;
		
		m_nCreateType = E_GRAPH_RSC_TYPE_STATIC;
	}

	// デストラクタ
	CVertexBuffer::~CVertexBuffer()
	{
		m_pDevice->RemoveVertexBuffer(this);

		SAFE_RELEASE(m_pVB);
		SAFE_RELEASE(m_pDevice);
	}

	// 解放
	void CVertexBuffer::InternalRelease()
	{
		delete this;

		if (m_Allocator != IZ_NULL) {
			m_Allocator->Free(this);
		}
	}

	void CVertexBuffer::ReleaseResource()
	{
		SAFE_RELEASE(m_pVB);
	}

	// 頂点サイズ取得
	IZ_UINT CVertexBuffer::GetStride() const
	{
		return m_nStride;
	}

	// 頂点数取得
	IZ_UINT CVertexBuffer::GetVtxNum() const
	{
		return m_nVtxNum;
	}

	// サイズ取得
	IZ_UINT CVertexBuffer::GetSize() const
	{
		return m_nVtxNum * m_nStride;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_VERTEX_BUFFER_H__)
