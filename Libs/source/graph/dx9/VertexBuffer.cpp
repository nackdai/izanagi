#include "graph/dx9/VertexBuffer.h"

using namespace uranus;

// �C���X�^���X�쐬
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

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CVertexBuffer));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new (pBuf)CVertexBuffer;
	{
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);
		pInstance->m_pAllocator = pAllocator;
		pInstance->AddRef();
	}

	// �{�̍쐬
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

// �{�̍쐬
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

	// �{�̍쐬
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

// ���b�N
UN_BOOL CVertexBuffer::Lock(
	UN_UINT nOffset,
	UN_UINT nSize,
	void** data,
	UN_BOOL bIsReadOnly,
	UN_BOOL bIsDiscard/*= UN_FALSE*/)
{
	// NOTE
	// �������̂��߂� IsDynamic �̂Ƃ��� WRITEONLY��
	// �쐬���Ă���̂ŁAREADONLY �͕s��

	UN_ASSERT(m_pVB != UN_NULL);

	DWORD flag = 0;
	if (IsDynamic()) {
		// READONLY�s��
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
* �A�����b�N
*/
UN_BOOL CVertexBuffer::Unlock()
{
	UN_ASSERT(m_pVB != UN_NULL);
	
	HRESULT hr = m_pVB->Unlock();
	UN_BOOL ret = SUCCEEDED(hr);

	return ret;
}

// ���Z�b�g
UN_BOOL CVertexBuffer::Reset()
{
	UN_BOOL ret = UN_TRUE;

	// NOTE
	// Dynamic�łȂ��Ƃ��́AD3DPOOL_MANAGED�ō쐬����Ă���̂ōč쐬����K�v�͂Ȃ�

	if (IsDynamic()) {
		SAFE_RELEASE(m_pVB);

		// �{�̍쐬
		ret = CreateBody(
				m_nStride,
				m_nVtxNum,
				m_nCreateType);
	}

	return ret;
}
