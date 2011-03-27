#include "graph/dx9/VertexBuffer.h"

using namespace izanagi;

// インスタンス作成
CVertexBuffer* CVertexBuffer::CreateVertexBuffer(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	IZ_UINT nStride,
	IZ_UINT nVtxNum,
	E_GRAPH_RSC_TYPE nCreateType)
{
	IZ_ASSERT(pDevice != NULL);

	IZ_BOOL result = IZ_TRUE;
	IZ_UINT8* pBuf = IZ_NULL;
	CVertexBuffer* pInstance = IZ_NULL;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CVertexBuffer));
	if (!(result = (pBuf != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CVertexBuffer;
	{
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);
		pInstance->m_pAllocator = pAllocator;
		pInstance->AddRef();
	}

	// 本体作成
	result = pInstance->CreateBody(
				nStride,
				nVtxNum,
				nCreateType);
	if (!result) {
		goto __EXIT__;
	}

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

// 本体作成
IZ_BOOL CVertexBuffer::CreateBody(
	IZ_UINT nStride,
	IZ_UINT nVtxNum,
	E_GRAPH_RSC_TYPE nCreateType)
{
	IZ_ASSERT(m_pDevice != IZ_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	IZ_DWORD nUsage = (nCreateType == E_GRAPH_RSC_TYPE_DYNAMIC
						? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY
						: 0);

	D3DPOOL nPool = (nCreateType == E_GRAPH_RSC_TYPE_DYNAMIC
						? D3DPOOL_DEFAULT
						: D3DPOOL_MANAGED);

	// 本体作成
	HRESULT hr = pD3DDev->CreateVertexBuffer(
					nStride * nVtxNum,
					nUsage,
					0,
					nPool,
					&m_pVB,
					IZ_NULL);

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	m_nStride = nStride;
	m_nVtxNum = nVtxNum;
	
	m_nCreateType = nCreateType;

	return ret;
}

// ロック
IZ_BOOL CVertexBuffer::Lock(
	IZ_UINT nOffset,
	IZ_UINT nSize,
	void** data,
	IZ_BOOL bIsReadOnly,
	IZ_BOOL bIsDiscard/*= IZ_FALSE*/)
{
	// NOTE
	// 高速化のために IsDynamic のときは WRITEONLYで
	// 作成しているので、READONLY は不可

	IZ_ASSERT(m_pVB != IZ_NULL);

	DWORD flag = 0;
	if (IsDynamic()) {
		// READONLY不可
		VRETURN(!bIsReadOnly);
		
		if (bIsDiscard) {
			flag = D3DLOCK_DISCARD;
		}
		else {
			flag = D3DLOCK_NOOVERWRITE;
		}
	}
	else if (bIsReadOnly) {
		flag = D3DLOCK_READONLY;
	}

	HRESULT hr = m_pVB->Lock(
					nOffset,
					nSize,
					data,
					flag);

	IZ_BOOL ret = SUCCEEDED(hr);

	return ret;
}

/**
* アンロック
*/
IZ_BOOL CVertexBuffer::Unlock()
{
	IZ_ASSERT(m_pVB != IZ_NULL);
	
	HRESULT hr = m_pVB->Unlock();
	IZ_BOOL ret = SUCCEEDED(hr);

	return ret;
}

// リセット
IZ_BOOL CVertexBuffer::Reset()
{
	IZ_BOOL ret = IZ_TRUE;

	// NOTE
	// Dynamicでないときは、D3DPOOL_MANAGEDで作成されているので再作成する必要はない

	if (IsDynamic()) {
		SAFE_RELEASE(m_pVB);

		// 本体作成
		ret = CreateBody(
				m_nStride,
				m_nVtxNum,
				m_nCreateType);
	}

	return ret;
}
