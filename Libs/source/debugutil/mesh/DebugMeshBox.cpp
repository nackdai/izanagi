#include "debugutil/mesh/DebugMeshBox.h"

using namespace uranus;

namespace {

	// NOTE
	// 0---2 4---6    |+
	// |   | |   |  --+-->x
	// 1---3 5---7    |-
	//                z

	// 位置
	const UN_FLOAT POS[][3] = {
		{-0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f, -0.5f},
		{ 0.5f,  0.5f,  0.5f},
		{ 0.5f,  0.5f, -0.5f},

		{-0.5f, -0.5f,  0.5f},
		{-0.5f, -0.5f, -0.5f},
		{ 0.5f, -0.5f,  0.5f},
		{ 0.5f, -0.5f, -0.5f},
	};

	// 法線
	const UN_FLOAT NML[][3] = {
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{-1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
	};

	// UV
	const UN_FLOAT UV[][2] = {
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
	};

	enum {
		PRIM_NUM = 6,
		VTX_NUM_PER_FACE = 4,
		IDX_NUM_PER_FACE = 6,
	};

	// 面
	const UN_UINT16 PRIM[PRIM_NUM][IDX_NUM_PER_FACE] = {
		{0, 1, 2, 3},
		{2, 6, 0, 4},
		{3, 7, 2, 6},
		{1, 5, 3, 7},
		{0, 4, 1, 5},
		{5, 4, 7, 6},
	};
}	// namespace

// インスタンス作成
CDebugMeshBox* CDebugMeshBox::CreateDebugMeshBox(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fWidth,
	UN_FLOAT fHeight,
	UN_FLOAT fDepth)
{
	flag |= E_DEBUG_MESH_VTX_FORM_POS;

	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshBox));
	VRETURN_VAL(pBuf != UN_NULL, UN_NULL);

	CDebugMeshBox* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// インスタンス作成
	pInstance = CreateMesh<CDebugMeshBox>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != UN_NULL), __EXIT__);

	{
		pInstance->m_PrimType = E_GRAPH_PRIM_TYPE_TRIANGLELIST;
		pInstance->m_nPrimCnt = PRIM_NUM * 2;
	}

	result = pInstance->Init(pDevice, flag);
	VGOTO(result, __EXIT__);

	// 頂点データセット
	result = pInstance->SetData(
				pDevice,
				flag,
				nColor,
				fWidth,
				fHeight,
				fDepth);
	VGOTO(result, __EXIT__);

	// インデックスデータセット
	result = pInstance->SetIdx();
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
UN_BOOL CDebugMeshBox::Init(
	CGraphicsDevice* pDevice,
	UN_UINT flag)
{
	UN_UINT nVtxNum = PRIM_NUM * VTX_NUM_PER_FACE;
	UN_UINT nIdxNum = PRIM_NUM * IDX_NUM_PER_FACE;

	VRETURN(
		CreateVB(
			flag, 
			nVtxNum));

	VRETURN(
		CreateIB(
			nIdxNum, 
			E_GRAPH_INDEX_BUFFER_FMT_INDEX32));

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

// データセット
UN_BOOL CDebugMeshBox::SetData(
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_COLOR nColor,
	UN_FLOAT fWidth,
	UN_FLOAT fHeight,
	UN_FLOAT fDepth)
{
	SMeshVtx* pVtx = GetVtx();

	for (UN_UINT i = 0; i < PRIM_NUM; i++) {
		for (UN_UINT n = 0; n < VTX_NUM_PER_FACE; n++) {
			// 位置
			if (IsPos(flag)) {
				UN_UINT idx = PRIM[i][n];

				pVtx->pos.v[0] = fWidth * POS[idx][0];
				pVtx->pos.v[1] = fHeight * POS[idx][1];
				pVtx->pos.v[2] = fDepth * POS[idx][2];
				pVtx->pos.v[3] = 1.0f;
			}

			// 法線
			if (IsNormal(flag)) {
				pVtx->nml.v[0] = NML[i][0];
				pVtx->nml.v[1] = NML[i][1];
				pVtx->nml.v[2] = NML[i][2];
				pVtx->nml.v[3] = 0.0f;
			}

			// カラー
			if (IsColor(flag)) {
				pVtx->clr = nColor;
			}
			
			// UV
			if (IsUV(flag)) {
				pVtx->uv[0] = UV[n][0];
				pVtx->uv[1] = UV[n][1];
			}

			pVtx++;
		}
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

	return UN_TRUE;
}

// インデックスデータセット
UN_BOOL CDebugMeshBox::SetIdx()
{
	SMeshFace* pFace = GetFace();

	UN_UINT32 nCurIdx = 0;

	for (UN_UINT i = 0; i < PRIM_NUM; i++) {
		{
			pFace->idx[0] = nCurIdx + 0;
			pFace->idx[1] = nCurIdx + 1;
			pFace->idx[2] = nCurIdx + 2;

			BindFaceToVtx(pFace);
			pFace++;
		}
		{
			pFace->idx[0] = nCurIdx + 2;
			pFace->idx[1] = nCurIdx + 1;
			pFace->idx[2] = nCurIdx + 3;

			BindFaceToVtx(pFace);
			pFace++;
		}

		nCurIdx += 4;
	}

	UN_ASSERT(CStdUtil::GetPtrAdvanced(GetFace(), pFace) == m_nPrimCnt);

	return UN_TRUE;
}
