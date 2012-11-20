#include "scenegraph/msh/izMesh.h"
#include "scenegraph/msh/MshMeshGroup.h"
#include "scenegraph/msh/MshMeshSet.h"
#include "scenegraph/msh/MshPrimitiveSet.h"
#include "izGraph.h"
#include "izIo.h"

#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/skl/SkeletonInstance.h"

using namespace izanagi;

CMesh* CMesh::CreateMesh(
	IMemoryAllocator* pAllocator,
	graph::CGraphicsDevice* pDevice,
	IInputStream* pIn)
{
	S_MSH_HEADER sHeader;
	IZ_INPUT_READ_VRETURN(pIn, &sHeader, 0, sizeof(sHeader));

	IZ_BOOL result = IZ_FALSE;

	// Compute buffer size.
	IZ_UINT nSize = sizeof(CMesh);
	nSize += sizeof(graph::CVertexBuffer*) * sHeader.numVB;
	nSize += sizeof(CMeshGroup*) * sHeader.numMeshGroup;
	nSize += sizeof(CMeshGroup) * sHeader.numMeshGroup;
	nSize += sizeof(CMeshSet*) * sHeader.numMeshSet;
	nSize += sizeof(CMeshSet) * sHeader.numMeshSet;
	nSize += sizeof(CPrimitiveSet*) * sHeader.numMeshSubset;
	nSize += sizeof(CPrimitiveSet) * sHeader.numMeshSubset;
	nSize += sizeof(IZ_UINT16) * sHeader.numAllJointIndices;

	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	IZ_UINT8* pTop = pBuf;

	CMesh* pInstance = new(pBuf) CMesh;
	{
		pBuf += sizeof(CMesh);

		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;

		memcpy(&pInstance->m_Header, &sHeader, sizeof(sHeader));

		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->m_pMeshGroup = reinterpret_cast<CMeshGroup**>(pBuf);
		pBuf += sizeof(CMeshGroup*) * sHeader.numMeshGroup;

		for (IZ_UINT i = 0; i < sHeader.numMeshGroup; ++i) {
			pInstance->m_pMeshGroup[i] = new(pBuf) CMeshGroup;
			pInstance->m_pMeshGroup[i]->AddRef();

			pBuf += sizeof(CMeshGroup);
			
			pBuf = pInstance->m_pMeshGroup[i]->Read(pBuf, pDevice, pIn);
			VGOTO(result = (pBuf != IZ_NULL), __EXIT__);
		}
	}

	IZ_ASSERT(CStdUtil::GetPtrDistance(pBuf, pTop) == nSize);

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}

	return pInstance;
}

CMesh::CMesh()
{
	m_Allocator = IZ_NULL;
	m_pDevice = IZ_NULL;

	FILL_ZERO(&m_Header, sizeof(m_Header));

	m_pMeshGroup = IZ_NULL;
}

CMesh::~CMesh()
{
	for (IZ_UINT i = 0; i < m_Header.numMeshGroup; ++i) {
		SAFE_RELEASE(m_pMeshGroup[i]);
	}

	SAFE_RELEASE(m_pDevice);
}

IZ_BOOL CMesh::Render(
	CSkeletonInstance* pSkl,
	IMshRenderHandler* pRenderHandler)
{
	// TODO

	IZ_BOOL ret = m_pMeshGroup[0]->Render(
					m_pDevice,
					pSkl,
					pRenderHandler);
	return ret;
}

CMeshGroup* CMesh::GetMeshGroup(IZ_UINT idx)
{
	IZ_ASSERT(idx < m_Header.numMeshGroup);
	return m_pMeshGroup[idx];
}
