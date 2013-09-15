#include "resource/Resource.h"

namespace izanagi
{
namespace resource
{
    CResource* CResource::CreateResource(
        IMemoryAllocator* allocator,
        IZ_UINT type,
        const CKey& key)
    {
        void* buf = ALLOC(allocator, sizeof(CResource));
        VRETURN_NULL(buf != IZ_NULL);

        CResource* instance = new(buf) CResource(type, key);
        return instance;
    }

    CResource::CResource(IZ_UINT type, const CKey& key)
    {
        m_Object = IZ_NULL;

        m_Type = type;

        m_HashItem.Init(key, this);
    }

    CResource::~CResource()
    {
        SAFE_RELEASE(m_Object);
    }

    void CResource::SetObject(CObject* object)
    {
        SAFE_REPLACE(m_Object, object);
    }

    CObject* CResource::GetObject()
    {
        return m_Object;
    }

    void CResource::Init(IZ_UINT type, const CKey& key)
    {
        m_HashItem.Leave();

        m_Type = type;
        m_HashItem.Init(key, this);
    }

    void CResource::Clear()
    {
        SAFE_RELEASE(m_Object);

        m_Type = 0;

        m_HashItem.Leave();
        m_HashItem.Init(CKey::ZERO, this);
    }
}   // namespace resource
}   // namespace izanagi
