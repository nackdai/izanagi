#include "graph/dx9/VertexDeclaration.h"

using namespace uranus;

// インスタンス作成
CVertexDeclaration* CVertexDeclaration::CreateVertexDeclaration(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const D3D_VTX_ELEMENT* pElem)
{
	CVertexDeclaration* pInstance = UN_NULL;
	UN_UINT8* pBuf = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// メモリ確保
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CVertexDeclaration));
	result = (pBuf != UN_NULL);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CVertexDeclaration;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

	// 頂点宣言本体作成
	D3D_DEVICE* pRawDevice = pDevice->GetRawInterface();
	HRESULT hr = pRawDevice->CreateVertexDeclaration(
					pElem,
					&pInstance->m_pVD);
	result = SUCCEEDED(hr);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	SAFE_REPLACE(pInstance->m_pDevice, pDevice);

__EXIT__:
	if (!result) {
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != UN_NULL) {
			pAllocator->Free(pBuf);
		}
	}

	return pInstance;
}

// インスタンス作成
CVertexDeclaration* CVertexDeclaration::CreateVertexDeclaration(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const SVertexElement* pElem,
	UN_UINT nElemNum)
{
	static const UN_UINT ElemBufferSize = 32;
	static D3D_VTX_ELEMENT ElemBuffer[ElemBufferSize];

	UN_ASSERT(nElemNum < ElemBufferSize - 1);

	UN_UINT nPos = 0;

	for (nPos = 0; nPos < nElemNum; nPos++) {
		ElemBuffer[nPos].Stream = pElem[nPos].Stream;
		ElemBuffer[nPos].Offset = pElem[nPos].Offset;
		ElemBuffer[nPos].Type = UN_GET_TARGET_VTX_DECL_TYPE(pElem[nPos].Type);
		ElemBuffer[nPos].Method = 0;
		ElemBuffer[nPos].Usage = UN_GET_TARGET_VTX_DECL_USAGE(pElem[nPos].Usage);
		ElemBuffer[nPos].UsageIndex = pElem[nPos].UsageIndex;
	}

	FILL_ZERO(&ElemBuffer[nPos], sizeof(ElemBuffer[nPos]));
	ElemBuffer[nPos].Stream = 0xff;
	ElemBuffer[nPos].Type = D3DDECLTYPE_UNUSED;

	CVertexDeclaration* ret = CreateVertexDeclaration(
								pDevice,
								pAllocator,
								ElemBuffer);
	UN_ASSERT(ret != UN_NULL);

	return ret;
}
