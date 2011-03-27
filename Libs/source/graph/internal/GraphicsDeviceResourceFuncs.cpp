#include "izGraph.h"

using namespace izanagi;

/**
* ファイルからテクスチャ作成
*/
CTexture* CGraphicsDevice::CreateTextureFromFile(
	IZ_PCSTR lpszPathName,
	E_GRAPH_PIXEL_FMT fmt/*= E_GRAPH_PIXEL_FMT_NUM*/)
{
	CTexture* pTexture = CTexture::CreateTextureFromFile(
							this,
							m_pAllocator,
							lpszPathName,
							fmt);
	return pTexture;
}

/**
* メモリからテクスチャ作成
*/
CTexture* CGraphicsDevice::CreateTextureFromMemory(
	void* pData,
	IZ_UINT nDataSize,
	E_GRAPH_PIXEL_FMT fmt)
{
	CTexture* pTexture = CTexture::CreateTextureFromMemory(
							this,
							m_pAllocator,
							pData,
							nDataSize,
							fmt);
	return pTexture;
}

/**
* テクスチャ作成
*/
CTexture* CGraphicsDevice::CreateTexture(
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT fmt,
	E_GRAPH_RSC_TYPE nRscType)
{
	CTexture* pTexture = CTexture::CreateTexture(
							this,
							m_pAllocator,
							nWidth, nHeight,
							nMipLevel,
							fmt,
							nRscType);

	if ((pTexture != IZ_NULL) && pTexture->IsOnVram()) {
		// リセット用リストに登録
		m_pResetTexture = InsertResource(pTexture, m_pResetTexture);
	}

	return pTexture;
}

/**
* ファイルからキューブテクスチャ作成
*/
CCubeTexture* CGraphicsDevice::CreateCubeTextureFromFile(
	IZ_PCSTR lpszPathName,
	E_GRAPH_PIXEL_FMT fmt/*= E_GRAPH_PIXEL_FMT_NUM*/)
{
	CCubeTexture* pTexture = CCubeTexture::CreateCubeTextureFromFile(
								this,
								m_pAllocator,
								lpszPathName,
								fmt);
	return pTexture;
}

/**
* キューブテクスチャ作成
*/
CCubeTexture* CGraphicsDevice::CreateCubeTexture(
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT fmt,
	IZ_BOOL bIsDynamic)
{
	CCubeTexture* pTexture = CCubeTexture::CreateCubeTexture(
								this,
								m_pAllocator,
								nWidth,
								nHeight,
								nMipLevel,
								fmt);
	return pTexture;
}

/**
* レンダーターゲット作成
*/
CTexture* CGraphicsDevice::CreateRenderTarget(
	IZ_UINT nWidth, IZ_UINT nHeight,
	E_GRAPH_PIXEL_FMT fmt)
{
	CTexture* pRT = CTexture::CreateRenderTarget(
						this,
						m_pAllocator,
						nWidth,
						nHeight,
						fmt);

	if (pRT != IZ_NULL) {
		// リセット用リストに登録
		m_pResetTexture = InsertResource(pRT, m_pResetTexture);
	}

	return pRT;
}

/**
* システムメモリ上にテクスチャ作成
*/
CTexture* CGraphicsDevice::CreateTextureOnSysMem(
	IZ_UINT nWidth, IZ_UINT nHeight,
	E_GRAPH_PIXEL_FMT fmt,
	IZ_UINT nMipLevel)
{
	CTexture* pTexture = CTexture::CreateTexture(
							this,
							m_pAllocator,
							nWidth,
							nHeight,
							nMipLevel,
							fmt,
							E_GRAPH_RSC_TYPE_STATIC,
							IZ_TRUE);
	return pTexture;
}

/**
* 頂点バッファ作成
*/
CVertexBuffer* CGraphicsDevice::CreateVertexBuffer(
	IZ_UINT nStride,
	IZ_UINT nVtxNum,
	E_GRAPH_RSC_TYPE nCreateType)
{
	CVertexBuffer* pVB = CVertexBuffer::CreateVertexBuffer(
							this,
							m_pAllocator,
							nStride,
							nVtxNum,
							nCreateType);

	if ((pVB != IZ_NULL)
		&& pVB->IsDynamic())
	{
		// リセット用リストに登録
		m_pResetVB = InsertResource(pVB, m_pResetVB);
	}

	return pVB;
}

/**
* インデックスバッファ作成
*/
CIndexBuffer* CGraphicsDevice::CreateIndexBuffer(
	IZ_UINT nIdxNum,
	E_GRAPH_INDEX_BUFFER_FMT fmt,
	E_GRAPH_RSC_TYPE nCreateType)
{
	CIndexBuffer* pIB = CIndexBuffer::CreateIndexBuffer(
							this,
							m_pAllocator,
							nIdxNum,
							fmt,
							nCreateType);

	if ((pIB != IZ_NULL)
		&& pIB->IsDynamic())
	{
		// リセット用リストに登録
		m_pResetIB = InsertResource(pIB, m_pResetIB);
	}

	return pIB;
}

/**
* 頂点シェーダ作成
*/
CVertexShader* CGraphicsDevice::CreateVertexShader(const void* pProgram)
{
	CVertexShader* ret = CVertexShader::CreateVertexShader(
							this,
							m_pAllocator,
							pProgram);
	return ret;
}

/**
* ピクセルシェーダ作成
*/
CPixelShader* CGraphicsDevice::CreatePixelShader(const void* pProgram)
{
	CPixelShader* ret = CPixelShader::CreatePixelShader(
							this,
							m_pAllocator,
							pProgram);
	return ret;
}

/**
* 頂点宣言作成
*/
CVertexDeclaration* CGraphicsDevice::CreateVertexDeclaration(const D3D_VTX_ELEMENT* pElem)
{
	CVertexDeclaration* ret = CVertexDeclaration::CreateVertexDeclaration(
								this,
								m_pAllocator,
								pElem);
	return ret;
}

CVertexDeclaration* CGraphicsDevice::CreateVertexDeclaration(const SVertexElement* pElem, IZ_UINT nNum)
{
	CVertexDeclaration* ret = CVertexDeclaration::CreateVertexDeclaration(
								this,
								m_pAllocator,
								pElem,
								nNum);
	return ret;
}

// リソース挿入
template <class _T>
_T* CGraphicsDevice::InsertResource(_T* p, _T* pListTop)
{
	if (pListTop == IZ_NULL) {
		pListTop = p;
	}
	else {
		_T* pElem = pListTop;
		_T* pTail = IZ_NULL;

		while (pElem != IZ_NULL) {
			pTail = pElem;
			pElem = pElem->m_pNext;
		}

		pTail->m_pNext = p;
	}

	return pListTop;
}

// リソース解放
template <class _T>
_T* CGraphicsDevice::RemoveResource(_T* p, _T* pListTop)
{
	if (pListTop == p) {
		pListTop = pListTop->m_pNext;
	}
	else {
		_T* pElem = pListTop;
		_T* prev = IZ_NULL;

		while (pElem != IZ_NULL) {
			if (pElem == p) {
				prev->m_pNext = pElem->m_pNext;
				break;
			}

			prev = pElem;
			pElem = pElem->m_pNext;
		}
	}

	return pListTop;
}

// テクスチャ削除
void CGraphicsDevice::RemoveTexture(CTexture* p)
{
	m_pResetTexture = RemoveResource(p, m_pResetTexture);
}

// 頂点バッファ削除
void CGraphicsDevice::RemoveVertexBuffer(CVertexBuffer* p)
{
	m_pResetVB = RemoveResource(p, m_pResetVB);
}

// インデックスバッファ削除
void CGraphicsDevice::RemoveIndexBuffer(CIndexBuffer* p)
{
	m_pResetIB = RemoveResource(p, m_pResetIB);
}
