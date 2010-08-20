#include "graph/ogl/IndexBuffer.h"
#include "graph/ogl/OGLExtFuncProxy.h"
#include "graph/ogl/OGLParamValueConverter.h"

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

// コンストラクタ
CIndexBuffer::CIndexBuffer()
{
	m_pDevice = UN_NULL;
	m_pAllocator = UN_NULL;

	m_nIBO = 0;

	m_nIdxNum = 0;
	m_Fmt = E_GRAPH_INDEX_BUFFER_FMT_FORCE_INT32;
	
	m_nCreateType = E_GRAPH_RSC_CREATE_TYPE_STATIC;
}

// デストラクタ
CIndexBuffer::~CIndexBuffer()
{
	m_pDevice->RemoveIndexBuffer(this);

	if (m_nIBO > 0) {
		UN_OGL_EXT_PROC(glDeleteBuffers)(1, &m_nIBO);
	}

	SAFE_RELEASE(m_pDevice);
}

// 本体作成
UN_BOOL CIndexBuffer::CreateBody(
	UN_UINT nIdxNum,
	E_GRAPH_INDEX_BUFFER_FMT nFmt,
	E_GRAPH_RSC_CREATE_TYPE nCreateType)
{
	UN_ASSERT(m_pDevice != UN_NULL);
	UN_ASSERT(m_nIBO == 0);

	UN_OGL_EXT_PROC(glGenBuffers)(1, &m_nIBO);
	VRETURN(m_nIBO > 0);

	m_nIdxNum = nIdxNum;
	m_Fmt = nFmt;

	UN_UINT nStride = GetStride();

	GLenum usage = (nCreateType == E_GRAPH_RSC_CREATE_TYPE_DYNAMIC
					? GL_DYNAMIC_DRAW
					: GL_STATIC_DRAW);

	UN_OGL_EXT_PROC(glBufferData)(
		GL_ARRAY_BUFFER,
		nIdxNum * nStride,
		NULL,
		usage);
	
	m_nCreateType = nCreateType;

	return UN_TRUE;
}

// ロック
UN_BOOL CIndexBuffer::Lock(
	UN_UINT nOffset,
	UN_UINT nSize,
	void** data,
	UN_BOOL bIsReadOnly,
	UN_BOOL bIsDiscard/*= UN_FALSE*/)
{
	UN_ASSERT(m_nIBO > 0);
	UN_ASSERT(m_LockInfo.data == UN_NULL);

	// NOTE
	// OpenGLES はバッファの読み込みはできないので
	// OpenGL でもできないようにする
	UN_ASSERT(!bIsReadOnly);

	// NOTE
	// nSize == 0 のときはバッファ全体

	nOffset = (nSize > 0 ? nOffset : 0);

	UN_UINT nStride = GetStride();

	nSize = (nSize > 0
				? nSize
				: nStride * m_nIdxNum);

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
UN_BOOL CIndexBuffer::Unlock()
{
	UN_ASSERT(m_nIBO > 0);

	if (m_LockInfo.IsLock() != UN_NULL) {
		UN_OGL_EXT_PROC(glBindBuffer)(GL_ARRAY_BUFFER, m_nIBO);

		UN_OGL_EXT_PROC(glGetBufferSubData)(
			GL_ARRAY_BUFFER,
			m_LockInfo.offset,
			m_LockInfo.size,
			m_LockInfo.data);

		FREE(m_pAllocator, m_LockInfo.data);

		// TODO
		// Error Check;

		UN_OGL_EXT_PROC(glBindBuffer)(GL_ARRAY_BUFFER, 0);
	}

	m_LockInfo.Clear();

	return UN_TRUE;;
}

// リセット
UN_BOOL CIndexBuffer::Reset()
{
	// Nothing is done.
	UN_ASSERT(UN_FALSE);
	return UN_TRUE;
}
