#include "graph/dx9/VertexDeclaration.h"

using namespace izanagi;

// インスタンス作成
CVertexDeclaration* CVertexDeclaration::CreateVertexDeclaration(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const D3D_VTX_ELEMENT* pElem)
{
	CVertexDeclaration* pInstance = IZ_NULL;
	IZ_UINT8* pBuf = IZ_NULL;
	IZ_BOOL result = IZ_FALSE;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CVertexDeclaration));
	result = (pBuf != IZ_NULL);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CVertexDeclaration;
	{
		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;
	}

	// 頂点宣言本体作成
	D3D_DEVICE* pRawDevice = pDevice->GetRawInterface();
	HRESULT hr = pRawDevice->CreateVertexDeclaration(
					pElem,
					&pInstance->m_pVD);
	result = SUCCEEDED(hr);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	SAFE_REPLACE(pInstance->m_pDevice, pDevice);

__EXIT__:
	if (!result) {
		if (pInstance != IZ_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != IZ_NULL) {
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
	IZ_UINT nElemNum)
{
	static const IZ_UINT ElemBufferSize = 32;
	static D3D_VTX_ELEMENT ElemBuffer[ElemBufferSize];

	IZ_ASSERT(nElemNum < ElemBufferSize - 1);

	IZ_UINT nPos = 0;

	for (nPos = 0; nPos < nElemNum; ++nPos) {
		ElemBuffer[nPos].Stream = pElem[nPos].Stream;
		ElemBuffer[nPos].Offset = pElem[nPos].Offset;
		ElemBuffer[nPos].Type = IZ_GET_TARGET_VTX_DECL_TYPE(pElem[nPos].Type);
		ElemBuffer[nPos].Method = 0;
		ElemBuffer[nPos].Usage = IZ_GET_TARGET_VTX_DECL_USAGE(pElem[nPos].Usage);
		ElemBuffer[nPos].UsageIndex = pElem[nPos].UsageIndex;
	}

	FILL_ZERO(&ElemBuffer[nPos], sizeof(ElemBuffer[nPos]));
	ElemBuffer[nPos].Stream = 0xff;
	ElemBuffer[nPos].Type = D3DDECLTYPE_UNUSED;

	CVertexDeclaration* ret = CreateVertexDeclaration(
								pDevice,
								pAllocator,
								ElemBuffer);
	IZ_ASSERT(ret != IZ_NULL);

	return ret;
}
