#include "graph/ogl/VertexBuffer.h"
#include "graph/ogl/OGLExtFuncProxy.h"

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
	UN_ASSERT(m_nVBO == 0);

	UN_OGL_EXT_PROC(glGenBuffers)(1, &m_nVBO);
	VRETURN(m_nVBO > 0);

	GLenum usage = (nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC
					? GL_DYNAMIC_DRAW
					: GL_STATIC_DRAW);

	UN_OGL_EXT_PROC(glBufferData)(
		GL_ARRAY_BUFFER,
		nVtxNum * nStride,
		NULL,
		usage);

	m_nStride = nStride;
	m_nVtxNum = nVtxNum;
	
	m_nCreateType = nCreateType;

	return UN_TRUE;
}

// コンストラクタ
CVertexBuffer::CVertexBuffer()
{
	m_pDevice = UN_NULL;
	m_pAllocator = UN_NULL;

	m_nVBO = 0;

	m_nStride = 0;
	m_nVtxNum = 0;

	m_LockInfo.Clear();

	m_nCreateType = E_GRAPH_RSC_CREATE_TYPE_STATIC;
}

// デストラクタ
CVertexBuffer::~CVertexBuffer()
{
	m_pDevice->RemoveVertexBuffer(this);

	FREE(m_pAllocator, m_LockInfo.data);

	if (m_nVBO > 0) {
		UN_OGL_EXT_PROC(glDeleteBuffers)(1, &m_nVBO);
	}
	
	SAFE_RELEASE(m_pDevice);
}

// ロック
UN_BOOL CVertexBuffer::Lock(
	UN_UINT nOffset,
	UN_UINT nSize,
	void** data,
	UN_BOOL bIsReadOnly,
	UN_BOOL bIsDiscard/*= UN_FALSE*/)
{
	UN_ASSERT(m_nVBO > 0);
	UN_ASSERT(m_LockInfo.data == UN_NULL);

	// NOTE
	// OpenGLES はバッファの読み込みはできないので
	// OpenGL でもできないようにする
	UN_ASSERT(!bIsReadOnly);

	// NOTE
	// nSize == 0 のときはバッファ全体

	nOffset = (nSize > 0 ? nOffset : 0);

	nSize = (nSize > 0
				? nSize
				: m_nStride * m_nVtxNum);

	m_LockInfo.data = ALLOC_ZERO(m_pAllocator, nSize);
	VRETURN(m_LockInfo.data != UN_NULL);

	*data = m_LockInfo.data;
	m_LockInfo.offset = nOffset;
	m_LockInfo.size = nSize;

	return UN_TRUE;;
}

/**
* アンロック
*/
UN_BOOL CVertexBuffer::Unlock()
{
	UN_ASSERT(m_nVBO > 0);

	if (m_LockInfo.IsLock() != UN_NULL) {
		UN_OGL_EXT_PROC(glBindBuffer)(GL_ARRAY_BUFFER, m_nVBO);

		UN_OGL_EXT_PROC(glGetBufferSubData)(
			GL_ARRAY_BUFFER,
			m_LockInfo.offset,
			m_LockInfo.size,
			m_LockInfo.data);

		FREE(m_pAllocator, m_LockInfo.data);

		// TODO
		// Error Check...

		UN_OGL_EXT_PROC(glBindBuffer)(GL_ARRAY_BUFFER, 0);
	}

	m_LockInfo.Clear();

	return UN_TRUE;;
}

// リセット
UN_BOOL CVertexBuffer::Reset()
{
	// Nothing is done.
	UN_ASSERT(UN_FALSE);
	return UN_TRUE;
}
