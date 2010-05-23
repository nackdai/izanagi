#include "unGraph.h"

using namespace uranus;

/**
* �t�@�C������e�N�X�`���쐬
*/
CTexture* CGraphicsDevice::CreateTextureFromFile(
	UN_PCSTR lpszPathName,
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
* ����������e�N�X�`���쐬
*/
CTexture* CGraphicsDevice::CreateTextureFromMemory(
	void* pData,
	UN_UINT nDataSize,
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
* �e�N�X�`���쐬
*/
CTexture* CGraphicsDevice::CreateTexture(
	UN_UINT nWidth,
	UN_UINT nHeight,
	UN_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT fmt,
	E_GRAPH_RSC_CREATE_TYPE nCreateType)
{
	CTexture* pTexture = CTexture::CreateTexture(
							this,
							m_pAllocator,
							nWidth, nHeight,
							nMipLevel,
							fmt,
							nCreateType);

	if ((pTexture != UN_NULL) && pTexture->IsOnVram()) {
		// ���Z�b�g�p���X�g�ɓo�^
		m_pResetTexture = InsertResource(pTexture, m_pResetTexture);
	}

	return pTexture;
}

/**
* �t�@�C������L���[�u�e�N�X�`���쐬
*/
CCubeTexture* CGraphicsDevice::CreateCubeTextureFromFile(
	UN_PCSTR lpszPathName,
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
* �L���[�u�e�N�X�`���쐬
*/
CCubeTexture* CGraphicsDevice::CreateCubeTexture(
	UN_UINT nWidth,
	UN_UINT nHeight,
	UN_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT fmt,
	UN_BOOL bIsDynamic)
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
* �����_�[�^�[�Q�b�g�쐬
*/
CTexture* CGraphicsDevice::CreateRenderTarget(
	UN_UINT nWidth, UN_UINT nHeight,
	E_GRAPH_PIXEL_FMT fmt)
{
	CTexture* pRT = CTexture::CreateRenderTarget(
						this,
						m_pAllocator,
						nWidth,
						nHeight,
						fmt);

	if (pRT != UN_NULL) {
		// ���Z�b�g�p���X�g�ɓo�^
		m_pResetTexture = InsertResource(pRT, m_pResetTexture);
	}

	return pRT;
}

/**
* �V�X�e����������Ƀe�N�X�`���쐬
*/
CTexture* CGraphicsDevice::CreateTextureOnSysMem(
	UN_UINT nWidth, UN_UINT nHeight,
	E_GRAPH_PIXEL_FMT fmt,
	UN_UINT nMipLevel)
{
	CTexture* pTexture = CTexture::CreateTexture(
							this,
							m_pAllocator,
							nWidth,
							nHeight,
							nMipLevel,
							fmt,
							E_GRAPH_RSC_CREATE_TYPE_STATIC,
							UN_TRUE);
	return pTexture;
}

/**
* ���_�o�b�t�@�쐬
*/
CVertexBuffer* CGraphicsDevice::CreateVertexBuffer(
	UN_UINT nStride,
	UN_UINT nVtxNum,
	E_GRAPH_RSC_CREATE_TYPE nCreateType)
{
	CVertexBuffer* pVB = CVertexBuffer::CreateVertexBuffer(
							this,
							m_pAllocator,
							nStride,
							nVtxNum,
							nCreateType);

	if ((pVB != UN_NULL)
		&& pVB->IsDynamic())
	{
		// ���Z�b�g�p���X�g�ɓo�^
		m_pResetVB = InsertResource(pVB, m_pResetVB);
	}

	return pVB;
}

/**
* �C���f�b�N�X�o�b�t�@�쐬
*/
CIndexBuffer* CGraphicsDevice::CreateIndexBuffer(
	UN_UINT nIdxNum,
	E_GRAPH_INDEX_BUFFER_FMT fmt,
	E_GRAPH_RSC_CREATE_TYPE nCreateType)
{
	CIndexBuffer* pIB = CIndexBuffer::CreateIndexBuffer(
							this,
							m_pAllocator,
							nIdxNum,
							fmt,
							nCreateType);

	if ((pIB != UN_NULL)
		&& pIB->IsDynamic())
	{
		// ���Z�b�g�p���X�g�ɓo�^
		m_pResetIB = InsertResource(pIB, m_pResetIB);
	}

	return pIB;
}

/**
* ���_�V�F�[�_�쐬
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
* �s�N�Z���V�F�[�_�쐬
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
* ���_�錾�쐬
*/
CVertexDeclaration* CGraphicsDevice::CreateVertexDeclaration(const D3D_VTX_ELEMENT* pElem)
{
	CVertexDeclaration* ret = CVertexDeclaration::CreateVertexDeclaration(
								this,
								m_pAllocator,
								pElem);
	return ret;
}

CVertexDeclaration* CGraphicsDevice::CreateVertexDeclaration(const SVertexElement* pElem, UN_UINT nNum)
{
	CVertexDeclaration* ret = CVertexDeclaration::CreateVertexDeclaration(
								this,
								m_pAllocator,
								pElem,
								nNum);
	return ret;
}

// ���\�[�X�}��
template <class _T>
_T* CGraphicsDevice::InsertResource(_T* p, _T* pListTop)
{
	if (pListTop == UN_NULL) {
		pListTop = p;
	}
	else {
		_T* pElem = pListTop;
		_T* pTail = UN_NULL;

		while (pElem != UN_NULL) {
			pTail = pElem;
			pElem = pElem->m_pNext;
		}

		pTail->m_pNext = p;
	}

	return pListTop;
}

// ���\�[�X���
template <class _T>
_T* CGraphicsDevice::RemoveResource(_T* p, _T* pListTop)
{
	if (pListTop == p) {
		pListTop = pListTop->m_pNext;
	}
	else {
		_T* pElem = pListTop;
		_T* prev = UN_NULL;

		while (pElem != UN_NULL) {
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

// �e�N�X�`���폜
void CGraphicsDevice::RemoveTexture(CTexture* p)
{
	m_pResetTexture = RemoveResource(p, m_pResetTexture);
}

// ���_�o�b�t�@�폜
void CGraphicsDevice::RemoveVertexBuffer(CVertexBuffer* p)
{
	m_pResetVB = RemoveResource(p, m_pResetVB);
}

// �C���f�b�N�X�o�b�t�@�폜
void CGraphicsDevice::RemoveIndexBuffer(CIndexBuffer* p)
{
	m_pResetIB = RemoveResource(p, m_pResetIB);
}
