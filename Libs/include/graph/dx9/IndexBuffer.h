#if !defined(__URANUS_GRAPH_INDEX_BUFFER_H__)
#define __URANUS_GRAPH_INDEX_BUFFER_H__

#include "graph/GraphDefs.h"
#include "GraphicsDevice.h"
#include "std/StdObject.h"

namespace uranus {
	class IMemoryAllocator;

	// インデックスバッファ
	class CIndexBuffer : public CObject {
		friend class CGraphicsDevice;
		friend class C2DRenderer;

	private:
		// インスタンス作成
		static CIndexBuffer* CreateIndexBuffer(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT fmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType);

	private:
		inline CIndexBuffer();
		inline ~CIndexBuffer();

		CIndexBuffer(const CIndexBuffer& rhs);
		const CIndexBuffer& operator=(const CIndexBuffer& rhs);

	private:
		// 解放
		inline void InternalRelease();

		inline void ReleaseResource();

		// 本体作成
		UN_BOOL CreateBody(
			UN_UINT nIdxNum,
			E_GRAPH_INDEX_BUFFER_FMT nFmt,
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
		// インデックスサイズ取得
		inline UN_UINT GetStride() const;

		// インデックス数取得
		inline UN_UINT GetIdxNum() const;

		// バッファサイズ取得
		inline UN_UINT GetSize() const;

		// フォーマット取得
		inline E_GRAPH_INDEX_BUFFER_FMT GetFormat() const;

	private:
		// 動的リソースかどうか
		UN_BOOL IsDynamic() const { return (m_nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC); }

	public:
		D3D_IB* GetRawInterface() { return m_pIB; }

	private:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_pAllocator;

		// 本体
		D3D_IB* m_pIB;

		UN_UINT m_nIdxNum;				// インデックス数
		E_GRAPH_INDEX_BUFFER_FMT m_Fmt;	// フォーマット

		E_GRAPH_RSC_CREATE_TYPE m_nCreateType;

		CIndexBuffer* m_pNext;
	};

	// inline *************************************

	// コンストラクタ
	CIndexBuffer::CIndexBuffer()
	{
		m_pDevice = UN_NULL;
		m_pAllocator = UN_NULL;

		m_pIB = UN_NULL;

		m_nIdxNum = 0;
		m_Fmt = E_GRAPH_INDEX_BUFFER_FMT_FORCE_INT32;
		
		m_nCreateType = E_GRAPH_RSC_CREATE_TYPE_STATIC;
	}

	// デストラクタ
	CIndexBuffer::~CIndexBuffer()
	{
		m_pDevice->RemoveIndexBuffer(this);

		SAFE_RELEASE(m_pIB);
		SAFE_RELEASE(m_pDevice);
	}

	// 解放
	void CIndexBuffer::InternalRelease()
	{
		delete this;

		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	void CIndexBuffer::ReleaseResource()
	{
		SAFE_RELEASE(m_pIB);
	}

	// インデックスサイズ取得
	UN_UINT CIndexBuffer::GetStride() const
	{
		UN_UINT ret = (m_Fmt == E_GRAPH_INDEX_BUFFER_FMT_INDEX16
						? sizeof(UN_UINT16) 
						: sizeof(UN_UINT32));
		return ret;
	}

	// インデックス数取得
	UN_UINT CIndexBuffer::GetIdxNum() const
	{
		return m_nIdxNum;
	}

	// バッファサイズ取得
	UN_UINT CIndexBuffer::GetSize() const
	{
		return m_nIdxNum * GetStride();
	}

	// フォーマット取得
	E_GRAPH_INDEX_BUFFER_FMT CIndexBuffer::GetFormat() const
	{
		return m_Fmt;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_INDEX_BUFFER_H__)
