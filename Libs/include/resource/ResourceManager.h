#if !defined(__IZANAGI_RESOURCE_RESOURCE_MANAGER_H__)
#define __IZANAGI_RESOURCE_RESOURCE_MANAGER_H__

#include "izStd.h"

namespace izanagi {
	class CResource;

	typedef CStdString<IZ_CHAR, 7>	CRscType;

	/**
	*/
	class CResourceManager : public CObject {
		friend class CResource;

		enum {
			HASH_SIZE = 5,
		};

		typedef CStdHash<CKey, CResource, HASH_SIZE>	CRscHash;
		typedef CRscHash::Item	CRscHashItem;

		typedef CStdList<CResource>	CFreeRscList;
		typedef CFreeRscList::Item	CFreeRscListItem;

	public:
		// Create an instance of CResourceManager.
		static CResourceManager* CreateResourceManager(
			IMemoryAllocator* pAllocator,
			IZ_UINT nInitialCreateRscNum);

	protected:
		CResourceManager() { m_Allocator = IZ_NULL; }
		~CResourceManager() { DeleteAll(); }

		NO_COPIABLE(CResourceManager);

	public:
		// Add a resource.
		CResource* Add(
			CObject* pObj,
			const CKey& cKey,
			const CRscType& cType);

		// Remove a specified resource.
		CResource* Remove(const CKey& cKey);

		// Remove all resources.
		void RemoveAll();

		// Delete a specified resource.
		IZ_BOOL Delete(const CKey& cKey);

		// Delete all resources.
		void DeleteAll();

		// Find a specified resource.
		CResource* Find(const CKey& cKey);

		// Return sum of registered resources.
		IZ_UINT GetResourceNum() const;
		IZ_UINT GetResourceNumByType(IZ_PCSTR pszType) const;
		IZ_UINT GetResourceNumByType(const CRscType& cType) const;

#if 0
		// Remove all resources that is not refered.
		void Clean();
#endif

	protected:
		// Create instances of CResource.
		void* CreateInitialRsc(
			IZ_UINT8* pBuf,
			IZ_UINT nInitialCreateRscNum);

		// Remove a resource
		void RemoveResource(
			CRscHashItem* pItem,
			IZ_BOOL bIsDelete);

		// Get a free resource from FreeList.
		CResource* GetFreeResource();

		// Add a free resource to FreeList.
		void AddFreeResrouce(CResource* pRsc);

		// Create an empty CReource instance.
		CResource* CreateResource();
		CResource* CreateResource(IMemoryAllocator* pAllocator);

		IZ_DEFINE_INTERNAL_RELEASE();

	protected:
		IMemoryAllocator* m_Allocator;

		CRscHash m_RscHash;
		CFreeRscList m_FreeRscList;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_RESOURCE_RESOURCE_MANAGER_H__)
