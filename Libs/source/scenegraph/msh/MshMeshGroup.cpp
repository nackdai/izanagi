#include "scenegraph/msh/MshMeshGroup.h"
#include "scenegraph/msh/MshMeshSet.h"
#include "scenegraph/msh/MshPrimitiveSet.h"
#include "izGraph.h"
#include "izIo.h"

#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/skl/SkeletonInstance.h"

using namespace izanagi;

CMeshGroup::CMeshGroup()
{
	FILL_ZERO(&m_Info, sizeof(m_Info));

	m_pVB = IZ_NULL;
	m_pMeshSet = IZ_NULL;
}

CMeshGroup::~CMeshGroup()
{
	for (IZ_UINT i = 0; i < m_Info.numVB; ++i) {
		SAFE_RELEASE(m_pVB[i]);
	}

	for (IZ_UINT i = 0; i < m_Info.numMeshSet; ++i) {
		SAFE_RELEASE(m_pMeshSet[i]);
	}
}

CMeshSet* CMeshGroup::GetMeshSet(IZ_UINT idx)
{
	IZ_ASSERT(idx < m_Info.numMeshSet);
	IZ_ASSERT(m_pMeshSet != IZ_NULL);

	return m_pMeshSet[idx];
}

IZ_BOOL CMeshGroup::Render(
	CGraphicsDevice* pDevice,
	CSkeletonInstance* pSkl,
	IMshRenderHandler* pRenderHandler)
{
	for (IZ_UINT i = 0; i < m_Info.numMeshSet; ++i) {
		VRETURN(
			m_pMeshSet[i]->Render(
				pDevice,
				pSkl,
				pRenderHandler));
	}

	return IZ_TRUE;
}

IZ_UINT8* CMeshGroup::Read(
	IZ_UINT8* pBuf,
	CGraphicsDevice* pDevice,
	IInputStream* pIn)
{
	IZ_INPUT_READ_VRETURN(pIn, &m_Info, 0, sizeof(m_Info));

	IZ_BOOL result = IZ_FALSE;

	m_pVB = reinterpret_cast<CVertexBuffer**>(pBuf);
	pBuf += sizeof(CVertexBuffer*) * m_Info.numVB;

	// Read vertices.
	result = ReadVertices(
				pDevice,
				pIn);
	VRETURN(result);

	m_pMeshSet = reinterpret_cast<CMeshSet**>(pBuf);
	pBuf += sizeof(CMeshSet*) * m_Info.numMeshSet;

	for (IZ_UINT i = 0; i < m_Info.numMeshSet; ++i) {
		// Create geometry.
		m_pMeshSet[i] = new(pBuf) CMeshSet;
		m_pMeshSet[i]->AddRef();

		pBuf += sizeof(CMeshSet);

		pBuf = m_pMeshSet[i]->Read(pBuf, pDevice, pIn);
		VRETURN(pBuf != IZ_NULL)

		// Set vertex buffer to mesh subset.
		IZ_UINT nSubsetNum = m_pMeshSet[i]->GetPrimSetNum();
		for (IZ_UINT n = 0; n < nSubsetNum; ++n) {
			CPrimitiveSet* pSubset = m_pMeshSet[i]->GetPrimSet(n);
			IZ_ASSERT(pSubset != IZ_NULL);

			CVertexBuffer* pVB = m_pVB[pSubset->GetInfo().idxVB];
			pSubset->SetVB(pVB);
		}
	}

	return pBuf;
}

namespace {
	// Create VertexBuffer.
	CVertexBuffer* _CreateVertexBuffer(
		CGraphicsDevice* pDevice,
		const S_MSH_VERTICES& sVtxInfo,
		IInputStream* pIn)
	{
		// Create VertexBuffer.
		CVertexBuffer* pVB = pDevice->CreateVertexBuffer(
								sVtxInfo.sizeVtx,
								sVtxInfo.numVtx,
								E_GRAPH_RSC_TYPE_STATIC);
		IZ_BOOL ret = (pVB != IZ_NULL);
		VGOTO(ret, __EXIT__);

		void* buf = IZ_NULL;
		ret = pVB->Lock(0, 0, &buf, IZ_FALSE);
		VGOTO(ret, __EXIT__);

		IZ_UINT nSize = sVtxInfo.sizeVtx * sVtxInfo.numVtx;

		ret = IZ_INPUT_READ(pIn, buf, 0, nSize);

		pVB->Unlock();

__EXIT__:
		if (!ret) {
			SAFE_RELEASE(pVB);
		}
		return pVB;
	}
}	// namespace

IZ_BOOL CMeshGroup::ReadVertices(
	CGraphicsDevice* pDevice,
	IInputStream* pIn)
{
	IZ_ASSERT(m_pVB != IZ_NULL);

	for (IZ_UINT i = 0; i < m_Info.numVB; ++i) {
		S_MSH_VERTICES sVtxInfo;
		IZ_INPUT_READ_VRETURN(pIn, &sVtxInfo, 0, sizeof(sVtxInfo));

		m_pVB[i] = _CreateVertexBuffer(
					pDevice,
					sVtxInfo,
					pIn);
		VRETURN(m_pVB[i] != IZ_NULL);
	}

	return IZ_TRUE;
}
