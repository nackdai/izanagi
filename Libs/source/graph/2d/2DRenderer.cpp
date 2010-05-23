#include "graph/2d/2DRenderer.h"
#include "graph/2d/2DShader.h"
#include "std/allocator/MemoryAllocator.h"
#include "unGraph.h"

using namespace uranus;

// �C���X�^���X�쐬
C2DRenderer* C2DRenderer::Create2DRenderer(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator)
{
	UN_ASSERT(pDevice != UN_NULL);
	UN_ASSERT(pAllocator != UN_NULL);

	UN_BOOL result = UN_FALSE;
	UN_UINT8* pBuf = UN_NULL;
	C2DRenderer* pInstance = UN_NULL;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(C2DRenderer));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new (pBuf)C2DRenderer;
	if (!(result = (pInstance != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	{
		pInstance->m_pAllocator = pAllocator;
		pInstance->AddRef();

		// ������
		result = pInstance->Init(pDevice);
		if (!result) {
			UN_ASSERT(UN_FALSE);
			goto __EXIT__;
		}
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

// �R���X�g���N�^
C2DRenderer::C2DRenderer()
{
	m_pAllocator = UN_NULL;

	m_pVB = UN_NULL;
	m_pIB = UN_NULL;
	m_pVD = UN_NULL;

	m_pShader = UN_NULL;

	m_nPrimType = PRIM_TYPE_SPRITE;

	m_nCurIdx = 0;
	m_nCurPrimNum = 0;
}

// �f�X�g���N�^
C2DRenderer::~C2DRenderer()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVD);
	SAFE_RELEASE(m_pShader);
}

// ���
void C2DRenderer::InternalRelease()
{
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

// ������
UN_BOOL C2DRenderer::Init(CGraphicsDevice* pDevice)
{
	UN_ASSERT(pDevice != UN_NULL);

	// ���_�o�b�t�@
	{
		m_pVB = pDevice->CreateVertexBuffer(
					VTX_STRIDE,
					MAX_VERTEX_NUM,
					E_GRAPH_RSC_CREATE_TYPE_DYNAMIC);
		VRETURN(m_pVB != UN_NULL);

		// ���Ȃ薳�����邪�E�E�E
		// m_pVB�̕��̃f�o�C�X�̎Q�ƃJ�E���g�����炷
		pDevice->Release();
	}

	// �C���f�b�N�X�o�b�t�@
	{
		m_pIB = pDevice->CreateIndexBuffer(
					MAX_INDEX_NUM,
					E_GRAPH_INDEX_BUFFER_FMT_INDEX16,	// TODO �Ƃ肠�����E�E�E
					E_GRAPH_RSC_CREATE_TYPE_DYNAMIC);
		VRETURN(m_pIB != UN_NULL);

		// ���Ȃ薳�����邪�E�E�E
		// m_pIB�̕��̃f�o�C�X�̎Q�ƃJ�E���g�����炷
		pDevice->Release();
	}

#if 0
	// ���_�錾
	static const D3D_VTX_ELEMENT VERTEX_ELEMENT[] = {
		{0,  0, D3DDECLTYPE_FLOAT4,   0, D3DDECLUSAGE_POSITION, 0},	// ���W
		{0, 16, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR,    0},	// ���_�J���[
		{0, 20, D3DDECLTYPE_FLOAT2,   0, D3DDECLUSAGE_TEXCOORD, 0},	// �e�N�X�`�����W
		D3DDECL_END(),
	};
	{
		m_pVD = pDevice->CreateVertexDeclaration(VERTEX_ELEMENT);
		VRETURN(m_pVD != UN_NULL);

		// ���Ȃ薳�����邪�E�E�E
		// m_pVD�̕��̃f�o�C�X�̎Q�ƃJ�E���g�����炷
		pDevice->Release();
	}
#else
	static const SVertexElement VtxElement[] = {
		{0,  0, E_GRAPH_VTX_DECL_TYPE_FLOAT4, E_GRAPH_VTX_DECL_USAGE_POSITION, 0},	// ���W
		{0, 16, E_GRAPH_VTX_DECL_TYPE_COLOR,  E_GRAPH_VTX_DECL_USAGE_COLOR,    0},	// ���_�J���[
		{0, 20, E_GRAPH_VTX_DECL_TYPE_FLOAT2, E_GRAPH_VTX_DECL_USAGE_TEXCOORD, 0},	// �e�N�X�`�����W
	};

	{
		m_pVD = pDevice->CreateVertexDeclaration(VtxElement, COUNTOF(VtxElement));
		VRETURN(m_pVD != UN_NULL);

		// ���Ȃ薳�����邪�E�E�E
		// m_pVD�̕��̃f�o�C�X�̎Q�ƃJ�E���g�����炷
		pDevice->Release();
	}
#endif

	// �V�F�[�_
	{
		// �Q�ƃJ�E���g�����o���Ă����E�E�E
		UN_UINT nRefCnt = pDevice->GetRefCnt();

		m_pShader = C2DShader::Create2DShader(
						m_pAllocator,
						pDevice);
		VRETURN(m_pShader != UN_NULL);

		// �������J�E���g��
		nRefCnt = pDevice->GetRefCnt() - nRefCnt;

		// �������������f�o�C�X�̎Q�ƃJ�E���g�����炷
		for (UN_UINT i = 0; i < nRefCnt; i++) {
			pDevice->Release();
		}
	}

	return UN_TRUE;
}

// �t���[���J�n���ɌĂԂ���
void C2DRenderer::BeginFrame()
{
	m_sVBInfo.Clear();
	m_sIBInfo.Clear();

	m_nCurIdx = 0;
	m_nCurPrimNum = 0;
}

// �`��J�n
UN_BOOL C2DRenderer::BeginDraw()
{
	m_sVBInfo.buf_ptr = UN_NULL;
	m_sIBInfo.buf_ptr = UN_NULL;

	m_bIsLock = UN_FALSE;

	return UN_TRUE;
}

// �`��I��
UN_BOOL C2DRenderer::EndDraw(CGraphicsDevice* pDevice)
{
	// �Ƃ����Ă��t���b�V�����邾���E�E�E
	UN_BOOL ret = Flush(pDevice);

	// FVF�������N���A
	pDevice->SetFVF(0);

	// FVF�`��ɂ�蒸�_�錾�������ɂȂ��Ă���̂�
	pDevice->SetVertexDeclaration(UN_NULL);

	return ret;
}

namespace {
	// ��`�̒��_���擾
	UN_UINT GetRectVtxNum(UN_UINT nRectNum)
	{
		return nRectNum << 2;
	}

	// ��`��`�悷�邽�߂ɕK�v�ȃv���~�e�B�u���v�Z
	UN_UINT ComputeRectRealPrimNum(UN_UINT nRectNum)
	{
		// �v���~�e�B�u��
		// NOTE
		// �P��` = �Q�O�p�`(�v���~�e�B�u)
		// ��`�Ƌ�`�̊Ԃ��Ȃ������Ȃ��O�p�`�͂S���݂���
		// ex) �Q��` -> 0123 34 4567
		//               012-123-[233-334-344-445]-456-567 -> 2 * 2 + 4
		UN_UINT nPrimNum = nRectNum * 2 + (nRectNum - 1) * 4;

		return nPrimNum;
	}

	// ��`�`�掞�̍ŏ��C���f�b�N�X�l�v�Z
	UN_INT ComputeRectMinIdx(
		UN_UINT nIdx,
		UN_UINT nRectNum)
	{
		// �ŏ��C���f�b�N�X�l
		// NOTE
		// �����`�̃C���f�b�N�X�ɂ��čl����
		// n, n+1, n+2, n+3 �ƂȂ�Am_ushNowSetIndex = n+3 �ƂȂ��Ă���
		// ����āA-3���邱�Ƃŋ�`�̐擪�̃C���f�b�N�X�ɂȂ�B
		// ����ɁA��`�̐擪�̃C���f�b�N�X�͂S�̔{���ɂȂ�B
		// ����āA(�`�悷���`�� - 1) x 4 �����}�C�i�X����ƍŏ��C���f�b�N�X�l�ɂȂ�
		// -1 ���Ă���̂́A�`�悷���`�����P�̂Ƃ��́A�}�C�i�X����K�v���Ȃ��̂ŁB
		UN_INT nMinIdx = nIdx - 3 - 4 * (nRectNum - 1);

		return nMinIdx;
	}

	// ���̒��_���擾
	UN_UINT GetLineVtxNum(UN_UINT nLineNum)
	{
		return nLineNum << 1;
	}

	// ����`�悷�邽�߂ɕK�v�ȃv���~�e�B�u���v�Z
	UN_UINT ComputeLineRealPrimNum(UN_UINT nLineNum)
	{
		return nLineNum;
	}

	// ��`�`�掞�̍ŏ��C���f�b�N�X�l�v�Z
	UN_INT ComputeLineMinIdx(
		UN_UINT nIdx,
		UN_UINT nLineNum)
	{
		UN_INT nMinIdx = nIdx - 2 * nLineNum;
		return nMinIdx;
	}

	// ���_���擾
	UN_UINT (*GetVtxNum[])(UN_UINT) = {
		GetRectVtxNum,
		GetRectVtxNum,
		GetLineVtxNum,
	};

	// �`�悷�邽�߂ɕK�v�ȃv���~�e�B�u���v�Z
	UN_UINT (*ComputeRealPrimNum[])(UN_UINT) = {
		ComputeRectRealPrimNum,
		ComputeRectRealPrimNum,
		ComputeLineRealPrimNum,
	};

	// �`�掞�̍ŏ��C���f�b�N�X�l�v�Z
	UN_INT (*ComputeMinIdx[])(UN_UINT, UN_UINT) = {
		ComputeRectMinIdx,
		ComputeRectMinIdx,
		ComputeLineMinIdx,
	};

	E_GRAPH_PRIM_TYPE PrimType[] = {
		E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
		E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
		E_GRAPH_PRIM_TYPE_LINESTRIP,
	};
}	// namespace

// �`��R�}���h���t���b�V��
UN_BOOL C2DRenderer::Flush(CGraphicsDevice* pDevice)
{
	UN_C_ASSERT(COUNTOF(GetVtxNum) == PRIM_TYPE_NUM);
	UN_C_ASSERT(COUNTOF(ComputeRealPrimNum) == PRIM_TYPE_NUM);
	UN_C_ASSERT(COUNTOF(ComputeMinIdx) == PRIM_TYPE_NUM);
	UN_C_ASSERT(COUNTOF(PrimType) == PRIM_TYPE_NUM);

	// �A�����b�N
	Unlock();

	if (m_nCurPrimNum == 0) {
		// �����o�^���Ă��Ȃ�
		return UN_TRUE;
	}

	{
		// �C���f�b�N�X�o�b�t�@�̃Z�b�g
		pDevice->SetIndexBuffer(m_pIB);

		// ���_�錾�̃Z�b�g
		pDevice->SetVertexDeclaration(m_pVD);

		// ���_�o�b�t�@�̃Z�b�g
		pDevice->SetVertexBuffer(
			0,
			0,
			m_pVB->GetStride(),
			m_pVB);

		// �V�F�[�_�̃Z�b�g
		m_pShader->SetShader(pDevice);

		// �V�F�[�_�p�����[�^�̃Z�b�g
		m_pShader->SetShaderParams(pDevice);
	}

	// ���_��
	UN_UINT nVtxNum = (*GetVtxNum[m_nPrimType])(m_nCurPrimNum);

	// �v���~�e�B�u��
	UN_UINT nPrimNum = (*ComputeRealPrimNum[m_nPrimType])(m_nCurPrimNum);

	// �ŏ��C���f�b�N�X�l
	UN_INT nMinIdx = (*ComputeMinIdx[m_nPrimType])(m_nCurIdx, m_nCurPrimNum);
	nMinIdx = (nMinIdx < 0 ? 0 : nMinIdx);	// �ꉞ�E�E�E

	UN_UINT nBaseIdx = m_sVBInfo.offset / m_pVB->GetStride();
	UN_UINT nStartIdx = m_sIBInfo.offset / m_pIB->GetStride();

	// NOTE
	// �C���f�b�N�X�͕K���O����n�܂�

	// �`��
	UN_BOOL ret = pDevice->DrawIndexedPrimitive(
					PrimType[m_nPrimType],
					nBaseIdx,	// BaseIdx
					nMinIdx,	// MinIdx
					nVtxNum,	// VtxNum
					nStartIdx,	// StartIdx
					nPrimNum);	// PrimCnt

	m_nCurIdx = 0;
	m_nCurPrimNum = 0;

	return ret;
}

// �X�v���C�g�`��
UN_BOOL C2DRenderer::DrawSprite(
	CGraphicsDevice* pDevice,
	const CFloatRect& rcSrc,
	const CIntRect& rcDst,
	const UN_COLOR color/*= UN_COLOR_RGBA(255, 255, 255, 255)*/)
{
	// �`�揀��
	VRETURN(PrepareDraw(
				pDevice,
				PRIM_TYPE_SPRITE));

	// ���b�N
	VRETURN(Lock());

	// UV���W��`
	CFloatRect rcUV(rcSrc);
	{
		// �e�N�Z���̒��S���T���v�����O����悤�ɂ���
		CBaseTexture* pTex = pDevice->GetTexture(0);
		UN_FLOAT fTexWidth = (UN_FLOAT)pTex->GetWidth();
		UN_FLOAT fTexHeight = (UN_FLOAT)pTex->GetHeight();
		
		rcUV.left += 0.5f / fTexWidth;
		rcUV.top += 0.5f / fTexHeight;
		rcUV.right += 0.5f / fTexWidth;
		rcUV.bottom += 0.5f / fTexHeight;
	}
	
	// ���_�f�[�^�Z�b�g
	SetVtx(
		rcUV, rcDst,
		color);

	// �C���f�b�N�X�f�[�^�Z�b�g
	SetIdx();

	m_nCurPrimNum++;

	return UN_TRUE;
}

// �X�v���C�g�`��
UN_BOOL C2DRenderer::DrawSpriteEx(
	CGraphicsDevice* pDevice,
	const CIntRect& rcSrc,
	const CIntRect& rcDst,
	const UN_COLOR color/*= UN_COLOR_RGBA(255, 255, 255, 255)*/)
{
	// �`�揀��
	VRETURN(PrepareDraw(
				pDevice,
				PRIM_TYPE_SPRITE));

	// ���b�N
	VRETURN(Lock());

	// UV���W��`
	CFloatRect rcUV;
	{
		UN_FLOAT fTexLeft = static_cast<UN_FLOAT>(rcSrc.left);
		UN_FLOAT fTexTop = static_cast<UN_FLOAT>(rcSrc.top);
		UN_FLOAT fTexRight = static_cast<UN_FLOAT>(rcSrc.right);
		UN_FLOAT fTexBottom = static_cast<UN_FLOAT>(rcSrc.bottom);

		{
			// �e�N�Z���̒��S���T���v�����O����悤�ɂ���
			fTexLeft += 0.5f;
			fTexTop += 0.5f;
			fTexRight += 0.5f;
			fTexBottom += 0.5f;

			CBaseTexture* pTex = pDevice->GetTexture(0);
			UN_FLOAT fTexWidth = (UN_FLOAT)pTex->GetWidth();
			UN_FLOAT fTexHeight = (UN_FLOAT)pTex->GetHeight();
			
			rcUV.left = fTexLeft / fTexWidth;
			rcUV.top = fTexTop / fTexHeight;
			rcUV.right = fTexRight / fTexWidth;
			rcUV.bottom = fTexBottom / fTexHeight;
		}
	}
	
	// ���_�f�[�^�Z�b�g
	SetVtx(
		rcUV, rcDst,
		color);

	// �C���f�b�N�X�f�[�^�Z�b�g
	SetIdx();

	m_nCurPrimNum++;

	return UN_TRUE;
}

// ��`�`��
UN_BOOL C2DRenderer::DrawRect(
	CGraphicsDevice* pDevice,
	const CIntRect& rcDst,
	const UN_COLOR color)
{
	// ���̒l���o���Ă���
	E_GRAPH_2D_RENDER_OP nPrevOp = GetRenderOp();

	// �e�N�X�`���𒣂�Ȃ��̂ŋ����I��
	VRETURN(SetRenderOp(
				pDevice,
				E_GRAPH_2D_RENDER_OP_VTX));

	// �`�揀��
	VRETURN(PrepareDraw(
				pDevice,
				PRIM_TYPE_RECT));

	// ���b�N
	VRETURN(Lock());
	
	// ���_�f�[�^�Z�b�g
	SetVtx(
		CFloatRect(),	// �_�~�[
		rcDst,
		color);

	// �C���f�b�N�X�f�[�^�Z�b�g
	SetIdx();

	m_nCurPrimNum++;

	// ���ɖ߂�
	VRETURN(SetRenderOp(
				pDevice,
				nPrevOp));

	return UN_TRUE;
}

// ���`��
UN_BOOL C2DRenderer::DrawLine(
	CGraphicsDevice* pDevice,
	const CIntPoint& ptStart,
	const CIntPoint& ptGoal,
	const UN_COLOR color)
{
	// ���̒l���o���Ă���
	E_GRAPH_2D_RENDER_OP nPrevOp = GetRenderOp();

	// �e�N�X�`���𒣂�Ȃ��̂ŋ����I��
	VRETURN(SetRenderOp(
				pDevice,
				E_GRAPH_2D_RENDER_OP_VTX));

	// �`�揀��
	VRETURN(PrepareDraw(
				pDevice,
				PRIM_TYPE_LINE));

	// ���b�N
	VRETURN(Lock());
	
	// ���_�f�[�^�Z�b�g
	{
		CUSTOM_VERTEX* v = (CUSTOM_VERTEX*)m_sVBInfo.buf_ptr;

		// ���_�F
		v[0].color = v[1].color = v[2].color = v[3].color = color;

		// ���_�ʒu
		v[0].x = (UN_FLOAT)ptStart.x;
		v[0].y = (UN_FLOAT)ptStart.y;

		v[1].x = (UN_FLOAT)ptGoal.x;
		v[1].y = (UN_FLOAT)ptGoal.y;

		v[0].z = v[1].z = 0.5f;	// �K��
		v[0].w = v[1].w = 1.0f;

		// �P���C���Q���_�Ȃ̂�
		m_sVBInfo.buf_ptr = (UN_UINT8*)m_sVBInfo.buf_ptr + 2 * m_pVB->GetStride();
		m_sVBInfo.num += 2;
	}

	// �C���f�b�N�X�f�[�^�Z�b�g
	{
		UN_ASSERT(m_pIB->GetFormat() == E_GRAPH_INDEX_BUFFER_FMT_INDEX16);

		UN_UINT16* pIdx = (UN_UINT16*)m_sIBInfo.buf_ptr;

		*pIdx++ = m_nCurIdx++;
		*pIdx++ = m_nCurIdx++;

		m_sIBInfo.buf_ptr = pIdx;
		m_sIBInfo.num += 2;
	}

	m_nCurPrimNum++;

	// ���ɖ߂�
	VRETURN(SetRenderOp(
				pDevice,
				nPrevOp));

	return UN_TRUE;
}

// �`��ݒ�Z�b�g
UN_BOOL C2DRenderer::SetRenderOp(
	CGraphicsDevice* pDevice,
	E_GRAPH_2D_RENDER_OP nOp)
{
	UN_ASSERT(m_pShader != UN_NULL);

	// �O�ƈقȂ�Ƃ��̓t���b�V������
	if (m_pShader->GetRenderOp() != nOp) {
		VRETURN(Flush(pDevice));
	}

	m_pShader->SetRenderOp(nOp);

	return UN_TRUE;
}

// �`�揀��
UN_BOOL C2DRenderer::PrepareDraw(
	CGraphicsDevice* pDevice,
	PRIM_TYPE nPrimType)
{
	if (m_nPrimType != nPrimType) {
		// �v���~�e�B�u�^�C�v���Ⴄ�Ƃ��̓t���b�V������
		VRETURN(Flush(pDevice));
	}

	m_nPrimType = nPrimType;

	// ���_�o�b�t�@
	{
		// �P�v���~�e�B�u������̒��_��
		UN_UINT nVtxNumPerPrim = 0;
		switch (nPrimType) {
		case PRIM_TYPE_SPRITE:
		case PRIM_TYPE_RECT:
			nVtxNumPerPrim = 4;
			break;
		case PRIM_TYPE_LINE:
			nVtxNumPerPrim = 2;
			break;
		default:
			UN_ASSERT(UN_FALSE);
			break;
		};

		UN_UINT nSize = m_sVBInfo.num * m_pVB->GetStride();
		nSize += nVtxNumPerPrim * m_pVB->GetStride();

		if (nSize > m_pVB->GetSize()) {
			UN_ASSERT(UN_FALSE);	// �ꉞ

			Flush(pDevice);
			m_sVBInfo.Clear();

			m_sVBInfo.next_lock_discard = UN_TRUE;
		}
	}

	// �C���f�b�N�X�o�b�t�@
	{
		// �P�v���~�e�B�u������̃C���f�b�N�X��
		UN_UINT nIdxNumPerPrim = 0;
		switch (nPrimType) {
		case PRIM_TYPE_SPRITE:
		case PRIM_TYPE_RECT:
			nIdxNumPerPrim = (m_nCurIdx == 0 ? 4 : 6);
			break;
		case PRIM_TYPE_LINE:
			nIdxNumPerPrim = 2;
			break;
		default:
			UN_ASSERT(UN_FALSE);
			break;
		};

		UN_UINT nSize = m_sIBInfo.num * m_pIB->GetStride();
		nSize += nIdxNumPerPrim * m_pIB->GetStride();

		if (nSize > m_pIB->GetSize()) {
			UN_ASSERT(UN_FALSE);	// �ꉞ

			Flush(pDevice);
			m_sIBInfo.Clear();

			m_sIBInfo.next_lock_discard = UN_TRUE;
		}
	}

	return UN_TRUE;
}

// ���b�N
UN_BOOL C2DRenderer::Lock()
{
	if (!IsLock()) {
		// ���b�N�t���O�؂�ւ�
		ToggleIsLock();

		// ���_�o�b�t�@
		{
			m_sVBInfo.offset = m_sVBInfo.num * m_pVB->GetStride();

			// ���b�N�T�C�Y
			UN_UINT nLockSize = (m_sVBInfo.next_lock_discard	// DISCARD�Ń��b�N���邩�ǂ���
									? 0
									: m_pVB->GetSize() - m_sVBInfo.offset);	// �c�胍�b�N�\�T�C�Y

			UN_BOOL result = m_pVB->Lock(
								m_sVBInfo.offset,
								nLockSize,
								(void**)&m_sVBInfo.buf_ptr,
								UN_FALSE,
								m_sVBInfo.next_lock_discard);
			m_sVBInfo.next_lock_discard = UN_FALSE;
			VRETURN(result);
		}

		// �C���f�b�N�X�o�b�t�@
		{
			m_sIBInfo.offset = m_sIBInfo.num * m_pIB->GetStride();

			// ���b�N�T�C�Y
			UN_UINT nLockSize = (m_sIBInfo.next_lock_discard	// DISCARD�Ń��b�N���邩�ǂ���
									? 0
									: m_pIB->GetSize() - m_sIBInfo.offset);	// �c�胍�b�N�\�T�C�Y

			UN_BOOL result = m_pIB->Lock(
								m_sIBInfo.offset,
								nLockSize,
								(void**)&m_sIBInfo.buf_ptr,
								UN_FALSE,
								m_sIBInfo.next_lock_discard);
			m_sIBInfo.next_lock_discard = UN_FALSE;
			VRETURN(result);
		}
	}

	return UN_TRUE;
}

// �A�����b�N
void C2DRenderer::Unlock()
{
	if (IsLock()) {
		// ���b�N����Ă���
		ToggleIsLock();

		m_pVB->Unlock();
		m_pIB->Unlock();

		m_sVBInfo.buf_ptr = UN_NULL;
		m_sIBInfo.buf_ptr = UN_NULL;
	}
}

// ���_�f�[�^�Z�b�g
void C2DRenderer::SetVtx(
	const CFloatRect& rcSrc,
	const CIntRect& rcDst,
	const UN_COLOR color)
{
	CUSTOM_VERTEX* v = (CUSTOM_VERTEX*)m_sVBInfo.buf_ptr;

	// ���_�F
	v[0].color = v[1].color = v[2].color = v[3].color = color;

	// �]�����`
	{
		UN_FLOAT fLeft = static_cast<UN_FLOAT>(rcDst.left);
		UN_FLOAT fTop = static_cast<UN_FLOAT>(rcDst.top);
		UN_FLOAT fRight = static_cast<UN_FLOAT>(rcDst.right);
		UN_FLOAT fBottom = static_cast<UN_FLOAT>(rcDst.bottom);

		v[0].x = fLeft;
		v[0].y = fTop;

		v[1].x = fLeft;
		v[1].y = fBottom;

		v[2].x = fRight;
		v[2].y = fTop;

		v[3].x = fRight;
		v[3].y = fBottom;

		v[0].z = v[1].z = v[2].z = v[3].z = 0.5f;	// �K���E�E�E
		v[0].w = v[1].w = v[2].w = v[3].w = 1.0f;
	}

	v[0].u = rcSrc.left;
	v[0].v = rcSrc.top;

	v[1].u = rcSrc.left;
	v[1].v = rcSrc.bottom;

	v[2].u = rcSrc.right;
	v[2].v = rcSrc.top;

	v[3].u = rcSrc.right;
	v[3].v = rcSrc.bottom;

	// �P��`�S���_�Ȃ̂�
	m_sVBInfo.buf_ptr = (UN_UINT8*)m_sVBInfo.buf_ptr + 4 * m_pVB->GetStride();
	m_sVBInfo.num += 4;
}

// �C���f�b�N�X�f�[�^�Z�b�g
void C2DRenderer::SetIdx()
{
	// NOTE
	// 0123 34 4567�E�E�E
	// 012-123-[233-334-344-445]-456-567-�E�E�E
	// 233 �̂悤�ɃC���f�b�N�X����A������ꍇ�́A�����Ȃ��O�p�`���ł���

	UN_ASSERT(m_pIB->GetFormat() == E_GRAPH_INDEX_BUFFER_FMT_INDEX16);

	UN_UINT16* pIdx = (UN_UINT16*)m_sIBInfo.buf_ptr;

	if (m_nCurIdx == 0) {
		// �ŏ��� 0, 1, 2, 3 �Œ�
		*pIdx++ = 0;
		*pIdx++ = 1;
		*pIdx++ = 2;
		*pIdx++ = 3;

		m_nCurIdx = 3;

		m_sIBInfo.buf_ptr = pIdx;
		m_sIBInfo.num += 4;
	}
	else {
		// ��`�Ƌ�`�̊Ԃ̌����Ȃ��O�p�`�����C���f�b�N�X
		*pIdx++ = m_nCurIdx++;
		*pIdx++ = m_nCurIdx;

		// ���̋�`
		*pIdx++ = m_nCurIdx++;
		*pIdx++ = m_nCurIdx++;
		*pIdx++ = m_nCurIdx++;
		*pIdx++ = m_nCurIdx;

		m_sIBInfo.buf_ptr = pIdx;
		m_sIBInfo.num += 6;
	}
}

// ���\�[�X���Z�b�g
void C2DRenderer::ResetResource()
{
	UN_ASSERT(m_pVB != UN_NULL);
	UN_ASSERT(m_pIB != UN_NULL);

	m_pVB->ReleaseResource();
	m_pIB->ReleaseResource();
}

// ���Z�b�g
void C2DRenderer::Reset()
{
	UN_ASSERT(m_pVB != UN_NULL);
	UN_ASSERT(m_pIB != UN_NULL);

	m_pVB->Reset();
	m_pIB->Reset();
}
