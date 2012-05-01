#include "graph/dx9/IndexBuffer.h"
#include "graph/dx9/D3D9ParamValueConverter.h"

using namespace izanagi;

// インスタンス作成
CIndexBuffer* CIndexBuffer::CreateIndexBuffer(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	IZ_UINT nIdxNum,
	E_GRAPH_INDEX_BUFFER_FMT fmt,
	E_GRAPH_RSC_TYPE nCreateType)
{
	IZ_ASSERT(pDevice != NULL);

	IZ_BOOL result = IZ_TRUE;
	IZ_UINT8* pBuf = IZ_NULL;
	CIndexBuffer* pInstance = IZ_NULL;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CIndexBuffer));
	if (!(result = (pBuf != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CIndexBuffer;
	{
		pInstance->m_Allocator = pAllocator;
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
IZ_BOOL CIndexBuffer::CreateBody(
	IZ_UINT nIdxNum,
	E_GRAPH_INDEX_BUFFER_FMT nFmt,
	E_GRAPH_RSC_TYPE nCreateType)
{
	IZ_ASSERT(m_pDevice != IZ_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	D3DFORMAT fmt = IZ_GET_TARGET_IDX_BUF_FMT(nFmt);
	IZ_UINT nStride = (fmt == D3DFMT_INDEX16 ? sizeof(IZ_UINT16) : sizeof(IZ_UINT32));

	IZ_DWORD nUsage = (nCreateType == E_GRAPH_RSC_TYPE_DYNAMIC
						? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY
						: 0);

	D3DPOOL nPool = (nCreateType == E_GRAPH_RSC_TYPE_DYNAMIC
						? D3DPOOL_DEFAULT
						: D3DPOOL_MANAGED);

	HRESULT hr = pD3DDev->CreateIndexBuffer(
					nIdxNum * nStride,
					nUsage,
					fmt,
					nPool,
					&m_pIB,
					IZ_NULL);

	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	m_nIdxNum = nIdxNum;
	m_Fmt = nFmt;
	
	m_nCreateType = nCreateType;

	return ret;
}

// ロック
IZ_BOOL CIndexBuffer::Lock(
	IZ_UINT nOffset,
	IZ_UINT nSize,
	void** data,
	IZ_BOOL bIsReadOnly,
	IZ_BOOL bIsDiscard/*= IZ_FALSE*/)
{
	// NOTE
	// 高速化のために IsDynamic のときは WRITEONLYで
	// 作成しているので、READONLY は不可

	IZ_ASSERT(m_pIB != IZ_NULL);

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

	IZ_BOOL ret = SUCCEEDED(hr);

	return ret;
}

/**
* アンロック
*/
IZ_BOOL CIndexBuffer::Unlock()
{
	IZ_ASSERT(m_pIB != IZ_NULL);
	
	HRESULT hr = m_pIB->Unlock();
	IZ_BOOL ret = SUCCEEDED(hr);

	return ret;
}

// リセット
IZ_BOOL CIndexBuffer::Reset()
{
	IZ_BOOL ret = IZ_TRUE;

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
