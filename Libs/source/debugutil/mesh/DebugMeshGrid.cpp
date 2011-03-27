#include "debugutil/mesh/DebugMeshGrid.h"

using namespace izanagi;

// インスタンス作成
CDebugMeshGrid* CDebugMeshGrid::CreateDebugMeshGrid(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IZ_COLOR nColor,
	IZ_UINT nGridNumX,
	IZ_UINT nGridNumY,
	IZ_FLOAT fGridSize)
{
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshGrid));
	VRETURN_VAL(pBuf != IZ_NULL, IZ_NULL);

	CDebugMeshGrid* pInstance = IZ_NULL;
	IZ_BOOL result = IZ_FALSE;

	// 強制
	IZ_UINT flag = (E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR);

	// インスタンス作成
	pInstance = CreateMesh<CDebugMeshGrid>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != IZ_NULL), __EXIT__);

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
IZ_BOOL CDebugMeshGrid::Init(
	CGraphicsDevice* pDevice,
	IZ_UINT flag,
	IZ_UINT nGridNumX,
	IZ_UINT nGridNumY)
{
	IZ_UINT nVtxNum = ((nGridNumX + 1) + (nGridNumY + 1)) * 2;

	VRETURN(CreateVB(flag, nVtxNum));
	VRETURN(CreateVD(flag));

	m_PrimType = E_GRAPH_PRIM_TYPE_LINELIST;
	m_nPrimCnt = (nGridNumX + 1) + (nGridNumY + 1);

	return IZ_TRUE;
}

// 頂点データセット
IZ_BOOL CDebugMeshGrid::SetVtx(
	IZ_UINT flag,
	IZ_COLOR nColor,
	IZ_UINT nGridNumX,
	IZ_UINT nGridNumY,
	IZ_FLOAT fGridSize)
{
	IZ_ASSERT(flag == (E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR));

	IZ_UINT8* pVtx = IZ_NULL;
	m_pVB->Lock(0, 0, (void**)&pVtx, IZ_FALSE);

	IZ_FLOAT fLeft = -0.5f * fGridSize * nGridNumX;
	IZ_FLOAT fTop = -0.5f * fGridSize * nGridNumY;

	IZ_FLOAT fWidth = nGridNumX * fGridSize;
	IZ_FLOAT fHeight = nGridNumY * fGridSize;

	// 横
	for (IZ_UINT y = 0; y <= nGridNumY; ++y) {
		{
			IZ_FLOAT* pos = (IZ_FLOAT*)pVtx;

			pos[0] = fLeft;
			pos[1] = 0.0f;
			pos[2] = fTop + y * fGridSize;
			pos[3] = 1.0f;

			pVtx += GetPosSize(flag);

			IZ_COLOR* color = (IZ_COLOR*)pVtx;
			*color = nColor;
			pVtx += GetColorSize(flag);
		}

		{
			IZ_FLOAT* pos = (IZ_FLOAT*)pVtx;

			pos[0] = fLeft + fWidth;
			pos[1] = 0.0f;
			pos[2] = fTop + y * fGridSize;
			pos[3] = 1.0f;

			pVtx += GetPosSize(flag);

			IZ_COLOR* color = (IZ_COLOR*)pVtx;
			*color = nColor;
			pVtx += GetColorSize(flag);
		}
	}

	// 縦
	for (IZ_UINT x = 0; x <= nGridNumX; ++x) {
		{
			IZ_FLOAT* pos = (IZ_FLOAT*)pVtx;

			pos[0] = fLeft + x * fGridSize;
			pos[1] = 0.0f;
			pos[2] = fTop;
			pos[3] = 1.0f;

			pVtx += GetPosSize(flag);

			IZ_COLOR* color = (IZ_COLOR*)pVtx;
			*color = nColor;
			pVtx += GetColorSize(flag);
		}

		{
			IZ_FLOAT* pos = (IZ_FLOAT*)pVtx;

			pos[0] = fLeft + x * fGridSize;
			pos[1] = 0.0f;
			pos[2] = fTop + fHeight;
			pos[3] = 1.0f;

			pVtx += GetPosSize(flag);

			IZ_COLOR* color = (IZ_COLOR*)pVtx;
			*color = nColor;
			pVtx += GetColorSize(flag);
		}
	}

	m_pVB->Unlock();

	return IZ_TRUE;
}
