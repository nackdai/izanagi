#include "graph/ogl/VertexDeclaration.h"
#include "graph/ogl/OGLExtFuncProxy.h"

using namespace uranus;

// インスタンス作成
CVertexDeclaration* CVertexDeclaration::CreateVertexDeclaration(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const SVertexElement* pElem,
	UN_UINT nElemNum)
{
	size_t nSize = sizeof(CVertexDeclaration);
	nSize += sizeof(SVertexElement) * nElemNum;

	UN_UINT8* pBuf = reinterpret_cast<UN_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != UN_NULL);

	CVertexDeclaration* pInstance = new(pBuf) CVertexDeclaration;
	{
		pBuf +=  sizeof(CVertexDeclaration);

		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->m_pVtxElem = reinterpret_cast<SVertexElement*>(pBuf);
		pInstance->m_nElemNum = nElemNum;
	}

	memcpy(pInstance->m_pVtxElem, pElem, sizeof(SVertexElement) * nElemNum);

	return pInstance;
}

void CVertexDeclaration::Apply(CVertexShader* pVS)
{
	for (UN_UINT i = 0; i < m_nElemNum; i++) {
		const SVertexElement& sElem = m_pVtxElem[i];

		UN_OGL_EXT_PROC(glEnableVertexAttribArray)(sElem.
	}
}
