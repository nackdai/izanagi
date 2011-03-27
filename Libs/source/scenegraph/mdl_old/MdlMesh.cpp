#include "scenegraph/mdl/MdlMesh.h"
#include "scenegraph/mdl/MdlMeshSubset.h"
#include "scenegraph/mdl/MdlSkeleton.h"
#include "izGraph.h"
#include "izIo.h"

using namespace izanagi;

CMdlMesh::CMdlMesh()
{
	FILL_ZERO(&m_Info, sizeof(m_Info));

	m_VD = IZ_NULL;

	m_pSubset = IZ_NULL;
}

CMdlMesh::~CMdlMesh()
{
	for (IZ_UINT i = 0; i < m_Info.numSubset; i++) {
		SAFE_DELETE(m_pSubset[i]);
	}

	SAFE_RELEASE(m_VD);
}

namespace {
	// 位置
	IZ_WORD _SetVtxElementPos(SVertexElement* pElem, IZ_WORD nOffset)
	{
		pElem->Offset = nOffset;
		pElem->Type = E_GRAPH_VTX_DECL_TYPE_FLOAT4;
		pElem->Usage = E_GRAPH_VTX_DECL_USAGE_POSITION;
		nOffset += E_MDL_VTX_SIZE_POS;
		return nOffset;
	}

	// 法線
	IZ_WORD _SetVtxElementNormal(SVertexElement* pElem, IZ_WORD nOffset)
	{
		pElem->Offset = nOffset;
		pElem->Type = E_GRAPH_VTX_DECL_TYPE_FLOAT3;
		pElem->Usage = E_GRAPH_VTX_DECL_USAGE_NORMAL;
		nOffset += E_MDL_VTX_SIZE_NORMAL;
		return nOffset;
	}

	// 頂点カラー
	IZ_WORD _SetVtxElementColor(SVertexElement* pElem, IZ_WORD nOffset)
	{
		pElem->Offset = nOffset;
		pElem->Type = E_GRAPH_VTX_DECL_TYPE_COLOR;
		pElem->Usage = E_GRAPH_VTX_DECL_USAGE_COLOR;
		nOffset += E_MDL_VTX_SIZE_COLOR;
		return nOffset;
	}

	// UV座標
	IZ_WORD _SetVtxElementUV(SVertexElement* pElem, IZ_WORD nOffset)
	{
		pElem->Offset = nOffset;
		pElem->Type = E_GRAPH_VTX_DECL_TYPE_FLOAT2;
		pElem->Usage = E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
		nOffset += E_MDL_VTX_SIZE_UV;
		return nOffset;
	}

	// 接ベクトル
	IZ_WORD _SetVtxElementTangent(SVertexElement* pElem, IZ_WORD nOffset)
	{
		pElem->Offset = nOffset;
		pElem->Type = E_GRAPH_VTX_DECL_TYPE_FLOAT3;
		pElem->Usage = E_GRAPH_VTX_DECL_USAGE_TANGENT;
		nOffset += E_MDL_VTX_SIZE_TANGENT;
		return nOffset;
	}

	// ブレンドウエイト
	IZ_WORD _SetVtxElementBlendWeight(SVertexElement* pElem, IZ_WORD nOffset)
	{
		pElem->Offset = nOffset;
		pElem->Type = E_GRAPH_VTX_DECL_TYPE_FLOAT4;
		pElem->Usage = E_GRAPH_VTX_DECL_USAGE_BLENDWEIGHT;
		nOffset += E_MDL_VTX_SIZE_BLENDWEIGHT;
		return nOffset;
	}

	// ブレンドインデックス
	IZ_WORD _SetVtxElementBlendIndices(SVertexElement* pElem, IZ_WORD nOffset)
	{
		pElem->Offset = nOffset;
		pElem->Type = E_GRAPH_VTX_DECL_TYPE_FLOAT4;
		pElem->Usage = E_GRAPH_VTX_DECL_USAGE_BLENDINDICES;
		nOffset += E_MDL_VTX_SIZE_BLENDINDICES;
		return nOffset;
	}

	// Create VertexDeclaration.
	CVertexDeclaration* _CreateVertexDeclaration(
		CGraphicsDevice* pDevice,
		const S_MDL_MESH& sMesh)
	{
		typedef IZ_WORD (*FuncSetVtxElem)(SVertexElement*, IZ_WORD);

		FuncSetVtxElem funcSetVtxElem[] = {
			_SetVtxElementPos,
			_SetVtxElementNormal,
			_SetVtxElementColor,
			_SetVtxElementUV,
			_SetVtxElementTangent,
			_SetVtxElementBlendIndices,
			_SetVtxElementBlendWeight,
		};
		IZ_C_ASSERT(COUNTOF(funcSetVtxElem) == E_MDL_VTX_FMT_TYPE_NUM);

		SVertexElement sVtxElem[E_MDL_VTX_FMT_TYPE_NUM];
		FILL_ZERO(&sVtxElem, sizeof(sVtxElem));

		IZ_WORD nOffset = 0;
		IZ_WORD nVtxElemNum = 0;

		for (IZ_UINT i = 0; i < E_MDL_VTX_FMT_TYPE_NUM; i++) {
			if ((sMesh.fmt & (1 << i)) > 0) {
				nOffset = (*funcSetVtxElem[i])(&sVtxElem[nVtxElemNum], nOffset);
				nVtxElemNum++;
			}
		}

		CVertexDeclaration* pVD = pDevice->CreateVertexDeclaration(
									sVtxElem,
									nVtxElemNum);
		IZ_ASSERT_NULL(pVD);

		return pVD;
	}
}	// namespace

IZ_UINT8* CMdlMesh::Read(
	IZ_UINT8* pBuf,
	CGraphicsDevice* pDevice,
	IInputStream* pIn)
{
	IZ_INPUT_READ_ASSERT(pIn, &m_Info, 0, sizeof(m_Info));

	m_VD = _CreateVertexDeclaration(pDevice, m_Info);
	VRETURN(m_VD != IZ_NULL);

	m_pSubset = reinterpret_cast<CMdlMeshSubset**>(pBuf);

	pBuf += sizeof(CMdlMeshSubset*) * m_Info.numSubset;

	for (IZ_UINT i = 0; i < m_Info.numSubset; i++) {
		// Create mesh.
		m_pSubset[i] = new(pBuf) CMdlMeshSubset;

		pBuf += sizeof(CMdlMeshSubset);

		VRETURN_NULL(m_pSubset[i]->Read(pDevice, pIn));

		// Registry mesh to render list.
		if (m_Info.isTranslucent) {
			m_TranslucentList.AddTail(m_pSubset[i]->GetRenderListItem());
		}
		else {
			m_OpacityList.AddTail(m_pSubset[i]->GetRenderListItem());
		}
	}

	return pBuf;
}

CMdlMeshSubset* CMdlMesh::GetSubset(IZ_UINT idx)
{
	IZ_ASSERT(idx < m_Info.numSubset);
	IZ_ASSERT(m_pSubset != IZ_NULL);

	return m_pSubset[idx];
}

IZ_BOOL CMdlMesh::Render(
	CGraphicsDevice* pDevice,
	CStdList<CMdlMeshSubset>& listMeshSubset,
	const CMdlSkeleton* pSkeleton,
	IMdlRenderHandler* pRenderHandler)
{
	VRETURN(pDevice->SetVertexDeclaration(m_VD));

	CStdList<CMdlMeshSubset>::Item* pItem = listMeshSubset.GetTop();

	while (pItem != IZ_NULL) {
		CMdlMeshSubset* pMeshSubset = pItem->GetData();

		if (pRenderHandler != IZ_NULL) {
			pRenderHandler->BeginRenderMesh();
		}

#if 0
		VRETURN(
			pMeshSubset->DebugRender(
				pDevice,
				pSkeleton,
				pRenderHandler));
#else
		VRETURN(
			pMeshSubset->Render(
				pDevice,
				pSkeleton,
				pRenderHandler));
#endif

		if (pRenderHandler != IZ_NULL) {
			pRenderHandler->EndRenderMesh();
		}

		pItem = pItem->GetNext();
	}

	return IZ_TRUE;
}
