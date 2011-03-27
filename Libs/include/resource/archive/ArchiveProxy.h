#if !defined(__IZANAGI_ARCHIVE_ARCHIVE_PROXY_H__)
#define __IZANAGI_ARCHIVE_ARCHIVE_PROXY_H__

#include "izDefs.h"
#include "izStd.h"
#include "Archive.h"
#include "resource/Resource.h"

namespace izanagi {
	class CGraphicsDevice;
	class CCreateRscListener;

	/**
	*/
	class CArchiveProxy : public CObject {
		friend class CCreateRscListener;

		enum {
			HASH_SIZE = 5,
		};

		typedef CStdHash<CRscType, CCreateRscListener, HASH_SIZE>	CCreateRscListenerHash;
		typedef CCreateRscListenerHash::Item	CCreateRscListenerHashItem;

	public:
		// Create instance of CArchiveProxy.
		static CArchiveProxy* CreateArchiveProxy(
			IMemoryAllocator* pAllocator,
			IArchive* pArchive);

	protected:
		CArchiveProxy();
		~CArchiveProxy();

		NO_COPIABLE(CArchiveProxy);

	public:
		// Add listener for creating resource.
		IZ_BOOL AddCreateRscListener(
			const CRscType& type,
			CCreateRscListener* pListener);

		// Return whether listener for creating resource is exist.
		IZ_BOOL IsExistCreateRscListener(const CRscType& type);

		// Return resource.
		void* CreateResource(
			IZ_UINT nKey,
			IMemoryAllocator* pAllocator, 
			CGraphicsDevice* pDevice);

		template <typename _T>
		_T* CreateResource(
			IZ_UINT nKey,
			IMemoryAllocator* pAllocator, 
			CGraphicsDevice* pDevice)
		{
			void* ret = CreateResource(nKey, pAllocator, pDevice);
			IZ_ASSERT(ret != IZ_NULL);
			return reinterpret_cast<_T*>(ret);
		}

		// Find resource.
		void* Find(IZ_UINT nKey);

		template <typename _T>
		_T* Find(IZ_UINT nKey)
		{
			void* ret = Find(nKey);
			return reinterpret_cast<_T*>(ret);
		}

		// Return current file's description.
		IZ_BOOL GetFileDesc(SArchiveFileDesc* pDesc)
		{
			IZ_ASSERT(m_pArchive != IZ_NULL);
			return m_pArchive->GetFileDesc(pDesc);
		}

		// Return instance of CResourceManager.
		CResourceManager* GetResourceManager() { return &m_RscMgr; }

		// Return instance of CArchive.
		IArchive* GetArchive() { return m_pArchive; }

	protected:
		IZ_DEFINE_INTERNAL_RELEASE();

		class CResourceManagerProxy : public CResourceManager {
		public:
			CResourceManagerProxy() {}
			~CResourceManagerProxy() {}

			void* Init(IZ_UINT8* pBuf, IZ_UINT nInitialCreateRscNum)
			{
				return CreateInitialRsc(pBuf, nInitialCreateRscNum);
			}

			static size_t ComputeRscSize(IZ_UINT nNum)
			{
				return sizeof(CResource) * nNum;
			}
		};

	protected:
		IMemoryAllocator* m_pAllocator;

		CResourceManagerProxy m_RscMgr;

		IArchive* m_pArchive;
		CCreateRscListenerHash m_CreateRscListenerHash;
	};

	/////////////////////////////////////////////////////////////////////

	/**
	*/
	class CCreateRscListener {
		friend class CArchiveProxy;

		NO_COPIABLE(CCreateRscListener);

	public:
		CCreateRscListener() {}
		virtual ~CCreateRscListener() {}

		PURE_VIRTUAL(CObject* CreateRsc(IMemoryAllocator* pAllocator, CGraphicsDevice* pDevice, IInputStream* pIn));

	protected:
		CArchiveProxy::CCreateRscListenerHashItem m_HashItem;

		CArchiveProxy::CCreateRscListenerHashItem* GetHashItem() { return &m_HashItem; }
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_ARCHIVE_ARCHIVE_PROXY_H__)
