#if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_H__)
#define __IZANAGI_GRAPH_VERTEX_DECLARATION_H__

#include "izStd.h"
#include "izD3DDefs.h"
#include "GraphicsDevice.h"

namespace izanagi {
	/**
	*/
	struct SVertexElement {
		IZ_WORD Stream;					// Stream index
		IZ_WORD Offset;					// Offset in the stream in bytes
		E_GRAPH_VTX_DECL_TYPE Type;		// Data type
		E_GRAPH_VTX_DECL_USAGE Usage;	// Semantics
		IZ_BYTE UsageIndex;				// Semantic index
		IZ_BYTE padding[3];
	};

	/**
	* 頂点宣言
	*/
	class CVertexDeclaration : public CObject {
		friend class CGraphicsDevice;

	private:
		// インスタンス作成
		static CVertexDeclaration* CreateVertexDeclaration(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const D3D_VTX_ELEMENT* pElem);

		// インスタンス作成
		static CVertexDeclaration* CreateVertexDeclaration(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const SVertexElement* pElem,
			IZ_UINT nElemNum);

	protected:
		inline CVertexDeclaration();
		inline ~CVertexDeclaration();

		CVertexDeclaration(const CVertexDeclaration& rhs);
		const CVertexDeclaration& operator=(const CVertexDeclaration& rhs);

	protected:
		// 解放
		inline void InternalRelease();

	public:
		inline D3D_VD* GetRawInterface();

	protected:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_Allocator;

		// 本体
		D3D_VD* m_pVD;
	};

	// inline **********************************

	// コンストラクタ
	CVertexDeclaration::CVertexDeclaration()
	{
		m_pDevice = IZ_NULL;
		m_Allocator = IZ_NULL;
		m_pVD = IZ_NULL;
	}

	// デストラクタ
	CVertexDeclaration::~CVertexDeclaration()
	{
		SAFE_RELEASE(m_pDevice);
		SAFE_RELEASE(m_pVD);
	}

	// 解放
	void CVertexDeclaration::InternalRelease()
	{
		delete this;
		if (m_Allocator != IZ_NULL) {
			m_Allocator->Free(this);
		}
	}

	D3D_VD* CVertexDeclaration::GetRawInterface()
	{
		return m_pVD;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_H__)
