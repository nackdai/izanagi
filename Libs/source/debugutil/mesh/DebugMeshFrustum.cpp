#include "debugutil/mesh/DebugMeshFrustum.h"

using namespace izanagi;

CDebugMeshFrustum* CDebugMeshFrustum::CreateDebugMeshFrustum(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IZ_COLOR nColor,
	IZ_FLOAT fAspect,
	IZ_FLOAT fFov,
	IZ_FLOAT fNear, IZ_FLOAT fFar)
{
	IZ_ASSERT(pAllocator != IZ_NULL);
	IZ_ASSERT(pDevice != IZ_NULL);

	// Allocate memory.
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshFrustum));
	VRETURN_NULL(pBuf != IZ_NULL);

	// Create instance.
	CDebugMeshFrustum* pInstance = CreateMesh<CDebugMeshFrustum>(pBuf, pAllocator, pDevice, VTX_FORM_FLAG);
	VRETURN_NULL(pInstance != IZ_NULL);

	IZ_BOOL result = IZ_FALSE;

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

IZ_BOOL CDebugMeshFrustum::Init()
{
	IZ_ASSERT(m_pDevice != IZ_NULL);

	VRETURN(
		CreateVB(
			VTX_FORM_FLAG, 
			VTX_NUM));

	VRETURN(CreateVD(VTX_FORM_FLAG));

	m_PrimType = E_GRAPH_PRIM_TYPE_TRIANGLELIST;
	m_nPrimCnt = 4 + 2;

	return IZ_TRUE;
}

IZ_BOOL CDebugMeshFrustum::SetVtx(
	IZ_COLOR nColor,
	IZ_FLOAT fAspect,
	IZ_FLOAT fFov,
	IZ_FLOAT fNear, IZ_FLOAT fFar)
{
	IZ_FLOAT fTangent = CMath::TanF(fFov * 0.5f);

	IZ_FLOAT fWidth = 2.0f * fTangent * fFar;
	IZ_FLOAT fHeight = fWidth / fAspect;

	IZ_FLOAT fW = fWidth * 0.5f;
	IZ_FLOAT fH = fHeight * 0.5f;

	const IZ_FLOAT FrustumPos[][3][3] = {
#if 0
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
#else
		{
			// Left
			{0.0f, 0.0f, 0.0f},
			{fW,  fH, fFar},
			{fW, -fH, fFar},
		},
		{
			// Top
			{0.0f, 0.0f, 0.0f},
			{-fW, fH, fFar},
			{ fW, fH, fFar},
		},
		{
			// Right
			{0.0f, 0.0f, 0.0f},
			{-fW, -fH, fFar},
			{-fW,  fH, fFar},
		},
		{
			// Bottom
			{0.0f, 0.0f, 0.0f},
			{ fW, -fH, fFar},
			{-fW, -fH, fFar},
		},
		{
			// Far_0
			{ fW,  fH, fFar},
			{ fW, -fH, fFar},
			{-fW,  fH, fFar},
		},
		{
			// Far_1
			{-fW,  fH, fFar},
			{ fW, -fH, fFar},
			{-fW, -fH, fFar},
		}
#endif
	};

	// Lock vertex buffer.
	IZ_UINT8* pData = IZ_NULL;
	VRETURN(LockVB(reinterpret_cast<void**>(&pData)));

	SMeshVtx sVtx;

	for (IZ_UINT i = 0; i < m_nPrimCnt; ++i) {
		for (IZ_UINT n = 0; n < 3; ++n) {
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

	return IZ_TRUE;
}
