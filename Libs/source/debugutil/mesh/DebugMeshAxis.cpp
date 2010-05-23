#include "debugutil/mesh/DebugMeshAxis.h"
#include "math/MathVector.h"

using namespace uranus;

// インスタンス作成
CDebugMeshAxis* CDebugMeshAxis::CreateDebugMeshAxis(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	UN_UINT flag,
	UN_UINT nPointNum/*= 0*/)
{
	// 必ず１つはあるようにする・・・
	if (!(IsAxisX(flag) || IsAxisY(flag) || IsAxisZ(flag))) {
		flag |= E_DEBUG_MESH_AXIS_X;
	}

	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshAxis));
	VRETURN_VAL(pBuf != UN_NULL, UN_NULL);

	CDebugMeshAxis* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// インスタンス作成
	pInstance = CreateMesh<CDebugMeshAxis>(pBuf, pAllocator, pDevice, flag);
	VGOTO(result != (pInstance != UN_NULL), __EXIT__);

	{
		pInstance->m_PrimType = E_GRAPH_PRIM_TYPE_LINELIST;

		if (IsAxisX(flag)) {
			pInstance->m_nPrimCnt++;
		}
		if (IsAxisY(flag)) {
			pInstance->m_nPrimCnt++;
		}
		if (IsAxisZ(flag)) {
			pInstance->m_nPrimCnt++;
		}

		if (nPointNum > 0) {
			pInstance->m_nPrimCnt *= nPointNum;	
		}

		pInstance->m_nAxisFlag = flag;
	}

	result = pInstance->Init(
				pDevice,
				AXIS_VTX_FLAG);
	VGOTO(result, __EXIT__);

	// TODO

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

CDebugMeshAxis* CDebugMeshAxis::CreateDebugMeshAxisDefault(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice)
{
	CDebugMeshAxis* pInstance = CreateDebugMeshAxis(
									pAllocator,
									pDevice,
									uranus::E_DEBUG_MESH_AXIS_X | uranus::E_DEBUG_MESH_AXIS_Y | uranus::E_DEBUG_MESH_AXIS_Z);
	VRETURN(pInstance != UN_NULL);

	pInstance->BeginRegister();
	{
		pInstance->SetVtx(
			uranus::E_DEBUG_MESH_AXIS_X,
			uranus::CVector(0.0f, 0.0f, 0.0f),
			uranus::CVector(10.0f, 0.0f, 0.0f));
		pInstance->SetVtx(
			uranus::E_DEBUG_MESH_AXIS_Y,
			uranus::CVector(0.0f, 0.0f, 0.0f),
			uranus::CVector(0.0f, 10.0f, 0.0f));
		pInstance->SetVtx(
			uranus::E_DEBUG_MESH_AXIS_Z,
			uranus::CVector(0.0f, 0.0f, 0.0f),
			uranus::CVector(0.0f, 0.0f, 10.0f));
	}
	pInstance->EndRegister();

	return pInstance;
}

CDebugMeshAxis::~CDebugMeshAxis()
{
	if (m_pVtxDst != UN_NULL) {
		UN_ASSERT(UN_FALSE);
		EndRegister();
	}
}

// 初期化
UN_BOOL CDebugMeshAxis::Init(
	CGraphicsDevice* pDevice,
	UN_UINT flag)
{
	VRETURN(
		CreateVB(
			flag, 
			m_nPrimCnt * 2));	// １ライン２頂点

	VRETURN(CreateVD(flag));

	return UN_TRUE;
}

// 描画
UN_BOOL CDebugMeshAxis::Draw()
{
	UN_BOOL ret = (m_nSetPrimCnt == 0);

	if (!ret && (m_nSetPrimCnt == m_nPrimCnt)) {
		ret = CDebugMesh::Draw();
	}

	UN_ASSERT(ret);
	return ret;
}

UN_BOOL CDebugMeshAxis::BeginRegister()
{
	VRETURN(m_pVtxDst == UN_NULL);
	VRETURN(m_pVB->Lock(0, 0, (void**)&m_pVtxDst, UN_FALSE));
	m_nSetPrimCnt = 0;
	return UN_TRUE;
}

UN_BOOL CDebugMeshAxis::EndRegister()
{
	VRETURN(m_pVtxDst != UN_NULL);
	VRETURN(m_pVB->Unlock());
	m_pVtxDst = UN_NULL;
	return UN_TRUE;
}

UN_BOOL CDebugMeshAxis::SetVtx(
	UN_UINT type,
	const SVector& start,
	const SVector& end)
{
	VRETURN(m_pVtxDst != UN_NULL);

	// 設定されてない軸
	if ((m_nAxisFlag & type) == 0) {
		return UN_TRUE;
	}

	// 頂点数オーバー
	if (m_nSetPrimCnt >= m_nPrimCnt) {
		UN_ASSERT(UN_FALSE);
		return UN_FALSE;
	}

	m_nSetPrimCnt++;

	UN_UINT nColor = 0;
	switch (type) {
	case E_DEBUG_MESH_AXIS_X:
		nColor = UN_COLOR_RGBA(0xff, 0, 0, 0xff);
		break;
	case E_DEBUG_MESH_AXIS_Y:
		nColor = UN_COLOR_RGBA(0, 0xff, 0, 0xff);
		break;
	case E_DEBUG_MESH_AXIS_Z:
		nColor = UN_COLOR_RGBA(0, 0, 0xff, 0xff);
		break;
	default:
		nColor = UN_COLOR_RGBA(0xff, 0, 0, 0xff);
		break;
	}

	SMeshVtx SMeshVtx[2];

	// Position
	{
		SMeshVtx[0].pos.v[0] = start.v[0];
		SMeshVtx[0].pos.v[1] = start.v[1];
		SMeshVtx[0].pos.v[2] = start.v[2];
		SMeshVtx[0].pos.v[3] = 1.0f;
	}
	{
		SMeshVtx[1].pos.v[0] = start.v[0] + end.v[0];
		SMeshVtx[1].pos.v[1] = start.v[1] + end.v[1];
		SMeshVtx[1].pos.v[2] = start.v[2] + end.v[2];
		SMeshVtx[1].pos.v[3] = 1.0f;
	}

	// Color
	SMeshVtx[0].clr = nColor;
	SMeshVtx[1].clr = nColor;

	m_pVtxDst = SetVtxData(
					SMeshVtx[0],
					AXIS_VTX_FLAG,
					m_pVtxDst);
	m_pVtxDst = SetVtxData(
					SMeshVtx[1],
					AXIS_VTX_FLAG,
					m_pVtxDst);

	return UN_TRUE;
}
