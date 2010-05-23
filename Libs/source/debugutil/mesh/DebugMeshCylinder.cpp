#include "debugutil/mesh/DebugMeshCylinder.h"

using namespace uranus;

// �C���X�^���X�쐬
CDebugMeshCylinder* CDebugMeshCylinder::CreateDebugMeshCylinder(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fRadius,
	UN_FLOAT fHeight,
	UN_UINT nSlices,
	UN_UINT nStacks)
{
	flag |= E_DEBUG_MESH_VTX_FORM_POS;

	nSlices = MAX(3, nSlices);
	nStacks = MAX(2, nStacks);

	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshCylinder));
	VRETURN_VAL(pBuf != UN_NULL, UN_NULL);

	CDebugMeshCylinder* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// �C���X�^���X�쐬
	pInstance = CreateMesh<CDebugMeshCylinder>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != UN_NULL), __EXIT__);

	// ������
	result = pInstance->Init(
				pDevice,
				flag,
				nSlices,
				nStacks);
	VGOTO(result, __EXIT__);

	// ���_�f�[�^�Z�b�g
	result = pInstance->SetVtx(
				flag,
				nColor,
				fRadius,
				fHeight,
				nSlices,
				nStacks);
	VGOTO(result, __EXIT__);

	// �C���f�b�N�X�f�[�^�Z�b�g
	result = pInstance->SetIdx(
				nSlices,
				nStacks);
	VGOTO(result, __EXIT__);

	// �f�[�^��VB�AIB�ɃR�s�[����
	result = pInstance->CopyDataToBuffer(flag);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

namespace {
	// ���_���v�Z
	inline UN_UINT _ComputeVtxNum(UN_UINT nSlices, UN_UINT nStacks)
	{
		// +nSlices * 2 -> ��[�A���[�̂Q�_
		UN_UINT ret = (nSlices * 2) + (nSlices + 1) * nStacks;
		return ret;
	}

	// �C���f�b�N�X���v�Z
	inline UN_UINT _ComputeIdxNum(UN_UINT nSlices, UN_UINT nStacks)
	{
		// �㉺
		UN_UINT ret = (nSlices * 3) * 2;

		// �^��
		ret += (nSlices * 3 * 2) * (nStacks - 1);

		return ret;
	}
}	// namespace

// ������
UN_BOOL CDebugMeshCylinder::Init(
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_UINT nSlices,
	UN_UINT nStacks)
{
	UN_UINT nVtxNum = _ComputeVtxNum(nSlices, nStacks);
	UN_UINT nIdxNum = _ComputeIdxNum(nSlices, nStacks);

#if 0
	E_GRAPH_INDEX_BUFFER_FMT fmt = (nIdxNum > UN_UINT16_MAX 
									? E_GRAPH_INDEX_BUFFER_FMT_INDEX32 
									: E_GRAPH_INDEX_BUFFER_FMT_INDEX16);
#else
	E_GRAPH_INDEX_BUFFER_FMT fmt = E_GRAPH_INDEX_BUFFER_FMT_INDEX32;
#endif

	VRETURN(
		CreateVB(
			flag, 
			nVtxNum));

	VRETURN(
		CreateIB(
			nIdxNum, 
			fmt));

	VRETURN(CreateVD(flag));

	m_PrimType = E_GRAPH_PRIM_TYPE_TRIANGLELIST;
	m_nPrimCnt = nIdxNum / 3;

	VRETURN(
		CreateDataBuffer(
			nVtxNum,
			nIdxNum));

	VRETURN(
		CreateDebugAxis(
			nVtxNum,
			flag));

	return UN_TRUE;
}

// ���_�f�[�^�Z�b�g
UN_BOOL CDebugMeshCylinder::SetVtx(
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fRadius,
	UN_FLOAT fHeight,
	UN_UINT nSlices,
	UN_UINT nStacks)
{
	SMeshVtx* pVtx = GetVtx();

	// NOTE
	// �������ɉ�����̂́A�~���̒��S�����_�ɂ���悤�ɂ��邽��

	// ���S�_�i��j
	for (UN_UINT i = 0; i < nSlices; i++) {
		ComputeVtx(
			pVtx,
			flag,
			nColor,
			0.0f,
			fHeight,
			0.5f * fHeight,
			UN_MATH_PI2 * i / (nSlices -1));

		if (IsNormal(flag)) {
			pVtx->nml.Set(0.0f, 1.0f, 0.0f, 0.0f);
		}

		pVtx++;
	}

	UN_FLOAT fLongitudeStep = UN_MATH_PI2 / nSlices;	// �o�x�X�e�b�v
	UN_FLOAT fHeightStep = fHeight / (nStacks - 1);

	// �^��
	for (UN_UINT stack = 0; stack < nStacks; stack++) {
		UN_FLOAT fY = fHeight * 0.5f - fHeightStep * stack;

		for (UN_UINT slice = 0; slice < nSlices; slice++) {
			UN_FLOAT fLongitude = fLongitudeStep * slice;

			ComputeVtx(
				pVtx,
				flag,
				nColor,
				fRadius,
				fHeight,
				fY,
				fLongitude);
			pVtx++;
		}

#if 1
		// �Ō�ɂЂƂd�˂�
		{
			UN_FLOAT fLongitude = 0.0f;

			ComputeVtx(
				pVtx,
				flag & (~E_DEBUG_MESH_VTX_FORM_UV),	// UV���Z�b�g�����Ȃ�
				nColor,
				fRadius,
				fHeight,
				fY,
				fLongitude);

			// ������UV���W���Z�b�g����
			if (IsUV(flag)) {
				pVtx->uv[0] = 1.0f;
				pVtx->uv[1] = (fHeight - (fY + fHeight * 0.5f)) / fHeight;
			}

			pVtx++;
		}
#endif
	}

	// ���S�_�i���j
	for (UN_UINT i = 0; i < nSlices; i++) {
		ComputeVtx(
			pVtx,
			flag,
			nColor,
			0.0f,
			fHeight,
			-0.5f * fHeight,
			UN_MATH_PI2 * i / (nSlices -1));

		if (IsNormal(flag)) {
			pVtx->nml.Set(0.0f, -1.0f, 0.0f, 0.0f);
		}

		pVtx++;
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

	return UN_TRUE;
}

// ���_�f�[�^�v�Z
void CDebugMeshCylinder::ComputeVtx(
	SMeshVtx* pVtx,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fRadius,
	UN_FLOAT fHeight,
	UN_FLOAT fY,
	UN_FLOAT fLongitude)	// �o�x
{
	UN_FLOAT fSinLong = CMath::SinF(fLongitude);
	UN_FLOAT fCosLong = CMath::CosF(fLongitude);

	// �ʒu
	if (IsPos(flag)) {
		pVtx->pos.v[0] = fRadius * fCosLong;
		pVtx->pos.v[1] = fY;
		pVtx->pos.v[2] = fRadius * fSinLong;
		pVtx->pos.v[3] = 1.0f;
	}

	// �@��
	if (IsNormal(flag)) {
		pVtx->nml.v[0] = fCosLong;
		pVtx->nml.v[1] = 0.0f;
		pVtx->nml.v[2] = fSinLong;
		pVtx->nml.v[3] = 0.0f;
	}

	// ���_�J���[
	if (IsColor(flag)) {
		pVtx->clr = nColor;
	}

	// UV���W
	if (IsUV(flag)) {
		pVtx->uv[0] = fLongitude / UN_MATH_PI2;

		// NOTE
		// �������ɉ������Ă��邽�� * 0.5f �Ōv�Z����
		pVtx->uv[1] = (fHeight - (fY + fHeight * 0.5f)) / fHeight;
	}
}

// �C���f�b�N�X�f�[�^�Z�b�g
UN_BOOL CDebugMeshCylinder::SetIdx(
	UN_UINT nSlices,
	UN_UINT nStacks)
{
	// ���_��
	const UN_UINT nVtxNum = GetVtxNum();

	SMeshFace* pFace = GetFace();
	VRETURN_NULL(pFace);

	// ��
	for (UN_UINT slice = 0; slice < nSlices; slice++) {
		pFace->idx[0] = slice;	// ��ԏ�̓_
		pFace->idx[1] = slice + nSlices;
		pFace->idx[2] = slice + 1 + nSlices;

		if (slice + 1 >= nSlices) {
			SetOverlapVtx(
				(slice + 1) % nSlices + nSlices,
				slice + 1 + nSlices);
		}
		
		BindFaceToVtx(pFace);
		pFace++;
	}

	// �^��
	// ex) nSlices = 4 �̏ꍇ
	//	1___2___3___4___1
	//	|  /|  /|  /|  /|
	//	| / | / | / | / |
	//	|/__|/__|/__|/__|
	//  5   6   7   8   5
	//
	//  1-5-2 / 2-5-6
	//  2-6-3 / 3-6-7

	for (UN_UINT stack = 0; stack < nStacks - 1; stack++) {
		for (UN_UINT slice = 0; slice < nSlices; slice++) {
			{
				pFace->idx[0] = slice + stack * (nSlices + 1) + nSlices;
				pFace->idx[1] = slice + (stack + 1) * (nSlices + 1) + nSlices;
				pFace->idx[2] = (slice + 1) + stack * (nSlices + 1) + nSlices;

				if (slice + 1 >= nSlices) {
					SetOverlapVtx(
						(slice + 1) % nSlices + stack * (nSlices + 1) + nSlices,
						(slice + 1) + stack * (nSlices + 1) + nSlices);
				}

				BindFaceToVtx(pFace);
				pFace++;
			}

			{
				pFace->idx[0] = (slice + 1) + stack * (nSlices + 1) + nSlices;
				pFace->idx[1] = slice + (stack + 1) * (nSlices + 1) + nSlices;
				pFace->idx[2] = (slice + 1) + (stack + 1) * (nSlices + 1) + nSlices;

				if (slice + 1 >= nSlices) {
					SetOverlapVtx(
						(slice + 1) % nSlices + (stack + 1) * (nSlices + 1) + nSlices,
						(slice + 1) + (stack + 1) * (nSlices + 1) + nSlices);
				}

				BindFaceToVtx(pFace);
				pFace++;
			}
		}
	}

	// ��
	for (UN_UINT slice = 0; slice < nSlices; slice++) {
		pFace->idx[0] = nVtxNum - nSlices + slice;	// ��ԉ��̓_
		pFace->idx[1] = nVtxNum - nSlices - ((nSlices + 1) - (slice + 1));
		pFace->idx[2] = nVtxNum - nSlices - ((nSlices + 1) - slice);

		if (slice + 1 >= nSlices) {
			SetOverlapVtx(
				nVtxNum - nSlices - ((nSlices + 1) - (slice + 1) % nSlices),
				nVtxNum - nSlices - ((nSlices + 1) - (slice + 1)));
		}

		BindFaceToVtx(pFace);
		pFace++;
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetFace(), pFace) == m_nPrimCnt);

	return UN_TRUE;
}
