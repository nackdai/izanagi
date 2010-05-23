#if !defined(__URANUS_GRAPH_VERTEX_DECLARATION_H__)
#define __URANUS_GRAPH_VERTEX_DECLARATION_H__

#include "unStd.h"
#include "unD3DDefs.h"
#include "GraphicsDevice.h"

namespace uranus {
	/**
	*/
	struct SVertexElement {
		UN_WORD Stream;					// Stream index
		UN_WORD Offset;					// Offset in the stream in bytes
		E_GRAPH_VTX_DECL_TYPE Type;		// Data type
		E_GRAPH_VTX_DECL_USAGE Usage;	// Semantics
		UN_BYTE UsageIndex;				// Semantic index
		UN_BYTE padding;
	};

	/**
	* ���_�錾
	*/
	class CVertexDeclaration : public CObject {
		friend class CGraphicsDevice;

	private:
		// �C���X�^���X�쐬
		static CVertexDeclaration* CreateVertexDeclaration(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const D3D_VTX_ELEMENT* pElem);

		// �C���X�^���X�쐬
		static CVertexDeclaration* CreateVertexDeclaration(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const SVertexElement* pElem,
			UN_UINT nElemNum);

	protected:
		inline CVertexDeclaration();
		inline ~CVertexDeclaration();

		CVertexDeclaration(const CVertexDeclaration& rhs);
		const CVertexDeclaration& operator=(const CVertexDeclaration& rhs);

	protected:
		// ���
		inline void InternalRelease();

	public:
		inline D3D_VD* GetRawInterface();

	protected:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_pAllocator;

		// �{��
		D3D_VD* m_pVD;
	};

	// inline **********************************

	// �R���X�g���N�^
	CVertexDeclaration::CVertexDeclaration()
	{
		m_pDevice = UN_NULL;
		m_pAllocator = UN_NULL;
		m_pVD = UN_NULL;
	}

	// �f�X�g���N�^
	CVertexDeclaration::~CVertexDeclaration()
	{
		SAFE_RELEASE(m_pDevice);
		SAFE_RELEASE(m_pVD);
	}

	// ���
	void CVertexDeclaration::InternalRelease()
	{
		delete this;
		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	D3D_VD* CVertexDeclaration::GetRawInterface()
	{
		return m_pVD;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_VERTEX_DECLARATION_H__)
