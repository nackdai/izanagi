#include "debugutil/mesh/DebugMeshGrid.h"

using namespace uranus;

// インスタンス作成
CDebugMeshGrid* CDebugMeshGrid::CreateDebugMeshGrid(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_COLOR nColor,
	UN_UINT nGridNumX,
	UN_UINT nGridNumY,
	UN_FLOAT fGridSize)
{
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshGrid));
	VRETURN_VAL(pBuf != UN_NULL, UN_NULL);

	CDebugMeshGrid* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// 強制
	UN_UINT flag = (E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR);

	// インスタンス作成
	pInstance = CreateMesh<CDebugMeshGrid>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != UN_NULL), __EXIT__);

	// 初期化
	result = pInstance->Init(
				pDevice,
				flag,
				nGridNumX,
				nGridNumY);
	VGOTO(result, __EXIT__);

	// 頂点データセット
	result = pInstance->SetVtx(
				flag,
				nColor,
				nGridNumX,
				nGridNumY,
				fGridSize);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

// 初期化
UN_BOOL CDebugMeshGrid::Init(
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_UINT nGridNumX,
	UN_UINT nGridNumY)
{
	UN_UINT nVtxNum = ((nGridNumX + 1) + (nGridNumY + 1)) * 2;

	VRETURN(CreateVB(flag, nVtxNum));
	VRETURN(CreateVD(flag));

	m_PrimType = E_GRAPH_PRIM_TYPE_LINELIST;
	m_nPrimCnt = (nGridNumX + 1) + (nGridNumY + 1);

	return UN_TRUE;
}

// 頂点データセット
UN_BOOL CDebugMeshGrid::SetVtx(
	UN_UINT flag,
	UN_COLOR nColor,
	UN_UINT nGridNumX,
	UN_UINT nGridNumY,
	UN_FLOAT fGridSize)
{
	UN_ASSERT(flag == (E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR));

	UN_UINT8* pVtx = UN_NULL;
	m_pVB->Lock(0, 0, (void**)&pVtx, UN_FALSE);

	UN_FLOAT fLeft = -0.5f * fGridSize * nGridNumX;
	UN_FLOAT fTop = -0.5f * fGridSize * nGridNumY;

	UN_FLOAT fWidth = nGridNumX * fGridSize;
	UN_FLOAT fHeight = nGridNumY * fGridSize;

	// 横
	for (UN_UINT y = 0; y <= nGridNumY; y++) {
		{
			UN_FLOAT* pos = (UN_FLOAT*)pVtx;

			pos[0] = fLeft;
			pos[1] = 0.0f;
			pos[2] = fTop + y * fGridSize;
			pos[3] = 1.0f;

			pVtx += GetPosSize(flag);

			UN_COLOR* color = (UN_COLOR*)pVtx;
			*color = nColor;
			pVtx += GetColorSize(flag);
		}

		{
			UN_FLOAT* pos = (UN_FLOAT*)pVtx;

			pos[0] = fLeft + fWidth;
			pos[1] = 0.0f;
			pos[2] = fTop + y * fGridSize;
			pos[3] = 1.0f;

			pVtx += GetPosSize(flag);

			UN_COLOR* color = (UN_COLOR*)pVtx;
			*color = nColor;
			pVtx += GetColorSize(flag);
		}
	}

	// 縦
	for (UN_UINT x = 0; x <= nGridNumX; x++) {
		{
			UN_FLOAT* pos = (UN_FLOAT*)pVtx;

			pos[0] = fLeft + x * fGridSize;
			pos[1] = 0.0f;
			pos[2] = fTop;
			pos[3] = 1.0f;

			pVtx += GetPosSize(flag);

			UN_COLOR* color = (UN_COLOR*)pVtx;
			*color = nColor;
			pVtx += GetColorSize(flag);
		}

		{
			UN_FLOAT* pos = (UN_FLOAT*)pVtx;

			pos[0] = fLeft + x * fGridSize;
			pos[1] = 0.0f;
			pos[2] = fTop + fHeight;
			pos[3] = 1.0f;

			pVtx += GetPosSize(flag);

			UN_COLOR* color = (UN_COLOR*)pVtx;
			*color = nColor;
			pVtx += GetColorSize(flag);
		}
	}

	m_pVB->Unlock();

	return UN_TRUE;
}
