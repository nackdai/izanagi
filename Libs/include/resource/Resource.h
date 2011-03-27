#if !defined(__IZANAGI_RESOURCE_RESOURCE_H__)
#define __IZANAGI_RESOURCE_RESOURCE_H__

#include "std/StdPlacementNew.h"
#include "std/StdObject.h"
#include "std/allocator/MemoryAllocator.h"
#include "ResourceManager.h"

namespace izanagi {
	/**
	*/
	class CResource : public CPlacementNew {
		friend class CResourceManager;

	protected:
		CResource();
		~CResource() { Clear(); }

		NO_COPIABLE(CResource);

	protected:
		IZ_BOOL Init(
			CResourceManager* pRscMgr,
			CObject* pData,
			const CKey& cKey,
			const CRscType& cType);

		void Clear();

		void ReleaseResource();

		// Return whether this has data.
		IZ_BOOL IsValid() const { return (m_pData != IZ_NULL); }

		void SetAllocator(IMemoryAllocator* pAllocator) { m_pAllocator = pAllocator; }
		IMemoryAllocator* GetAllocator() { return m_pAllocator; }

		CResourceManager::CRscHashItem* GetHashItem() { return &m_HashItem; }
		CResourceManager::CFreeRscListItem* GetFreeListItem() { return &m_FreeListItem; }

	public:
		// Return resource type.
		const CRscType& GetType() const { return m_Type; }
		IZ_PCSTR GetTypeAsString() const { return m_Type.GetString(); }

		// Return resource raw data.
		CObject* GetData() { return m_pData; }

		// Return resource raw data.
		template <typename _T>
		_T* GetData() { return reinterpret_cast<_T*>(m_pData); }

		// Return resource id.
		const CKey& GetKey() const { m_HashItem.GetKey(); }

	protected:
		enum {
			STR_SIZE = 7,
		};

		IMemoryAllocator* m_pAllocator;

		CRscType m_Type;
		CObject* m_pData;

		CResourceManager::CRscHashItem m_HashItem;
		CResourceManager::CFreeRscListItem m_FreeListItem;

		CResourceManager* m_pRscMgr;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_RESOURCE_RESOURCE_H__)
