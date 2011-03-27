#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/msh/izMesh.h"
#include "scenegraph/msh/MshMeshSet.h"

#include "scenegraph/msh/MshRenderHandler.h"
#include "scenegraph/msh/MshMeshGroup.h"
#include "scenegraph/msh/MshPrimitiveSet.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/mtrl/izMaterial.h"

using namespace izanagi;

///////////////////////////////////////////////////

class CMeshGroupInstance : public CPlacementNew {
public:
	static CMeshGroupInstance* CreateMeshGroupInstances(
		IZ_UINT8** pBuf,
		CMeshGroup* pGroup);

	static inline size_t ComputeBytes(CMesh* pMesh);

public:
	inline CMeshGroupInstance();
	inline ~CMeshGroupInstance();

	NO_COPIABLE(CMeshGroupInstance);

private:
	IZ_UINT8* Init(
		IZ_UINT8* pBuf,
		CMeshGroup* pGroup);

public:
	IZ_UINT GetMeshSetNum() { return m_nSetNum; }

	inline CMeshSetInstance* GetMeshSet(IZ_UINT idx);

	inline IZ_BOOL Render(
		CGraphicsDevice* pDevice,
		IMshRenderHandler* pRenderHandler);

	inline void SetSkeleton(CSkeletonInstance* pSkl);

	IZ_UINT GetMeshSetNum() const { return m_nSetNum; }

private:
	CMeshGroup* m_pBody;

	IZ_UINT m_nSetNum;
	CMeshSetInstance** m_pMeshSet;
};

CMeshGroupInstance* CMeshGroupInstance::CreateMeshGroupInstances(
	IZ_UINT8** pBuf,
	CMeshGroup* pGroup)
{
	IZ_ASSERT(pBuf != IZ_NULL);
	IZ_ASSERT(pGroup != IZ_NULL);

	IZ_UINT8*& buf = *pBuf;

	CMeshGroupInstance* pInstance = new(buf) CMeshGroupInstance;
	buf += sizeof(CMeshGroupInstance);
	
	buf = pInstance->Init(buf, pGroup);
	IZ_BOOL result = (buf != IZ_NULL);

	if (!result) {
		SAFE_DELETE(pInstance);
	}
	return pInstance;
}

size_t CMeshGroupInstance::ComputeBytes(CMesh* pMesh)
{
	IZ_ASSERT(pMesh != IZ_NULL);

	size_t ret = 0;

	IZ_UINT nGroupNum = pMesh->GetMeshGroupNum();

	for (IZ_UINT i = 0; i < nGroupNum; i++) {
		ret += sizeof(CMeshGroupInstance*);
		ret += sizeof(CMeshGroupInstance);

		CMeshGroup* pGroup = pMesh->GetMeshGroup(i);

		IZ_UINT nMeshSetNum = pGroup->GetMeshSetNum();

		for (IZ_UINT n = 0; n < nMeshSetNum; n++) {
			ret += sizeof(CMeshSetInstance*);
			ret += CMeshSetInstance::ComputeBytes(pGroup->GetMeshSet(n));
		}
	}

	return ret;
}

CMeshGroupInstance::CMeshGroupInstance()
{
	m_pBody = IZ_NULL;

	m_nSetNum = 0;
	m_pMeshSet = IZ_NULL;
}

CMeshGroupInstance::~CMeshGroupInstance()
{
	SAFE_RELEASE(m_pBody);

	for (IZ_UINT i = 0; i < m_nSetNum; i++) {
		delete m_pMeshSet[i];
	}
}

IZ_UINT8* CMeshGroupInstance::Init(
	IZ_UINT8* pBuf,
	CMeshGroup* pGroup)
{
	IZ_ASSERT(pBuf != IZ_NULL);
	IZ_ASSERT(pGroup != IZ_NULL);

	SAFE_REPLACE(m_pBody, pGroup);

	m_nSetNum = m_pBody->GetMeshSetNum();
	if (m_nSetNum == 0) {
		return pBuf;
	}

	m_pMeshSet = reinterpret_cast<CMeshSetInstance**>(pBuf);
	pBuf += sizeof(CMeshSetInstance*) * m_nSetNum;

	for (IZ_UINT i = 0; i < m_nSetNum; i++) {
		m_pMeshSet[i] = CMeshSetInstance::CreateMeshSetInstance(
							&pBuf,
							pGroup->GetMeshSet(i));
		if (m_pMeshSet[i] == IZ_NULL) {
			pBuf = IZ_NULL;
			break;
		}
	}
	
	return pBuf;
}

CMeshSetInstance* CMeshGroupInstance::GetMeshSet(IZ_UINT idx)
{
	IZ_ASSERT(idx < m_nSetNum);
	return m_pMeshSet[idx];
}

IZ_BOOL CMeshGroupInstance::Render(
	CGraphicsDevice* pDevice,
	IMshRenderHandler* pRenderHandler)
{
	for (IZ_UINT i = 0; i < m_nSetNum; ++i) {
		VRETURN(m_pMeshSet[i]->Render(pDevice, pRenderHandler));
	}

	return IZ_TRUE;
}

void CMeshGroupInstance::SetSkeleton(CSkeletonInstance* pSkl)
{
	for (IZ_UINT i = 0; i < m_nSetNum; ++i) {
		m_pMeshSet[i]->SetSkeleton(pSkl);
	}
}

///////////////////////////////////////////////////

CMeshSetInstance* CMeshSetInstance::CreateMeshSetInstance(
	IZ_UINT8** pBuf,
	CMeshSet* pMeshSet)
{
	IZ_ASSERT(pBuf != IZ_NULL);
	IZ_ASSERT(pMeshSet != IZ_NULL);

	IZ_UINT8*& buf = *pBuf;

	CMeshSetInstance* pInstance = new(buf) CMeshSetInstance;
	buf += sizeof(CMeshSetInstance);

	buf = pInstance->Init(buf, pMeshSet);
	IZ_BOOL result = (buf != IZ_NULL);

	if (!result) {
		SAFE_DELETE(pInstance);
	}
	return pInstance;
}

size_t CMeshSetInstance::ComputeBytes(CMeshSet* pMeshSet)
{
	IZ_ASSERT(pMeshSet != IZ_NULL);

	size_t ret = sizeof(CMeshSetInstance);

	ret += sizeof(CPrimitiveSet*) * pMeshSet->GetPrimSetNum();

	return ret;
}

CMeshSetInstance::CMeshSetInstance()
{
	m_pBody = IZ_NULL;

	m_nPrimNum = 0;
	m_pPrims = IZ_NULL;

	m_pSkl = IZ_NULL;

	m_pMtrl = IZ_NULL;

	m_fZ = 0.0f;

	m_ListItem.Init(this);
}

CMeshSetInstance::~CMeshSetInstance()
{
	SAFE_RELEASE(m_pBody);

	for (IZ_UINT i = 0; i < m_nPrimNum; i++) {
		SAFE_RELEASE(m_pPrims[i]);
	}

	SAFE_RELEASE(m_pSkl);
	SAFE_RELEASE(m_pMtrl);
}

IZ_UINT8* CMeshSetInstance::Init(
	IZ_UINT8* pBuf,
	CMeshSet* pMeshSet)
{
	IZ_ASSERT(pBuf != IZ_NULL);
	IZ_ASSERT(pMeshSet != IZ_NULL);

	SAFE_REPLACE(m_pBody, pMeshSet);

	m_nPrimNum = pMeshSet->GetPrimSetNum();
	if (m_nPrimNum == 0) {
		return pBuf;
	}

	m_pPrims = reinterpret_cast<CPrimitiveSet**>(pBuf);
	pBuf += sizeof(CPrimitiveSet*) * m_nPrimNum;

	for (IZ_UINT i = 0; i < m_nPrimNum; i++) {
		SAFE_REPLACE(m_pPrims[i], pMeshSet->GetPrimSet(i));
	}

	return pBuf;
}

IZ_BOOL CMeshSetInstance::Render(
	CGraphicsDevice* pDevice,
	IMshRenderHandler* pRenderHandler)
{
	IZ_ASSERT(m_pBody != IZ_NULL);

	CVertexDeclaration* pVD = m_pBody->GetVD();
	IZ_ASSERT(pVD != IZ_NULL);

	VRETURN(pDevice->SetVertexDeclaration(pVD));

	for (IZ_UINT i = 0; i < m_nPrimNum; ++i) {
		if (pRenderHandler != IZ_NULL) {
			pRenderHandler->BeginRenderMesh();
		}

		VRETURN(
			m_pPrims[i]->Render(
				pDevice,
				m_pSkl,
				pRenderHandler));

		if (pRenderHandler != IZ_NULL) {
			pRenderHandler->EndRenderMesh();
		}
	}

	return IZ_TRUE;
}

void CMeshSetInstance::SetSkeleton(CSkeletonInstance* pSkl)
{
	SAFE_REPLACE(m_pSkl, pSkl);
}

void CMeshSetInstance::SetMaterial(CMaterial* pMtrl)
{
	SAFE_REPLACE(m_pMtrl, pMtrl);
}

CMaterial* CMeshSetInstance::GetMaterial()
{
	return m_pMtrl;
}

const S_MSH_MTRL& CMeshSetInstance::GetMaterialInfo()
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	return m_pBody->GetMtrlInfo();
}

///////////////////////////////////////////////////

CMeshInstance* CMeshInstance::CreateMeshInstance(
	IMemoryAllocator* pAllocator,
	CMesh* pMesh)
{
	// Compute allocated size.
	size_t nSize = sizeof(CMeshInstance);
	nSize += CMeshGroupInstance::ComputeBytes(pMesh);

	// Allocate buffer.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	// Remenber buffer's top.
	IZ_UINT8* pTop = pBuf;

	IZ_BOOL result = IZ_TRUE;

	CMeshInstance* pInstance = new(pBuf) CMeshInstance;
	pBuf += sizeof(CMeshInstance);
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pBody, pMesh);

		IZ_UINT nGroupNum = pMesh->GetMeshGroupNum();

		pInstance->m_pGroups = reinterpret_cast<CMeshGroupInstance**>(pBuf);
		pBuf += sizeof(CMeshGroupInstance*) * nGroupNum;

		pInstance->m_nGroupNum = nGroupNum;

		for (IZ_UINT i = 0; i < nGroupNum; i++) {
			CMeshGroup* pGroup = pMesh->GetMeshGroup(i);

			pInstance->m_pGroups[i] = CMeshGroupInstance::CreateMeshGroupInstances(&pBuf, pGroup);

			result = (pInstance->m_pGroups[i] != IZ_NULL);
			IZ_ASSERT(result);
			if (!result) {
				break;
			}
		}
	}

	// Check distance of buffer.
	//IZ_ASSERT(CStdUtil::GetPtrDistance(pTop, pBuf) == nSize);

	if (!result) {
		SAFE_RELEASE(pInstance);
	}

	return pInstance;
}

CMeshInstance::CMeshInstance()
{
	m_pAllocator = IZ_NULL;
	m_pBody = IZ_NULL;

	m_nGroupNum = 0;
	m_pGroups = IZ_NULL;
}

CMeshInstance::~CMeshInstance()
{
	SAFE_RELEASE(m_pBody);

	for (IZ_UINT i = 0; i < m_nGroupNum; i++) {
		SAFE_DELETE(m_pGroups[i]);
	}
}

IZ_BOOL CMeshInstance::Render(
	CSkeletonInstance* pSkl,
	IMshRenderHandler* pRenderHandler)
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	IZ_ASSERT(m_pGroups != IZ_NULL);

	CGraphicsDevice* pDevice = m_pBody->GetGraphicsDevice();
	IZ_ASSERT(pDevice != IZ_NULL);

	// TODO
	m_pGroups[0]->SetSkeleton(pSkl);

	IZ_BOOL ret = m_pGroups[0]->Render(pDevice, pRenderHandler);

	return ret;
}

void CMeshInstance::ApplySkeleton(CSkeletonInstance* pSkl)
{
	IZ_ASSERT(m_pGroups != IZ_NULL);

	// TODO
	m_pGroups[0]->SetSkeleton(pSkl);
}

IZ_UINT CMeshInstance::GetMeshSetNum() const
{
	IZ_ASSERT(m_pGroups != IZ_NULL);

	// TODO
	IZ_UINT ret = m_pGroups[0]->GetMeshSetNum();
	return ret;
}

CMeshSetInstance* CMeshInstance::GetMeshSet(IZ_UINT idx)
{
	IZ_ASSERT(m_pGroups != IZ_NULL);
	IZ_ASSERT(idx < GetMeshSetNum());

	CMeshSetInstance* ret = m_pGroups[0]->GetMeshSet(idx);
	return ret;
}
