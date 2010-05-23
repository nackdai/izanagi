#include "debugutil/mesh/DebugMeshFrustum.h"

using namespace uranus;

CDebugMeshFrustum* CDebugMeshFrustum::CreateDebugMeshFrustum(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_COLOR nColor,
	UN_FLOAT fAspect,
	UN_FLOAT fFov,
	UN_FLOAT fNear, UN_FLOAT fFar)
{
	UN_ASSERT(pAllocator != UN_NULL);
	UN_ASSERT(pDevice != UN_NULL);

	// Allocate memory.
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshFrustum));
	VRETURN_NULL(pBuf != UN_NULL);

	// Create instance.
	CDebugMeshFrustum* pInstance = CreateMesh<CDebugMeshFrustum>(pBuf, pAllocator, pDevice, VTX_FORM_FLAG);
	VRETURN_NULL(pInstance != UN_NULL);

	UN_BOOL result = UN_FALSE;

	// Initialize instance.
	result = pInstance->Init();
	VGOTO(result, __EXIT__);

	// Set vertex data.
	result = pInstance->SetVtx(
				nColor,
				fAspect,
				fFov,
				fNear, fFar);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

UN_BOOL CDebugMeshFrustum::Init()
{
	UN_ASSERT(m_pDevice != UN_NULL);

	VRETURN(
		CreateVB(
			VTX_FORM_FLAG, 
			VTX_NUM));

	VRETURN(CreateVD(VTX_FORM_FLAG));

	m_PrimType = E_GRAPH_PRIM_TYPE_TRIANGLELIST;
	m_nPrimCnt = 4 + 2;

	return UN_TRUE;
}

UN_BOOL CDebugMeshFrustum::SetVtx(
	UN_COLOR nColor,
	UN_FLOAT fAspect,
	UN_FLOAT fFov,
	UN_FLOAT fNear, UN_FLOAT fFar)
{
	UN_FLOAT fTangent = CMath::TanF(fFov * 0.5f);

	UN_FLOAT fWidth = 2.0f * fTangent * fFar;
	UN_FLOAT fHeight = fWidth / fAspect;

	UN_FLOAT fW = fWidth * 0.5f;
	UN_FLOAT fH = fHeight * 0.5f;

	const UN_FLOAT FrustumPos[][3][3] = {
		{
			// Left
			{0.0f, 0.0f, 0.0f},
			{fW, -fH, fFar},
			{fW,  fH, fFar},
		},
		{
			// Top
			{0.0f, 0.0f, 0.0f},
			{ fW, fH, fFar},
			{-fW, fH, fFar},
		},
		{
			// Right
			{0.0f, 0.0f, 0.0f},
			{-fW,  fH, fFar},
			{-fW, -fH, fFar},
		},
		{
			// Bottom
			{0.0f, 0.0f, 0.0f},
			{-fW, -fH, fFar},
			{ fW, -fH, fFar},
		},
		{
			// Far_0
			{ fW,  fH, fFar},
			{-fW,  fH, fFar},
			{ fW, -fH, fFar},
		},
		{
			// Far_1
			{-fW,  fH, fFar},
			{-fW, -fH, fFar},
			{ fW, -fH, fFar},
		}
	};

	// Lock vertex buffer.
	UN_UINT8* pData = UN_NULL;
	VRETURN(LockVB(reinterpret_cast<void**>(&pData)));

	SMeshVtx sVtx;

	for (UN_UINT i = 0; i < m_nPrimCnt; i++) {
		for (UN_UINT n = 0; n < 3; n++) {
			sVtx.pos.Set(
				FrustumPos[i][n][0],
				FrustumPos[i][n][1],
				FrustumPos[i][n][2]);
			sVtx.clr = nColor;

			pData = SetVtxData(
						sVtx,
						VTX_FORM_FLAG,
						pData);
		}
	}

	// Unlock vertex buffer.
	VRETURN(UnlockVB());

	return UN_TRUE;
}
