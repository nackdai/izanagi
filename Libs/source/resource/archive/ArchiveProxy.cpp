#include "resource/archive/ArchiveProxy.h"

using namespace izanagi;

/**
* Create instance of CArchiveProxy.
*/
CArchiveProxy* CArchiveProxy::CreateArchiveProxy(
	IMemoryAllocator* pAllocator,
	IArchive* pArchive)
{
	IZ_ASSERT(pAllocator != IZ_NULL);
	IZ_ASSERT(pArchive != IZ_NULL);

	IZ_UINT nInitialRscNum = pArchive->GetFileNum();

	// Compute memory size.
	size_t nSize = sizeof(CArchiveProxy);
	nSize += CResourceManagerProxy::ComputeRscSize(nInitialRscNum);

	// Allocate memory.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	CArchiveProxy* pInstance = new(pBuf) CArchiveProxy;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pArchive, pArchive);
	}
	pBuf += sizeof(CArchiveProxy);

	pInstance->m_RscMgr.Init(pBuf, nInitialRscNum);

	return pInstance;
}

CArchiveProxy::CArchiveProxy()
{
	m_pAllocator = IZ_NULL;
	m_pArchive = IZ_NULL;
}

CArchiveProxy::~CArchiveProxy()
{
	m_RscMgr.DeleteAll();
	SAFE_RELEASE(m_pArchive);
}

/**
* Add listener for creating resource.
*/
IZ_BOOL CArchiveProxy::AddCreateRscListener(
	const CRscType& type,
	CCreateRscListener* pListener)
{
	IZ_ASSERT(pListener != IZ_NULL);

	CCreateRscListenerHashItem* pItem = pListener->GetHashItem();
	pItem->Init(type, pListener);

	IZ_BOOL ret = m_CreateRscListenerHash.Add(pItem);
	return ret;
}

/**
* Return whether listener for creating resource is exist.
*/
IZ_BOOL CArchiveProxy::IsExistCreateRscListener(const CRscType& type)
{
	CCreateRscListenerHashItem* pItem = m_CreateRscListenerHash.Find(type);
	return (pItem != IZ_NULL);
}

namespace {
	// Get file's extension.
	inline IZ_PCSTR _GetFileExt(IZ_PCSTR name)
	{
		IZ_INT len = static_cast<IZ_INT>(strlen(name));

		for (IZ_INT i = len - 1; i >= 0; i--) {
			IZ_CHAR ch = name[i];
			if (ch == '.') {
				return &name[i + 1];
			}
		}

		IZ_ASSERT(IZ_FALSE);
		return IZ_NULL;
	}
}	// namespace


/**
* Create resource.
*/
void* CArchiveProxy::CreateResource(
	IZ_UINT nKey,
	IMemoryAllocator* pAllocator, 
	CGraphicsDevice* pDevice)
{
	void* pFindData = Find(nKey);
	if (pFindData != IZ_NULL) {
		return pFindData;
	}

	IZ_ASSERT(m_pArchive != IZ_NULL);
	IZ_ASSERT(pAllocator != IZ_NULL);

	// Seek file by key.
	IZ_BOOL result = m_pArchive->Seek(nKey);
	VRETURN_NULL(result);

	// Get file's description.
	SArchiveFileDesc sDesc;
	VRETURN_NULL(m_pArchive->GetFileDesc(&sDesc));

	// Get file's extension.
	IZ_PCSTR pszExt = _GetFileExt(sDesc.name);
	VRETURN_NULL(pszExt != IZ_NULL);

	// TODO
	// If file is compressed...

	CRscType cType(pszExt);

	// Get resource create listener.
	CCreateRscListenerHashItem* pItem = m_CreateRscListenerHash.Find(cType);
	VRETURN_NULL(pItem != IZ_NULL);

	CCreateRscListener* pListener = pItem->GetData();
	VRETURN_NULL(pListener != IZ_NULL);

	// Create resource.
	CObject* pObj = pListener->CreateRsc(
						(pAllocator == IZ_NULL ? m_pAllocator : pAllocator),
						pDevice,
						m_pArchive->GetInputStream());
	VRETURN_NULL(pObj != IZ_NULL);

	// Add to resource manager.
	CResource* pRsc = m_RscMgr.Add(
						pObj,
						CKey(nKey),
						cType);
	IZ_ASSERT(pRsc != IZ_NULL);

	CObject* ret = pObj;
	SAFE_RELEASE(pObj);

	return ret;
}

/**
* Find resource.
*/
void* CArchiveProxy::Find(IZ_UINT nKey)
{
	CResource* pRsc = m_RscMgr.Find(CKey(nKey));
	if (pRsc != IZ_NULL) {
		return pRsc->GetData();
	}

	return IZ_NULL;
}
