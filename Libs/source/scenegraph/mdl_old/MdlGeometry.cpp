#include "scenegraph/mdl/MdlGeometry.h"
#include "scenegraph/mdl/MdlMesh.h"
#include "scenegraph/mdl/MdlMeshSubset.h"
#include "scenegraph/mdl/MdlRenderHandler.h"
#include "scenegraph/mdl/MdlSkeleton.h"
#include "izGraph.h"
#include "izIo.h"

using namespace izanagi;

CMdlGeometry* CMdlGeometry::CreateMdlGeometry(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice,
	IInputStream* pIn)
{
	S_MDL_GEOMETRY_CHUNK_HEADER sHeader;
	IZ_INPUT_READ_VRETURN(pIn, &sHeader, 0, sizeof(sHeader));

	IZ_BOOL result = IZ_FALSE;

	// Compute buffer size.
	IZ_UINT nSize = sizeof(CMdlGeometry);
	nSize += sizeof(CVertexBuffer*) * sHeader.numVB;
	nSize += sizeof(CMdlMesh*) * sHeader.numMesh;
	nSize += sizeof(CMdlMesh) * sHeader.numMesh;
	nSize += sizeof(CMdlMeshSubset*) * sHeader.numMeshSubset;
	nSize += sizeof(CMdlMeshSubset) * sHeader.numMeshSubset;

	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	CMdlGeometry* pInstance = new(pBuf) CMdlGeometry;
	{
		pBuf += sizeof(CMdlGeometry);

		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		memcpy(&pInstance->m_Header, &sHeader, sizeof(sHeader));

		pInstance->m_pVB = reinterpret_cast<CVertexBuffer**>(pBuf);
		pBuf += sizeof(CVertexBuffer*) * sHeader.numVB;

		// Read vertices.
		result = pInstance->ReadVertices(
					sHeader,
					pDevice,
					pIn);
		VGOTO(result, __EXIT__);

		pInstance->m_pMesh = reinterpret_cast<CMdlMesh**>(pBuf);
		pBuf += sizeof(CMdlMesh*) * sHeader.numMesh;

		for (IZ_UINT i = 0; i < sHeader.numMesh; i++) {
			// Create geometry.
			pInstance->m_pMesh[i] = new(pBuf) CMdlMesh;

			pBuf += sizeof(CMdlMesh);

			pBuf = pInstance->m_pMesh[i]->Read(pBuf, pDevice, pIn);
			VGOTO(result = (pBuf != IZ_NULL), __EXIT__);

			// Set vertex buffer to mesh subset.
			IZ_UINT nSubsetNum = pInstance->m_pMesh[i]->GetMeshSubsetNum();
			for (IZ_UINT n = 0; n < nSubsetNum; n++) {
				CMdlMeshSubset* pSubset = pInstance->m_pMesh[i]->GetSubset(n);
				IZ_ASSERT(pSubset != IZ_NULL);

				CVertexBuffer* pVB = pInstance->m_pVB[pSubset->GetInfo().idxVB];
				pSubset->SetVB(pVB);
			}
		}
	}

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}

	return pInstance;
}

CMdlGeometry::CMdlGeometry()
{
	m_pAllocator = IZ_NULL;

	FILL_ZERO(&m_Header, sizeof(m_Header));
}

CMdlGeometry::~CMdlGeometry()
{
	for (IZ_UINT i = 0; i < m_Header.numVB; i++) {
		SAFE_RELEASE(m_pVB[i]);
	}

	for (IZ_UINT i = 0; i < m_Header.numMesh; i++) {
		SAFE_DELETE(m_pMesh[i]);
	}
}

namespace {
	// Create VertexBuffer.
	CVertexBuffer* _CreateVertexBuffer(
		CGraphicsDevice* pDevice,
		const S_MDL_VERTICES& sVtxInfo,
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

IZ_BOOL CMdlGeometry::ReadVertices(
	const S_MDL_GEOMETRY_CHUNK_HEADER& sHeader,
	CGraphicsDevice* pDevice,
	IInputStream* pIn)
{
	IZ_ASSERT(m_pVB != IZ_NULL);

	for (IZ_UINT i = 0; i < sHeader.numVB; i++) {
		S_MDL_VERTICES sVtxInfo;
		IZ_INPUT_READ_VRETURN(pIn, &sVtxInfo, 0, sizeof(sVtxInfo));

		m_pVB[i] = _CreateVertexBuffer(
					pDevice,
					sVtxInfo,
					pIn);
		VRETURN(m_pVB[i] != IZ_NULL);
	}

	return IZ_TRUE;
}

IZ_BOOL CMdlGeometry::Render(
	CGraphicsDevice* pDevice,
	const CMdlSkeleton* pSkeleton,
	IMdlRenderHandler* pRenderHandler)
{
	for (IZ_UINT i = 0; i < m_Header.numMesh; i++) {
		VRETURN(
			m_pMesh[i]->RenderOpacityMesh(
				pDevice,
				pSkeleton,
				pRenderHandler));
	}

	for (IZ_UINT i = 0; i < m_Header.numMesh; i++) {
		VRETURN(
			m_pMesh[i]->RenderTranslucentMesh(
				pDevice, 
				pSkeleton,
				pRenderHandler));
	}

	return IZ_TRUE;
}
