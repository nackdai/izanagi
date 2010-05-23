#include "graph/dx9/IndexBuffer.h"
#include "graph/dx9/D3D9ParamValueConverter.h"

using namespace uranus;

// インスタンス作成
CIndexBuffer* CIndexBuffer::CreateIndexBuffer(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	UN_UINT nIdxNum,
	E_GRAPH_INDEX_BUFFER_FMT fmt,
	E_GRAPH_RSC_CREATE_TYPE nCreateType)
{
	UN_ASSERT(pDevice != NULL);

	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CIndexBuffer* pInstance = UN_NULL;

	// メモリ確保
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CIndexBuffer));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CIndexBuffer;
	{
		pInstance->m_pAllocator = pAllocator;
		pInstance->AddRef();
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);
	}

	// 本体作成
	result = pInstance->CreateBody(
				nIdxNum,
				fmt,
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
UN_BOOL CIndexBuffer::CreateBody(
	UN_UINT nIdxNum,
	E_GRAPH_INDEX_BUFFER_FMT nFmt,
	E_GRAPH_RSC_CREATE_TYPE nCreateType)
{
	UN_ASSERT(m_pDevice != UN_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	D3DFORMAT fmt = UN_GET_TARGET_IDX_BUF_FMT(nFmt);
	UN_UINT nStride = (fmt == D3DFMT_INDEX16 ? sizeof(UN_UINT16) : sizeof(UN_UINT32));

	UN_DWORD nUsage = (nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC
						? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY
						: 0);

	D3DPOOL nPool = (nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC
						? D3DPOOL_DEFAULT
						: D3DPOOL_MANAGED);

	HRESULT hr = pD3DDev->CreateIndexBuffer(
					nIdxNum * nStride,
					nUsage,
					fmt,
					nPool,
					&m_pIB,
					UN_NULL);

	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	m_nIdxNum = nIdxNum;
	m_Fmt = nFmt;
	
	m_nCreateType = nCreateType;

	return ret;
}

// ロック
UN_BOOL CIndexBuffer::Lock(
	UN_UINT nOffset,
	UN_UINT nSize,
	void** data,
	UN_BOOL bIsReadOnly,
	UN_BOOL bIsDiscard/*= UN_FALSE*/)
{
	// NOTE
	// 高速化のために IsDynamic のときは WRITEONLYで
	// 作成しているので、READONLY は不可

	UN_ASSERT(m_pIB != UN_NULL);

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

	HRESULT hr = m_pIB->Lock(
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
UN_BOOL CIndexBuffer::Unlock()
{
	UN_ASSERT(m_pIB != UN_NULL);
	
	HRESULT hr = m_pIB->Unlock();
	UN_BOOL ret = SUCCEEDED(hr);

	return ret;
}

// リセット
UN_BOOL CIndexBuffer::Reset()
{
	UN_BOOL ret = UN_TRUE;

	// NOTE
	// Dynamicでないときは、D3DPOOL_MANAGEDで作成されているので再作成する必要はない

	if (IsDynamic()) {
		D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

		SAFE_RELEASE(m_pIB);

		// 本体作成
		ret = CreateBody(
				m_nIdxNum,
				m_Fmt,
				m_nCreateType);
	}

	return ret;
}
