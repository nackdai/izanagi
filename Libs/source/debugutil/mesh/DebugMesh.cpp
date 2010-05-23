#include "debugutil/mesh/DebugMesh.h"
#include "debugutil/mesh/DebugMeshAxis.h"
#include "math/MathVector.h"

using namespace uranus;

CDebugMesh::SetElemFunc CDebugMesh::SetElemFuncTbl[] = {
	&CDebugMesh::SetVtxElementPos,
	&CDebugMesh::SetVtxElementNormal,
	&CDebugMesh::SetVtxElementColor,
	&CDebugMesh::SetVtxElementUV,
	&CDebugMesh::SetVtxElementTangent,
};

CDebugMesh::SetVtxFunc CDebugMesh::SetVtxFuncTbl[] = {
	&CDebugMesh::SetVtxPos,
	&CDebugMesh::SetVtxNormal,
	&CDebugMesh::SetVtxColor,
	&CDebugMesh::SetVtxUV,
	&CDebugMesh::SetVtxTangent,
};

CDebugMesh::GetElemSizeFunc CDebugMesh::GetElemSizeFuncTbl[] = {
	&CDebugMesh::GetPosSize,
	&CDebugMesh::GetColorSize,
	&CDebugMesh::GetNormalSize,
	&CDebugMesh::GetUVSize,
	&CDebugMesh::GetTangentSize,
};

/**
* コンストラクタ
*/
CDebugMesh::CDebugMesh()
{
	m_pAllocator = UN_NULL;
	m_pDevice = UN_NULL;
	
	m_pVB = UN_NULL;
	m_pIB = UN_NULL;
	m_pVD = UN_NULL;

	m_PrimType = E_GRAPH_PRIM_TYPE_TRIANGLESTRIP;
	m_nPrimCnt = 0;

	m_pVtx = UN_NULL;
	m_pFace = UN_NULL;

	m_nVtxFormFlag = 0;

	m_pDebugAxis = UN_NULL;
}

/**
* デストラクタ
*/
CDebugMesh::~CDebugMesh()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVD);

	ClearDataBuffer();

	SAFE_RELEASE(m_pDebugAxis);
}

/**
* 描画
*/
UN_BOOL CDebugMesh::Draw(UN_BOOL bEnableDrawDebugAxis/*= UN_FALSE*/)
{
	UN_ASSERT(m_pVB != UN_NULL);
	UN_ASSERT(m_pVD != UN_NULL);

	// 頂点バッファ
	m_pDevice->SetVertexBuffer(
		0,
		0,
		m_pVB->GetStride(),
		m_pVB);

	// 頂点宣言
	m_pDevice->SetVertexDeclaration(m_pVD);

	// インデックスバッファ
	if (m_pIB != UN_NULL) {
		m_pDevice->SetIndexBuffer(m_pIB);
	}

	// 描画
	if (m_pIB != UN_NULL) {
		m_pDevice->DrawIndexedPrimitive(
			m_PrimType,
			0,
			0,
			m_pVB->GetVtxNum(),
			0,
			m_nPrimCnt);
	}
	else {
		m_pDevice->DrawPrimitive(
			m_PrimType,
			0,
			m_nPrimCnt);
	}

	if (bEnableDrawDebugAxis) {
		if (m_pDebugAxis != UN_NULL) {
			m_pDebugAxis->Draw();
		}
	}

	return UN_TRUE;
}

// 頂点バッファ作成
UN_BOOL CDebugMesh::CreateVB(
	UN_UINT flag,
	UN_UINT nVtxNum)
{
	UN_C_ASSERT(COUNTOF(GetElemSizeFuncTbl) == E_DEBUG_MESH_VTX_FORM_NUM);

	UN_UINT nStride = 0;
	for (UN_UINT i = 0; i < COUNTOF(GetElemSizeFuncTbl); i++) {
		nStride += (*GetElemSizeFuncTbl[i])(flag);
	}

	m_pVB = m_pDevice->CreateVertexBuffer(
				nStride,
				nVtxNum,
				E_GRAPH_RSC_CREATE_TYPE_STATIC);

	return (m_pVB != UN_NULL);
}

// インデックスバッファ作成
UN_BOOL CDebugMesh::CreateIB(
	UN_UINT nIdxNum,
	E_GRAPH_INDEX_BUFFER_FMT fmt)
{
	m_pIB = m_pDevice->CreateIndexBuffer(
				nIdxNum,
				fmt,
				E_GRAPH_RSC_CREATE_TYPE_STATIC);

	return (m_pIB != UN_NULL);
}

// 頂点宣言作成
UN_BOOL CDebugMesh::CreateVD(UN_UINT flag)
{
	UN_C_ASSERT(COUNTOF(SetElemFuncTbl) == E_DEBUG_MESH_VTX_FORM_NUM);

	// +1はENDの分
	SVertexElement VtxElement[E_DEBUG_MESH_VTX_FORM_NUM + 1];
	FILL_ZERO(VtxElement, sizeof(VtxElement));

	UN_WORD nOffset = 0;
	UN_UINT pos = 0;

	for (UN_UINT i = 0; i < COUNTOF(SetElemFuncTbl); i++) {
		pos = (*SetElemFuncTbl[i])(flag, VtxElement, pos, &nOffset);
	}

	m_pVD = m_pDevice->CreateVertexDeclaration(VtxElement, pos);

	return (m_pVD != UN_NULL);
}

// データバッファ作成
UN_BOOL CDebugMesh::CreateDataBuffer(
	UN_UINT nVtxNum,
	UN_UINT nIdxNum)
{
	UN_ASSERT(m_nPrimCnt > 0);

	m_pVtx = (SMeshVtx*)ALLOC_ZERO(m_pAllocator, sizeof(SMeshVtx) * nVtxNum);
	VRETURN_NULL(m_pVtx);

	m_pFace = (SMeshFace*)ALLOC_ZERO(m_pAllocator, sizeof(SMeshFace) * m_nPrimCnt);
	VRETURN_NULL(m_pFace);

	return UN_TRUE;
}

// デバッグ用軸メッシュ作成
UN_BOOL CDebugMesh::CreateDebugAxis(
	UN_UINT nVtxNum,
	UN_UINT flag)
{
	UN_ASSERT(nVtxNum > 0);

	UN_UINT nAxisFlag = 0;
	nAxisFlag |= (IsNormal(flag) ? E_DEBUG_MESH_AXIS_Z : 0);
	nAxisFlag |= (IsTangent(flag) ? E_DEBUG_MESH_AXIS_X | E_DEBUG_MESH_AXIS_Y : 0);

	m_pDebugAxis = CDebugMeshAxis::CreateDebugMeshAxis(
					m_pAllocator,
					m_pDevice,
					nAxisFlag,
					nVtxNum);
	UN_ASSERT(m_pDebugAxis != UN_NULL);

	return (m_pDebugAxis != UN_NULL);
}

// データバッファクリア
void CDebugMesh::ClearDataBuffer()
{
	FREE(m_pAllocator, m_pVtx);
	FREE(m_pAllocator, m_pFace);
}

// データをVB、IBにコピーする
UN_BOOL CDebugMesh::CopyDataToBuffer(UN_UINT flag)
{
	UN_ASSERT(m_pVtx != UN_NULL);
	UN_ASSERT(m_pFace != UN_NULL);

	UN_BOOL ret = UN_TRUE;

	// インデックス
	UN_UINT32* pIdxData = UN_NULL;
	VGOTO(ret = LockIB((void**)&pIdxData), __EXIT__);
	{
		for (UN_UINT i = 0; i < m_nPrimCnt; i++) {
			*pIdxData++ = m_pFace[i].idx[0];
			*pIdxData++ = m_pFace[i].idx[1];
			*pIdxData++ = m_pFace[i].idx[2];

			// 面ごとの計算
			ComputeFace(&m_pFace[i], flag);
		}
	}
	VGOTO(ret = UnlockIB(), __EXIT__);

	// 頂点
	UN_UINT8* pVtxData = UN_NULL;
	VGOTO(ret = LockVB((void**)&pVtxData), __EXIT__);
	{
		UN_UINT nVtxNum = GetVtxNum();

		for (UN_UINT i = 0; i < nVtxNum; i++) {
			ApplyOverlap(
				&m_pVtx[i],
				flag);

			pVtxData = SetVtxData(
						m_pVtx[i],
						flag,
						pVtxData);
		}
	}
	VGOTO(ret = UnlockVB(), __EXIT__);

__EXIT__:
	// もういらない
	ClearDataBuffer();

	return ret;
}

UN_BOOL CDebugMesh::LockVB(void** p)
{
	UN_ASSERT(p != UN_NULL);

	VRETURN(m_pVB->Lock(0, 0, p, UN_FALSE));

	if (m_pDebugAxis != UN_NULL) {
		VRETURN(m_pDebugAxis->BeginRegister());
	}

	return UN_TRUE;
}

UN_BOOL CDebugMesh::UnlockVB()
{
	VRETURN(m_pVB->Unlock());
	
	if (m_pDebugAxis != UN_NULL) {
		VRETURN(m_pDebugAxis->EndRegister());
	}

	return UN_TRUE;
}

UN_BOOL CDebugMesh::LockIB(void** p)
{
	UN_ASSERT(p != UN_NULL);

	VRETURN(m_pIB->Lock(0, 0, p, UN_FALSE));
	return UN_TRUE;
}

UN_BOOL CDebugMesh::UnlockIB()
{
	return m_pIB->Unlock();
}

// 頂点データセット
UN_UINT8* CDebugMesh::SetVtxData(
	const SMeshFace& sFace,
	UN_UINT flag,
	UN_UINT8* pData)
{
	for (UN_UINT i = 0; i < SMeshFace::VTX_NUM; i++) {
		SMeshVtx* pVtx = GetVtx(sFace.idx[i]);

		pData = SetVtxData(
					*pVtx,
					flag,
					pData);
	}

	return pData;
}

// 頂点データセット
UN_UINT8* CDebugMesh::SetVtxData(
	const SMeshVtx& sVtx,
	UN_UINT flag,
	UN_UINT8* pData)
{
	UN_C_ASSERT(COUNTOF(SetVtxFuncTbl) == E_DEBUG_MESH_VTX_FORM_NUM);

	for (UN_UINT i = 0; i < COUNTOF(SetVtxFuncTbl); i++) {
		pData = (*SetVtxFuncTbl[i])(flag, sVtx, pData);
	}

	if (m_pDebugAxis != UN_NULL) {
		if (IsNormal(flag)) {
			m_pDebugAxis->SetVtx(
				E_DEBUG_MESH_AXIS_Z,
				sVtx.pos,
				sVtx.nml);
		}

		if (IsTangent(flag)) {
			// Tangent
			m_pDebugAxis->SetVtx(
				E_DEBUG_MESH_AXIS_X,
				sVtx.pos,
				sVtx.tangent);

			// BiNormal
			m_pDebugAxis->SetVtx(
				E_DEBUG_MESH_AXIS_Y,
				sVtx.pos,
				sVtx.binml);
		}
	}
	
	return pData;
}

// 頂点と面を関連付ける
UN_BOOL CDebugMesh::BindFaceToVtx(SMeshFace* pFace)
{
	for (UN_UINT i = 0; i < SMeshFace::VTX_NUM; i++) {
		SMeshVtx* pVtx = GetVtx(pFace->idx[i]);
		VRETURN_NULL(pVtx);

		VRETURN(pVtx->AddFace(pFace));
	}

	return UN_TRUE;
}

// 面ごとの計算
void CDebugMesh::ComputeFace(
	SMeshFace* pFace,
	UN_UINT flag)
{
	if (IsTangent(flag)) {
		SMeshVtx* vtx0 = GetVtx(pFace->idx[0]);
		SMeshVtx* vtx1 = GetVtx(pFace->idx[1]);
		SMeshVtx* vtx2 = GetVtx(pFace->idx[2]);

		// Tangent
		ComputeTangent(vtx0, *vtx1, *vtx2);
		ComputeTangent(vtx1, *vtx2, *vtx0);
		ComputeTangent(vtx2, *vtx0, *vtx1);
	}
}

void CDebugMesh::ComputeTangent(
	SMeshVtx* vtx0,
	const SMeshVtx& vtx1,
	const SMeshVtx& vtx2)
{
	SVector vP;
	SubVector(vP, vtx1.pos, vtx0->pos);

	SVector vQ;
	SubVector(vQ, vtx2.pos, vtx0->pos);

	UN_FLOAT fCoeff[4] = {
		vtx2.uv[1] - vtx0->uv[1],
		-(vtx1.uv[1] - vtx0->uv[1]),
		-(vtx2.uv[0] - vtx0->uv[0]),
		vtx1.uv[0] - vtx0->uv[0],
	};

	UN_FLOAT fInvDeterminant = 1.0f / (fCoeff[3] * fCoeff[0] - fCoeff[2] * fCoeff[1]);

	// BiNormal
	SVector vB;
	{
		ScaleVector(vP, vP, fInvDeterminant * fCoeff[2]);
		ScaleVector(vQ, vQ, fInvDeterminant * fCoeff[3]);
		AddVector(vB, vP, vQ);
		NormalizeVector(vB, vB);
	}

	// Tangent
	SVector vT;
	{
		// X(T) = Y(B) x Z(N)
		CrossVector(vT, vB, vtx0->nml);
	}

	// Y(B) = Z(N) x X(T)
	CrossVector(vB, vtx0->nml, vT);

	UN_FLOAT fLen = LengthVector(vtx0->tangent);
	if (fLen == 0.0f) {
		CopyVector(vtx0->tangent, vT);
		CopyVector(vtx0->binml, vB);
	}
	else {
		AddVector(vtx0->tangent, vtx0->tangent, vT);
		ScaleVector(vtx0->tangent, vtx0->tangent, 0.5f);
		NormalizeVector(vtx0->tangent, vtx0->tangent);

		AddVector(vtx0->binml, vtx0->binml, vB);
		ScaleVector(vtx0->binml, vtx0->binml, 0.5f);
		NormalizeVector(vtx0->binml, vtx0->binml);
	}
}

void CDebugMesh::SetOverlapVtx(
	UN_UINT idx_0,
	UN_UINT idx_1)
{
	SMeshVtx* pVtx_0 = GetVtx(idx_0);
	SMeshVtx* pVtx_1 = GetVtx(idx_1);

	pVtx_0->SetOverlapIdx(idx_1);
	pVtx_1->SetOverlapIdx(idx_0);
}

void CDebugMesh::ApplyOverlap(
	SMeshVtx* pVtx,
	UN_UINT flag)
{
#if 1
	if (pVtx->HasOverlapIdx()) {
		UN_UINT idx = pVtx->GetOverlapIdx();
		SMeshVtx* pOverlap = GetVtx(idx);

		if (IsTangent(flag)) {
			// Tangent
			AddVector(pVtx->tangent, pVtx->tangent, pOverlap->tangent);
			ScaleVector(pVtx->tangent, pVtx->tangent, 0.5f);
			NormalizeVector(pVtx->tangent, pVtx->tangent);

			CopyVector(pOverlap->tangent, pVtx->tangent);

			// BiNormal
			AddVector(pVtx->binml, pVtx->binml, pOverlap->binml);
			ScaleVector(pVtx->binml, pVtx->binml, 0.5f);
			NormalizeVector(pVtx->binml, pVtx->binml);

			CopyVector(pOverlap->binml, pVtx->binml);
		}
	}
#endif
}

//////////////////////////////////////////////////////////////

// 位置
UN_UINT CDebugMesh::SetVtxElementPos(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset)
{
	if (IsPos(flag)) {
		pElem[nPos].Offset = *pOffset;
		pElem[nPos].Type = E_GRAPH_VTX_DECL_TYPE_FLOAT4;
		pElem[nPos].Usage = E_GRAPH_VTX_DECL_USAGE_POSITION;
		*pOffset += GetPosSize(flag);
		return nPos + 1;
	}
	return nPos;
}

// 法線
UN_UINT CDebugMesh::SetVtxElementNormal(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset)
{
	if (IsNormal(flag)) {
		pElem[nPos].Offset = *pOffset;
		pElem[nPos].Type = E_GRAPH_VTX_DECL_TYPE_FLOAT3;
		pElem[nPos].Usage = E_GRAPH_VTX_DECL_USAGE_NORMAL;
		*pOffset += GetNormalSize(flag);
		return nPos + 1;
	}
	return nPos;
}

// 頂点カラー
UN_UINT CDebugMesh::SetVtxElementColor(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset)
{
	if (IsColor(flag)) {
		pElem[nPos].Offset = *pOffset;
		pElem[nPos].Type = E_GRAPH_VTX_DECL_TYPE_COLOR;
		pElem[nPos].Usage = E_GRAPH_VTX_DECL_USAGE_COLOR;
		*pOffset += GetColorSize(flag);
		return nPos + 1;
	}
	return nPos;
}

// UV座標
UN_UINT CDebugMesh::SetVtxElementUV(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset)
{
	if (IsUV(flag)) {
		pElem[nPos].Offset = *pOffset;
		pElem[nPos].Type = E_GRAPH_VTX_DECL_TYPE_FLOAT2;
		pElem[nPos].Usage = E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
		*pOffset += GetUVSize(flag);
		return nPos + 1;
	}
	return nPos;
}

// 接ベクトル
UN_UINT CDebugMesh::SetVtxElementTangent(UN_UINT flag, SVertexElement* pElem, UN_UINT nPos, UN_WORD* pOffset)
{
	if (IsTangent(flag)) {
		pElem[nPos].Offset = *pOffset;
		pElem[nPos].Type = E_GRAPH_VTX_DECL_TYPE_FLOAT3;
		pElem[nPos].Usage = E_GRAPH_VTX_DECL_USAGE_TANGENT;
		*pOffset += GetTangentSize(flag);
		return nPos + 1;
	}
	return nPos;
}

// 位置
UN_UINT8* CDebugMesh::SetVtxPos(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx)
{
	if (IsPos(flag)) {
		UN_FLOAT* pos = (UN_FLOAT*)pVtx;
		pos[0] = sVtx.pos.v[0];
		pos[1] = sVtx.pos.v[1];
		pos[2] = sVtx.pos.v[2];
		pos[3] = 1.0f;

		pVtx += GetPosSize(flag);
	}
	return pVtx;
}

// 法線
UN_UINT8* CDebugMesh::SetVtxNormal(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx)
{
	if (IsNormal(flag)) {
		UN_FLOAT* nml = (UN_FLOAT*)pVtx;
		nml[0] = sVtx.nml.v[0];
		nml[1] = sVtx.nml.v[1];
		nml[2] = sVtx.nml.v[2];

		pVtx += GetNormalSize(flag);
	}
	return pVtx;
}

// 頂点カラー
UN_UINT8* CDebugMesh::SetVtxColor(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx)
{
	if (IsColor(flag)) {
		*(UN_COLOR*)pVtx = sVtx.clr;
		pVtx += GetColorSize(flag);
	}
	return pVtx;
}

// UV座標
UN_UINT8* CDebugMesh::SetVtxUV(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx)
{
	if (IsUV(flag)) {
		UN_FLOAT* uv = (UN_FLOAT*)pVtx;
		uv[0] = sVtx.uv[0];
		uv[1] = sVtx.uv[1];

		pVtx += GetUVSize(flag);
	}
	return pVtx;
}

// 接ベクトル
UN_UINT8* CDebugMesh::SetVtxTangent(UN_UINT flag, const SMeshVtx& sVtx, UN_UINT8* pVtx)
{
	if (IsTangent(flag)) {
		UN_FLOAT* tangent = (UN_FLOAT*)pVtx;
		tangent[0] = sVtx.tangent.v[0];
		tangent[1] = sVtx.tangent.v[1];
		tangent[2] = sVtx.tangent.v[2];
		//tangent[3] = sVtx.tangent.v[3];

		pVtx += GetTangentSize(flag);
	}
	return pVtx;
}
