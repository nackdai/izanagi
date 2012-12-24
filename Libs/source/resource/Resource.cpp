#include "resource/Resource.h"

using namespace izanagi;

CResource::CResource()
{
    m_Allocator = IZ_NULL;

    m_pData = IZ_NULL;
    m_pRscMgr = IZ_NULL;

    m_FreeListItem.Init(this);
}

IZ_BOOL CResource::Init(
    CResourceManager* pRscMgr,
    CObject* pData,
    const CKey& cKey,
    const CRscType& cType)
{
    IZ_ASSERT(pData != IZ_NULL);

    m_Type = cType;
    SAFE_REPLACE(m_pData, pData);

    m_HashItem.Init(cKey, this);
    m_FreeListItem.Init(this);

    m_pRscMgr = pRscMgr;

    return IZ_TRUE;
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
    IZ_ASSERT(m_pRscMgr != IZ_NULL);
    m_pRscMgr->RemoveResource(GetHashItem(), IZ_TRUE);
}
