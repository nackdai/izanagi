#include "debugutil/mesh/DebugMeshTorus.h"

using namespace uranus;

// �C���X�^���X�쐬
CDebugMeshTorus* CDebugMeshTorus::CreateDebugMeshTorus(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fInnerRadius,	// ���a
	UN_FLOAT fOuterRadius,	// �O�a
	UN_UINT nSides,			// ���f�ʂ̕ӂ̐�
	UN_UINT nRings)			// �g�[���X�Ɋ܂܂�郊���O�̐�
{
	flag |= E_DEBUG_MESH_VTX_FORM_POS;

	UN_FLOAT tmp = MIN(fInnerRadius, fOuterRadius);
	fOuterRadius = MAX(fInnerRadius, fOuterRadius);
	fInnerRadius = tmp;

	nSides = MAX(3, nSides);
	nRings = MAX(3, nRings);

	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshTorus));
	VRETURN_VAL(pBuf != UN_NULL, UN_NULL);

	CDebugMeshTorus* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// �C���X�^���X�쐬
	pInstance = CreateMesh<CDebugMeshTorus>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != UN_NULL), __EXIT__);

	// ������
	result = pInstance->Init(
				pDevice,
				flag,
				nSides,
				nRings);
	VGOTO(result, __EXIT__);

	// ���_�f�[�^�Z�b�g
	result = pInstance->SetVtx(
				flag,
				nColor,
				fInnerRadius,
				fOuterRadius,
				nSides,
				nRings);
	VGOTO(result, __EXIT__);

	// �C���f�b�N�X�f�[�^�Z�b�g
	result = pInstance->SetIdx(
				nSides,
				nRings);
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
	inline UN_UINT _ComputeVtxNum(UN_UINT nSides, UN_UINT nRings)
	{
		// NOTE
		// �n�_�̏d�������l��
		// �P�����O������ nSides + 1 �̒��_
		UN_UINT ret = (nSides + 1) * (nRings + 1);
		return ret;
	}

	// �C���f�b�N�X���v�Z
	inline UN_UINT _ComputeIdxNum(UN_UINT nSides, UN_UINT nRings)
	{
		// NOTE
		// �n�_�̏d�������l��
		// �P�����O������ nSides �̋�`
		//  => �P�����O������ 2 * nSides �̎O�p�`
		//  => �P�����O������ 2 * 3 * nSides �̒��_
		UN_UINT ret = nRings * nSides * 6;
		return ret;
	}
}	// namespace

// ������
UN_BOOL CDebugMeshTorus::Init(
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_UINT nSides, 
	UN_UINT nRings)
{
	UN_ASSERT(pDevice != UN_NULL);

	nSides = MAX(3, nSides);
	nRings = MAX(3, nRings);

	UN_UINT nVtxNum = _ComputeVtxNum(nSides, nRings);
	UN_UINT nIdxNum = _ComputeIdxNum(nSides, nRings);

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

	VRETURN(
		CreateVD(flag));

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
UN_BOOL CDebugMeshTorus::SetVtx(
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fInnerRadius,	// ���a
	UN_FLOAT fOuterRadius,	// �O�a
	UN_UINT nSides,			// ���f�ʂ̕ӂ̐�
	UN_UINT nRings)			// �g�[���X�Ɋ܂܂�郊���O�̐�
{
	UN_ASSERT(fOuterRadius > fInnerRadius);

	SMeshVtx* pVtx = GetVtx();

	UN_FLOAT fRingRadius = fOuterRadius - fInnerRadius;		// �����O�̔��a
	UN_FLOAT fCenterRadius = fInnerRadius + fRingRadius;	// �����O���S�܂ł̌a

	UN_FLOAT fLatitudeStep = UN_MATH_PI2 / nSides;		// �ܓx�X�e�b�v
	UN_FLOAT fLongitudeStep = UN_MATH_PI2 / nRings;	// �o�x�X�e�b�v

	for (UN_UINT ring = 0; ring <= nRings; ring++) {
		UN_FLOAT fLongitude = fLongitudeStep * ring;

		for (UN_UINT side = 0; side <= nSides; side++) {
			UN_FLOAT fLatitude = fLatitudeStep * side;

			// ���_�f�[�^�Z�b�g
			ComputeVtx(
				pVtx,
				flag,
				nColor,
				fCenterRadius,
				fRingRadius,
				fLongitude,
				fLatitude);
			pVtx++;
		}
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

	return UN_TRUE;
}

// ���_�f�[�^�Z�b�g
void CDebugMeshTorus::ComputeVtx(
	SMeshVtx* pVtx,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fCenterRadius,	// �����O���S�܂ł̌a
	UN_FLOAT fRingRadius,	// �����O�̔��a
	UN_FLOAT fLongitude,	// �o�x
	UN_FLOAT fLatitude)		// �ܓx
{
	UN_FLOAT fSinLat = CMath::SinF(fLatitude);
	UN_FLOAT fCosLat = CMath::CosF(fLatitude);
	UN_FLOAT fSinLong = CMath::SinF(fLongitude);
	UN_FLOAT fCosLong = CMath::CosF(fLongitude);

	// ���S�ʒu
	SVector vCenter;
	vCenter.Set(
		fCenterRadius * fCosLong,
		0.0f,
		fCenterRadius * fSinLong);

	// �ʒu
	SVector vPos;
	vPos.Set(
		fRingRadius * fCosLat,
		fRingRadius * fSinLat,
		0.0f);
	vPos.Set(
		vPos.x + fCenterRadius,
		vPos.y,
		vPos.z);
	vPos.Set(
		vPos.x * fCosLong,
		vPos.y,
		vPos.x * fSinLong);

	// �ʒu
	if (IsPos(flag)) {
		pVtx->pos.v[0] = vPos.x;
		pVtx->pos.v[1] = vPos.y;
		pVtx->pos.v[2] = vPos.z;
		pVtx->pos.v[3] = 1.0f;
	}

	// �@��
	if (IsNormal(flag)) {
		SVector vNml;
		vNml.Set(
			vPos.x - vCenter.x,
			vPos.y - vCenter.y,
			vPos.z - vCenter.z);

		NormalizeVector(vNml, vNml);

		pVtx->nml.v[0] = vNml.x;
		pVtx->nml.v[1] = vNml.y;
		pVtx->nml.v[2] = vNml.z;
		pVtx->nml.v[3] = 0.0f;
	}

	// ���_�J���[
	if (IsColor(flag)) {
		pVtx->clr = nColor;
	}

	// UV���W
	if (IsUV(flag)) {
		// NOTE
		// MATH_PI2 �Ŋ��邱�Ƃ� 0.0f - 1.0f �ɐ��K��
#if 0
		pVtx->uv[0] = fLongitude / UN_MATH_PI2;
		pVtx->uv[1] = fLatitude / UN_MATH_PI2;
#else
		pVtx->uv[0] = fLongitude / UN_MATH_PI2;
		pVtx->uv[1] = (-fSinLat + 1.0f) * 0.5f;
#endif
	}
}

// �C���f�b�N�X�f�[�^�Z�b�g
UN_BOOL CDebugMeshTorus::SetIdx(
	UN_UINT nSides,		// ���f�ʂ̕ӂ̐�
	UN_UINT nRings)		// �g�[���X�Ɋ܂܂�郊���O�̐�
{
	// NOTE
	// �P�����O�ɂ��Ċ��L�����ꍇ
	// ex) nSides = 3 �̏ꍇ
	// 3--7
	// |\ |
	// | \|
	// 2--6
	// |\ | �E�E�E
	// | \|
	// 1--5
	// |\ |
	// | \|
	// 0--4
	//  0-4-1 4-5-1
	//  1-5-2 5-6-2

	// ���_��
	const UN_UINT nVtxNum = GetVtxNum();

	SMeshFace* pFace = GetFace();
	VRETURN_NULL(pFace);

	UN_UINT nOverlapThreshold = (nSides + 1) * nRings;

	for (UN_UINT ring = 0; ring < nRings; ring++) {
		UN_UINT nIdx = (nSides + 1) * ring;

		for (UN_UINT side = 0; side < nSides; side++) {
			{
				pFace->idx[0] = nIdx;
				pFace->idx[1] = nIdx + (nSides + 1);
				pFace->idx[2] = nIdx + 1;

				if (pFace->idx[1] >= nOverlapThreshold) {
					SetOverlapVtx(
						pFace->idx[1] % nOverlapThreshold,
						pFace->idx[1]);
				}

				BindFaceToVtx(pFace);
				pFace++;
			}

			{
				pFace->idx[0] = nIdx + (nSides + 1);
				pFace->idx[1] = nIdx + 1 + (nSides + 1);
				pFace->idx[2] = nIdx + 1;

				if (pFace->idx[0] >= nOverlapThreshold) {
					SetOverlapVtx(
						pFace->idx[0] % nOverlapThreshold,
						pFace->idx[0]);

					SetOverlapVtx(
						pFace->idx[1] % nOverlapThreshold,
						pFace->idx[1]);
				}

				BindFaceToVtx(pFace);
				pFace++;
			}

			nIdx++;
		}
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetFace(), pFace) == m_nPrimCnt);

	return UN_TRUE;
}
