#include "std/resource/StdResource.h"

using namespace uranus;

CResource::CResource()
{
	m_pData = UN_NULL;
	m_ReleaseHandler.Init(this);
	m_pRscMgr = UN_NULL;

	m_Flags.isImmediatelyRelease = UN_TRUE;
}

UN_BOOL CResource::Init(
	CResourceManager* pRscMgr,
	CObject* pData,
	const CKey& cKey,
	const CRscType& cType)
{
	UN_ASSERT(pData != UN_NULL);

	m_Type = cType;
	SAFE_REPLACE(m_pData, pData);

	m_HashItem.Init(cKey, this);
	m_FreeListItem.Init(this);

	m_pRscMgr = pRscMgr;

	return UN_TRUE;
}

void CResource::Clear()
{
	m_Type.Clear();
	SAFE_RELEASE(m_pData);
	m_HashItem.Leave();
	m_FreeListItem.Leave();
}

void CResource::ReleaseResource()
{
	UN_ASSERT(m_pRscMgr != UN_NULL);
	m_pRscMgr->RemoveResource(GetHashItem());
}
