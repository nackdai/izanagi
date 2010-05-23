#include "std/resource/StdResourceManager.h"
#include "std/resource/StdResource.h"

using namespace uranus;

/**
* Create an instance of CResourceManager.
*/
CResourceManager* CResourceManager::CreateResourceManager(
	IMemoryAllocator* pAllocator,
	UN_UINT nInitialCreateRscNum)
{
	UN_ASSERT(pAllocator != UN_NULL);

	// Compute allocated size.
	size_t nSize = sizeof(CResourceManager);
	nSize += sizeof(CResource) * nInitialCreateRscNum;

	// Allocate memory.
	UN_UINT8* pBuf = reinterpret_cast<UN_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != UN_NULL);

	// Create an instance.
	CResourceManager* pInstance = new(pBuf) CResourceManager;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}
	pBuf += sizeof(CResourceManager);

	// Create instances of CResource.
	for (UN_UINT i = 0; i < nInitialCreateRscNum; i++) {
		CResource* pRsc = new(pBuf) CResource;

		// TODO
		pInstance->AddFreeResrouce(pRsc);

		pBuf += sizeof(CResource);
	}

	return pInstance;
}

/**
* Add resource.
*/
CResource* CResourceManager::Add(
	CObject* pObj,
	const CKey& cKey,
	const CRscType& cType,
	UN_BOOL bIsImmediatelyRelease/*= UN_TRUE*/)
{
	// Get free resource from FreeList.
	CResource* pRsc = GetFreeResource();
	if (pRsc == UN_NULL) {
		// Create a new resource.
		pRsc = CreateResource();
	}

	VRETURN_NULL(pRsc != UN_NULL);

	// Initialize resource.
	UN_BOOL result = pRsc->Init(
						this,
						pObj,
						cKey,
						cType);
	VRETURN_NULL(result);

	result = m_RscHash.Add(pRsc->GetHashItem());
	VRETURN_NULL(result);

	pRsc->SetIsImmediatelyRelease(bIsImmediatelyRelease);

	return pRsc;
}

/**
* Remove a specified resource by key.
*/
UN_BOOL CResourceManager::Remove(const CKey& cKey)
{
	CRscHashItem* pItem = m_RscHash.Find(cKey);
	VRETURN(pItem != UN_NULL);

	RemoveResource(pItem);

	return UN_TRUE;
}

/**
* Remove all resources.
*/
void CResourceManager::RemoveAll()
{
	CRscHashItem* pItem = UN_NULL;

	while((pItem = m_RscHash.GetOrderTop()) != UN_NULL) {
		RemoveResource(pItem);
	}

	m_RscHash.Clear();
}

/**
* Find a specified resource by key.
*/
CResource* CResourceManager::Find(const CKey& cKey)
{
	CResource* ret = m_RscHash.FindData(cKey);
	UN_ASSERT(ret != UN_NULL);

	return ret;
}

/**
* Remove resource that is not refered.
*/
void CResourceManager::Clean()
{
	CRscHashItem* pItem = UN_NULL;

	while((pItem = m_RscHash.GetOrderTop()) != UN_NULL) {
		CResource* pRsc = pItem->GetData();
		UN_ASSERT(pRsc != UN_NULL);

		CObject* pObj = pRsc->GetData();
		UN_ASSERT(pObj != UN_NULL);

		if (pObj->GetRefCnt() == 1) {
			// NOTE
			// If reference count is 1, this object is not refered.
			// This object is here only.
			pRsc->Clear();
			AddFreeResrouce(pRsc);
		}
	}
}

// Remove a resource
void CResourceManager::RemoveResource(CRscHashItem* pItem)
{
	UN_ASSERT(pItem != UN_NULL);

	CResource* pRsc = pItem->GetData();
	UN_ASSERT(pRsc != UN_NULL);

	pRsc->Clear();
	AddFreeResrouce(pRsc);
}

// Get a free resource from FreeList.
CResource* CResourceManager::GetFreeResource()
{
	CFreeRscListItem* pItem = m_FreeRscList.GetTop();
	if (pItem == UN_NULL) {
		// Nothing is in FreeList.
		return UN_NULL;
	}

	// Leave from FreeList.
	pItem->Leave();

	return pItem->GetData();
}

// Add a free resource to FreeList.
void CResourceManager::AddFreeResrouce(CResource* pRsc)
{
	UN_ASSERT(pRsc != UN_NULL);

	pRsc->Clear();

	m_FreeRscList.AddTail(pRsc->GetFreeListItem());
}

// Create an empty CReource instance.
CResource* CResourceManager::CreateResource()
{
	UN_ASSERT(m_pAllocator != UN_NULL);

	CResource* ret = CreateResource(m_pAllocator);

	// TODO

	return ret;
}

// Create an empty CReource instance.
CResource* CResourceManager::CreateResource(IMemoryAllocator* pAllocator)
{
	UN_ASSERT(pAllocator != UN_NULL);

	// Allocate memory.
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CResource));
	VRETURN_NULL(pBuf != UN_NULL);

	CResource* ret = new(pBuf) CResource;

	// TODO

	return ret;
}