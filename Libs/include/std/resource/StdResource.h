#if !defined(__URANUS_RESOURCE_RESOURCE_H__)
#define __URANUS_RESOURCE_RESOURCE_H__

#include "std/StdPlacementNew.h"
#include "std/StdObject.h"
#include "StdResourceManager.h"

namespace uranus {
	/**
	*/
	class CResource : public CPlacementNew {
		friend class CResourceManager;

	protected:
		CResource();
		~CResource() { Clear(); }

		NO_COPIABLE(CResource);

	protected:
		UN_BOOL Init(
			CResourceManager* pRscMgr,
			CObject* pData,
			const CKey& cKey,
			const CRscType& cType);

		void Clear();

		void ReleaseResource();

		// Return whether this has data.
		UN_BOOL IsValid() const { return (m_pData != UN_NULL); }

		CResourceManager::CRscHashItem* GetHashItem() { return &m_HashItem; }
		CResourceManager::CFreeRscListItem* GetFreeListItem() { return &m_FreeListItem; }

	public:
		// Return resource type.
		const CRscType& GetType() const { return m_Type; }
		UN_PCSTR GetTypeAsString() const { return m_Type.GetString(); }

		// Return resource raw data.
		CObject* GetData() { return m_pData; }

		// Return resource raw data.
		template <typename _T>
		_T* GetData() { return reinterpret_cast<_T*>(m_pData); }

		// Return resource id.
		const CKey& GetKey() const { m_HashItem.GetKey(); }

		// Set flag for enabling/disabling whether resource is released immediately if resource is not refered.
		void SetIsImmediatelyRelease(UN_BOOL flag) { m_Flags.isImmediatelyRelease = flag; }

		// Return whether resource is released immediately if resource is not refered.
		UN_BOOL IsImmediatelyRelease() const { return m_Flags.isImmediatelyRelease; }

	protected:
		class CReleaseHandler : public CObject::CReleaseHandler {
		public:
			CReleaseHandler() { m_pRsc = UN_NULL; }
			void Init(CResource* pRsc) { m_pRsc = pRsc; }
			void HandleRelease()
			{
				UN_ASSERT(m_pRsc != UN_NULL);
				if (m_pRsc->m_Flags.isImmediatelyRelease
					&& (m_pRsc->m_pData->GetRefCnt() == 1))
				{
					m_pRsc->ReleaseResource();
				}
			}

		protected:
			CResource* m_pRsc;
		};

	protected:
		enum {
			STR_SIZE = 7,
		};

		CRscType m_Type;
		CObject* m_pData;

		CReleaseHandler m_ReleaseHandler;

		CResourceManager::CRscHashItem m_HashItem;
		CResourceManager::CFreeRscListItem m_FreeListItem;

		CResourceManager* m_pRscMgr;

		struct {
			UN_UINT isImmediatelyRelease	: 1;
		} m_Flags;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_RESOURCE_RESOURCE_H__)
