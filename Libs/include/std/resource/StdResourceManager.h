#if !defined(__URANUS_RESOURCE_RESOURCE_MANAGER_H__)
#define __URANUS_RESOURCE_RESOURCE_MANAGER_H__

#include "std/StdObject.h"
#include "std/StdString.h"
#include "std/container/StdHash.h"

namespace uranus {
	class CResource;

	typedef CStdString<UN_CHAR, 7>	CRscType;

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
			UN_UINT nInitialCreateRscNum);

	protected:
		CResourceManager();
		~CResourceManager();

		NO_COPIABLE(CResourceManager);

	public:
		// Add a resource.
		CResource* Add(
			CObject* pObj,
			const CKey& cKey,
			const CRscType& cType,
			UN_BOOL bIsImmediatelyRelease = UN_TRUE);

		// Remove a specified resource.
		UN_BOOL Remove(const CKey& cKey);

		// Remove all resources.
		void RemoveAll();

		// Find a specified resource.
		CResource* Find(const CKey& cKey);

		// Return sum of registered resources.
		UN_UINT GetResourceNum() const;
		UN_UINT GetResourceNumByType(UN_PCSTR pszType) const;
		UN_UINT GetResourceNumByType(const CRscType& cType) const;

		// Remove all resources that is not refered.
		void Clean();

	protected:
		// Remove a resource
		void RemoveResource(CRscHashItem* pItem);

		// Get a free resource from FreeList.
		CResource* GetFreeResource();

		// Add a free resource to FreeList.
		void AddFreeResrouce(CResource* pRsc);

		// Create an empty CReource instance.
		CResource* CreateResource();
		CResource* CreateResource(IMemoryAllocator* pAllocator);

		UN_DEFINE_INTERNAL_RELEASE();

	protected:
		IMemoryAllocator* m_pAllocator;

		CRscHash m_RscHash;
		CFreeRscList m_FreeRscList;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_RESOURCE_RESOURCE_MANAGER_H__)
