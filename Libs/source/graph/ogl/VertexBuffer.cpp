#include "graph/dx9/VertexBuffer.h"

using namespace uranus;

// インスタンス作成
CVertexBuffer* CVertexBuffer::CreateVertexBuffer(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	UN_UINT nStride,
	UN_UINT nVtxNum,
	E_GRAPH_RSC_CREATE_TYPE nCreateType)
{
	UN_ASSERT(pDevice != NULL);

	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CVertexBuffer* pInstance = UN_NULL;

	// メモリ確保
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CVertexBuffer));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
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
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != UN_NULL) {
			pAllocator->Free(pBuf);
		}
	}

	return pInstance;
}

// 本体作成
UN_BOOL CVertexBuffer::CreateBody(
	UN_UINT nStride,
	UN_UINT nVtxNum,
	E_GRAPH_RSC_CREATE_TYPE nCreateType)
{
	UN_ASSERT(m_pDevice != UN_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	UN_DWORD nUsage = (nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC
						? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY
						: 0);

	D3DPOOL nPool = (nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC
						? D3DPOOL_DEFAULT
						: D3DPOOL_MANAGED);

	// 本体作成
	HRESULT hr = pD3DDev->CreateVertexBuffer(
					nStride * nVtxNum,
					nUsage,
					0,
					nPool,
					&m_pVB,
					UN_NULL);

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	m_nStride = nStride;
	m_nVtxNum = nVtxNum;
	
	m_nCreateType = nCreateType;

	return ret;
}

// ロック
UN_BOOL CVertexBuffer::Lock(
	UN_UINT nOffset,
	UN_UINT nSize,
	void** data,
	UN_BOOL bIsReadOnly,
	UN_BOOL bIsDiscard/*= UN_FALSE*/)
{
	// NOTE
	// 高速化のために IsDynamic のときは WRITEONLYで
	// 作成しているので、READONLY は不可

	UN_ASSERT(m_pVB != UN_NULL);

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

	UN_BOOL ret = SUCCEEDED(hr);

	return ret;
}

/**
* アンロック
*/
UN_BOOL CVertexBuffer::Unlock()
{
	UN_ASSERT(m_pVB != UN_NULL);
	
	HRESULT hr = m_pVB->Unlock();
	UN_BOOL ret = SUCCEEDED(hr);

	return ret;
}

// リセット
UN_BOOL CVertexBuffer::Reset()
{
	UN_BOOL ret = UN_TRUE;

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
