#include "resource/ResourceManager.h"
#include "resource/Resource.h"

using namespace izanagi;

/**
* Create an instance of CResourceManager.
*/
CResourceManager* CResourceManager::CreateResourceManager(
	IMemoryAllocator* pAllocator,
	IZ_UINT nInitialCreateRscNum)
{
	IZ_ASSERT(pAllocator != IZ_NULL);

	// Compute allocated size.
	size_t nSize = sizeof(CResourceManager);
	nSize += sizeof(CResource) * nInitialCreateRscNum;

	// Allocate memory.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	// Create an instance.
	CResourceManager* pInstance = new(pBuf) CResourceManager;
	{
		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;
	}
	pBuf += sizeof(CResourceManager);

	// Create instances of CResource.
	pInstance->CreateInitialRsc(
		pBuf,
		nInitialCreateRscNum);

	return pInstance;
}

/**
* Add resource.
*/
CResource* CResourceManager::Add(
	CObject* pObj,
	const CKey& cKey,
	const CRscType& cType)
{
	// Get free resource from FreeList.
	CResource* pRsc = GetFreeResource();
	if (pRsc == IZ_NULL) {
		// Create a new resource.
		pRsc = CreateResource(m_Allocator);
	}

	VRETURN_NULL(pRsc != IZ_NULL);

	// Initialize resource.
	IZ_BOOL result = pRsc->Init(
						this,
						pObj,
						cKey,
						cType);
	VRETURN_NULL(result);

	result = m_RscHash.Add(pRsc->GetHashItem());
	VRETURN_NULL(result);

	return pRsc;
}

/**
* Remove a specified resource by key.
*/
CResource* CResourceManager::Remove(const CKey& cKey)
{
	CRscHashItem* pItem = m_RscHash.Find(cKey);
	VRETURN(pItem != IZ_NULL);

	CResource* ret = pItem->GetData();

	RemoveResource(pItem, IZ_FALSE);

	return ret;
}

/**
* Remove all resources.
*/
void CResourceManager::RemoveAll()
{
	CRscHashItem* pItem = IZ_NULL;

	while((pItem = m_RscHash.GetOrderTopHashItem()) != IZ_NULL) {
		RemoveResource(pItem, IZ_FALSE);
	}

	m_RscHash.Clear();
}

/**
* Delete a specified resource.
*/
IZ_BOOL CResourceManager::Delete(const CKey& cKey)
{
	CRscHashItem* pItem = m_RscHash.Find(cKey);
	VRETURN(pItem != IZ_NULL);

	RemoveResource(pItem, IZ_TRUE);

	return IZ_TRUE;
}

/**
* Delete all resources.
*/
void CResourceManager::DeleteAll()
{
	CRscHashItem* pItem = IZ_NULL;

	while((pItem = m_RscHash.GetOrderTopHashItem()) != IZ_NULL) {
		RemoveResource(pItem, IZ_TRUE);
	}

	m_RscHash.Clear();
}

/**
* Find a specified resource by key.
*/
CResource* CResourceManager::Find(const CKey& cKey)
{
	CResource* ret = m_RscHash.FindData(cKey);
	//IZ_ASSERT(ret != IZ_NULL);

	return ret;
}

#if 0
/**
* Remove resource that is not refered.
*/
void CResourceManager::Clean()
{
	CRscHashItem* pItem = IZ_NULL;

	while((pItem = m_RscHash.GetOrderTop()) != IZ_NULL) {
		CResource* pRsc = pItem->GetData();
		IZ_ASSERT(pRsc != IZ_NULL);

		CObject* pObj = pRsc->GetData();
		IZ_ASSERT(pObj != IZ_NULL);

		if (pObj->GetRefCnt() == 1) {
			// NOTE
			// If reference count is 1, this object is not refered.
			// This object is here only.
			pRsc->Clear();
			AddFreeResrouce(pRsc);
		}
	}
}
#endif

// Create instances of CResource.
void* CResourceManager::CreateInitialRsc(
	IZ_UINT8* pBuf,
	IZ_UINT nInitialCreateRscNum)
{
	for (IZ_UINT i = 0; i < nInitialCreateRscNum; ++i) {
		CResource* pRsc = new(pBuf) CResource;

		AddFreeResrouce(pRsc);

		pBuf += sizeof(CResource);
	}

	return pBuf;
}

// Remove a resource
void CResourceManager::RemoveResource(
	CRscHashItem* pItem,
	IZ_BOOL bIsDelete)
{
	IZ_ASSERT(pItem != IZ_NULL);

	CResource* pRsc = pItem->GetData();
	IZ_ASSERT(pRsc != IZ_NULL);

	if (bIsDelete) {
		IMemoryAllocator* pAllocator = pRsc->GetAllocator();
		
		delete pRsc;
		if (pAllocator != IZ_NULL) {
			FREE(pAllocator, pRsc);
		}
		return;
	}

	pRsc->Clear();
	AddFreeResrouce(pRsc);
}

// Get a free resource from FreeList.
CResource* CResourceManager::GetFreeResource()
{
	CFreeRscListItem* pItem = m_FreeRscList.GetTop();
	if (pItem == IZ_NULL) {
		// Nothing is in FreeList.
		return IZ_NULL;
	}

	// Leave from FreeList.
	pItem->Leave();

	return pItem->GetData();
}

// Add a free resource to FreeList.
void CResourceManager::AddFreeResrouce(CResource* pRsc)
{
	IZ_ASSERT(pRsc != IZ_NULL);

	pRsc->Clear();

	m_FreeRscList.AddTail(pRsc->GetFreeListItem());
}

// Create an empty CReource instance.
CResource* CResourceManager::CreateResource()
{
	IZ_ASSERT(m_Allocator != IZ_NULL);

	CResource* ret = CreateResource(m_Allocator);

	ret->SetAllocator(IZ_NULL);

	return ret;
}

// Create an empty CReource instance.
CResource* CResourceManager::CreateResource(IMemoryAllocator* pAllocator)
{
	IZ_ASSERT(pAllocator != IZ_NULL);

	// Allocate memory.
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CResource));
	VRETURN_NULL(pBuf != IZ_NULL);

	CResource* ret = new(pBuf) CResource;

	ret->SetAllocator(pAllocator);

	return ret;
}
