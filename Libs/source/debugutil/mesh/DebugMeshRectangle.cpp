#include "debugutil/mesh/DebugMeshRectangle.h"

using namespace uranus;

// インスタンス作成
CDebugMeshRectangle* CDebugMeshRectangle::CreateDebugMeshRectangle(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_UINT nDivideX,
	UN_UINT nDivideY,
	UN_FLOAT fWidth,
	UN_FLOAT fHeight)
{
	flag |= E_DEBUG_MESH_VTX_FORM_POS;

	nDivideX = MAX(nDivideX, 1);
	nDivideY = MAX(nDivideY, 1);

	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshRectangle));
	VRETURN_VAL(pBuf != UN_NULL, UN_NULL);

	CDebugMeshRectangle* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// インスタンス作成
	pInstance = CreateMesh<CDebugMeshRectangle>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != UN_NULL), __EXIT__);

	// 初期化
	result = pInstance->Init(
				pDevice,
				flag,
				nDivideX,
				nDivideY,
				fWidth,
				fHeight);
	VGOTO(result, __EXIT__);

	// 頂点データセット
	result = pInstance->SetVtx(
				flag,
				nColor,
				nDivideX,
				nDivideY,
				fWidth,
				fHeight);
	VGOTO(result, __EXIT__);

	// インデックスデータセット
	result = pInstance->SetIdx(
				nDivideX,
				nDivideY);
	VGOTO(result, __EXIT__);

	// データをVB、IBにコピーする
	result = pInstance->CopyDataToBuffer(flag);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

// 初期化
UN_BOOL CDebugMeshRectangle::Init(
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_UINT nDivideX,
	UN_UINT nDivideY,
	UN_FLOAT fWidth,
	UN_FLOAT fHeight)
{
	UN_UINT nRectNum = nDivideX * nDivideY;

	UN_UINT nVtxNum = (nDivideX + 1) * (nDivideY + 1);
	UN_UINT nIdxNum = nRectNum * 6;

	VRETURN(CreateVB(flag, nVtxNum));
	VRETURN(CreateIB(nIdxNum, E_GRAPH_INDEX_BUFFER_FMT_INDEX32));
	VRETURN(CreateVD(flag));

	m_PrimType = E_GRAPH_PRIM_TYPE_TRIANGLELIST;
	m_nPrimCnt = nRectNum * 2;

	VRETURN(CreateDataBuffer(nVtxNum, nIdxNum));

	VRETURN(CreateDebugAxis(nVtxNum, flag));

	return UN_TRUE;
}

// 頂点データセット
UN_BOOL CDebugMeshRectangle::SetVtx(
	UN_UINT flag,
	UN_COLOR nColor,
	UN_UINT nDivideX,
	UN_UINT nDivideY,
	UN_FLOAT fWidth,
	UN_FLOAT fHeight)
{
	SMeshVtx* pVtx = GetVtx();

	UN_FLOAT fDivX = 1.0f / nDivideX;
	UN_FLOAT fDivY = 1.0f / nDivideY;

	UN_FLOAT fWidthPerVtx = -fWidth * fDivX;
	UN_FLOAT fHeightPerVtx = fHeight * fDivY;

	UN_FLOAT fTexUPerVtx = 1.0f * fDivX;
	UN_FLOAT fTexVPerVtx = 1.0f * fDivY;

	UN_FLOAT fZ = -0.5f * fHeight;

	for (UN_UINT y = 0; y <= nDivideY; y++) {
		UN_FLOAT fX = 0.5f * fWidth;
		
		for (UN_UINT x = 0; x <= nDivideX; x++) {
			// 位置
			if (IsPos(flag)) {
				pVtx->pos.v[0] = fX;
				pVtx->pos.v[1] = 0.0f;
				pVtx->pos.v[2] = fZ;
				pVtx->pos.v[3] = 1.0f;
			}

			// 法線
			if (IsNormal(flag)) {
				pVtx->nml.v[0] = 0.0f;
				pVtx->nml.v[1] = 1.0f;
				pVtx->nml.v[2] = 0.0f;
				pVtx->nml.v[3] = 0.0f;
			}

			// 頂点カラー
			if (IsColor(flag)) {
				pVtx->clr = nColor;
			}

			// UV座標
			if (IsUV(flag)) {
				pVtx->uv[0] = fTexUPerVtx * x;
				pVtx->uv[1] = fTexVPerVtx * y;
			}

			pVtx++;

			fX += fWidthPerVtx;
		}

		fZ += fHeightPerVtx;
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

	return UN_TRUE;
}

// 面に頂点インデックスデータセット
UN_BOOL CDebugMeshRectangle::SetIdx(
	UN_UINT nDivideX,
	UN_UINT nDivideY)
{
	// 頂点数
	const UN_UINT nVtxNum = GetVtxNum();

	SMeshFace* pFace = GetFace();
	VRETURN_NULL(pFace);

	for (UN_UINT y = 0; y < nDivideY; y++) {
		UN_UINT nIdx = y * (nDivideX + 1);

		for (UN_UINT x = 0; x < nDivideX; x++) {
			{
				pFace->idx[0] = nIdx + x;
				pFace->idx[1] = nIdx + x + (nDivideX + 1);
				pFace->idx[2] = nIdx + (x + 1);

				BindFaceToVtx(pFace);
				pFace++;
			}

			{
				pFace->idx[0] = nIdx + (x + 1);
				pFace->idx[1] = nIdx + x + (nDivideX + 1);
				pFace->idx[2] = nIdx + (x + 1) + (nDivideX + 1);

				BindFaceToVtx(pFace);
				pFace++;
			}
		}
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetFace(), pFace) == m_nPrimCnt);

	return UN_TRUE;
}
