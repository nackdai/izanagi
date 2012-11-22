#include "debugutil/mesh/DebugMeshRectangle.h"

using namespace izanagi;

// インスタンス作成
CDebugMeshRectangle* CDebugMeshRectangle::CreateDebugMeshRectangle(
	IMemoryAllocator* pAllocator,
	graph::CGraphicsDevice* pDevice,
	IZ_UINT flag,
	IZ_COLOR nColor,
	IZ_UINT nDivideX,
	IZ_UINT nDivideY,
	IZ_FLOAT fWidth,
	IZ_FLOAT fHeight)
{
	flag |= E_DEBUG_MESH_VTX_FORM_POS;

	nDivideX = IZ_MAX(nDivideX, 1);
	nDivideY = IZ_MAX(nDivideY, 1);

	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshRectangle));
	VRETURN_VAL(pBuf != IZ_NULL, IZ_NULL);

	CDebugMeshRectangle* pInstance = IZ_NULL;
	IZ_BOOL result = IZ_FALSE;

	// インスタンス作成
	pInstance = CreateMesh<CDebugMeshRectangle>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != IZ_NULL), __EXIT__);

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
IZ_BOOL CDebugMeshRectangle::Init(
	graph::CGraphicsDevice* pDevice,
	IZ_UINT flag,
	IZ_UINT nDivideX,
	IZ_UINT nDivideY,
	IZ_FLOAT fWidth,
	IZ_FLOAT fHeight)
{
	IZ_UINT nRectNum = nDivideX * nDivideY;

	IZ_UINT nVtxNum = (nDivideX + 1) * (nDivideY + 1);
	IZ_UINT nIdxNum = nRectNum * 6;

	VRETURN(CreateVB(flag, nVtxNum));
	VRETURN(CreateIB(nIdxNum, graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32));
	VRETURN(CreateVD(flag));

	m_PrimType = graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST;
	m_nPrimCnt = nRectNum * 2;

	VRETURN(CreateDataBuffer(nVtxNum, nIdxNum));

	VRETURN(CreateDebugAxis(nVtxNum, flag));

	return IZ_TRUE;
}

// 頂点データセット
IZ_BOOL CDebugMeshRectangle::SetVtx(
	IZ_UINT flag,
	IZ_COLOR nColor,
	IZ_UINT nDivideX,
	IZ_UINT nDivideY,
	IZ_FLOAT fWidth,
	IZ_FLOAT fHeight)
{
	SMeshVtx* pVtx = GetVtx();

	IZ_FLOAT fDivX = 1.0f / nDivideX;
	IZ_FLOAT fDivY = 1.0f / nDivideY;

	IZ_FLOAT fWidthPerVtx = fWidth * fDivX;

	IZ_FLOAT fTexUPerVtx = 1.0f * fDivX;
	IZ_FLOAT fTexVPerVtx = 1.0f * fDivY;

#ifdef IZ_COORD_LEFT_HAND
	IZ_FLOAT fZ = 0.5f * fHeight;
	IZ_FLOAT fHeightPerVtx = -fHeight * fDivY;
#else
	IZ_FLOAT fZ = -0.5f * fHeight;
	IZ_FLOAT fHeightPerVtx = fHeight * fDivY;
#endif

	for (IZ_UINT y = 0; y <= nDivideY; ++y) {
		IZ_FLOAT fX = -0.5f * fWidth;
		
		for (IZ_UINT x = 0; x <= nDivideX; ++x) {
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

			++pVtx;

			fX += fWidthPerVtx;
		}

		fZ += fHeightPerVtx;
	}

	IZ_ASSERT(CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

	return IZ_TRUE;
}

// 面に頂点インデックスデータセット
IZ_BOOL CDebugMeshRectangle::SetIdx(
	IZ_UINT nDivideX,
	IZ_UINT nDivideY)
{
	// 頂点数
	const IZ_UINT nVtxNum = GetVtxNum();

	SMeshFace* pFace = GetFace();
	VRETURN_NULL(pFace);

	for (IZ_UINT y = 0; y < nDivideY; ++y) {
		IZ_UINT nIdx = y * (nDivideX + 1);

		for (IZ_UINT x = 0; x < nDivideX; ++x) {
			{
				pFace->idx[0] = nIdx + x;
				pFace->idx[1] = nIdx + x + (nDivideX + 1);
				pFace->idx[2] = nIdx + (x + 1);

				BindFaceToVtx(pFace);
				++pFace;
			}

			{
				pFace->idx[0] = nIdx + (x + 1);
				pFace->idx[1] = nIdx + x + (nDivideX + 1);
				pFace->idx[2] = nIdx + (x + 1) + (nDivideX + 1);

				BindFaceToVtx(pFace);
				++pFace;
			}
		}
	}

	IZ_ASSERT(CStdUtil::GetPtrAdvanced(GetFace(), pFace) == m_nPrimCnt);

	return IZ_TRUE;
}
