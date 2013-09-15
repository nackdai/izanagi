#include "resource/ResourceManager.h"
#include "resource/Resource.h"

namespace izanagi
{
namespace resource
{
    class CResourceManager::CRscTypeManager : public CPlacementNew
    {
        typedef CResourceManager::RscHash::Item ITEM;

        typedef CStdList<ITEM>::Item OrderItem;

    public:
        static CRscTypeManager* Create(
            IMemoryAllocator* allocator,
            IZ_UINT type)
        {
            void* buf = ALLOC(allocator, sizeof(CRscTypeManager));
            VRETURN_NULL(buf != IZ_NULL);

            CRscTypeManager* instance = new(buf) CRscTypeManager(type);

            return instance;
        }

    public:
        CRscTypeManager(IZ_UINT type)
        {
            m_HashItem.Init(type, this);
        }
        virtual ~CRscTypeManager()
        {
            IZ_ASSERT(m_Resources.GetDataNum() == 0);
        }

    public:
        void Add(CResource* resource)
        {
            m_Resources.Add(resource->GetHashItem());
        }

        IZ_BOOL Remove(
            const CKey& key,
            CResourceManager* mgr)
        {
            ITEM* item = m_Resources.Find(key);
            if (item != IZ_NULL) {
                CResource* resource = item->GetData();
                item->Leave();
                
                mgr->DeleteResource(resource);

                return IZ_TRUE;
            }

            return IZ_FALSE;
        }

        void RemoveAll(CResourceManager* mgr)
        {
            OrderItem* item = m_Resources.GetOrderTop();

            while (item != IZ_NULL) {
                CResource* resource = item->GetData()->GetData();
                item = item->GetNext();

                mgr->DeleteResource(resource);
            }
        }

        CResource* Find(const CKey& key)
        {
            ITEM* item = m_Resources.Find(key);
            if (item != IZ_NULL) {
                return item->GetData();
            }

            return IZ_NULL;
        }

        IZ_UINT GetResourceNum() const
        {
            return m_Resources.GetDataNum();
        }

        CResourceManager::RscTypeMgrHashItem* GetHashItem()
        {
            return &m_HashItem;
        }

    private:
        CResourceManager::RscHash m_Resources;

        CResourceManager::RscTypeMgrHashItem m_HashItem;
    };

    // Create an instance of CResourceManager.
    CResourceManager* CResourceManager::CreateResourceManager(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(CResourceManager));
        VRETURN_NULL(buf);

        CResourceManager* instance = new(buf) CResourceManager;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;
        }

        return instance;
    }

    CResourceManager::CResourceManager()
    {
        m_Allocator = IZ_NULL;
    }

    CResourceManager::~CResourceManager()
    {
        RemoveAll();

        CStdList<RscTypeMgrHashItem>::Item* mgrItem = m_RscHash.GetOrderTop();
        while (mgrItem != IZ_NULL) {
            CRscTypeManager* mgr = mgrItem->GetData()->GetData();
            mgrItem = mgrItem->GetNext();

            delete mgr;

            FREE(m_Allocator, mgr);
        }

        CStdList<RscHashItem>::Item* rscItem = m_FreeRsc.GetOrderTop();
        while(rscItem != IZ_NULL) {
            CResource* rsc = rscItem->GetData()->GetData();
            rscItem = rscItem->GetNext();

            delete rsc;

            FREE(m_Allocator, rsc);
        }
    }

    // Add a resource.
    IZ_BOOL CResourceManager::Add(
        CObject* obj,
        IZ_UINT type,
        const CKey& key)
    {
        CResource* resource = CreateResource(type, key);
        VRETURN(resource != IZ_NULL);

        CRscTypeManager* mgr = GetRscTypeManager(type);
        VRETURN(mgr != IZ_NULL);

        resource->SetObject(obj);

        mgr->Add(resource);

        return IZ_TRUE;
    }

    // Remove a specified resource.
    IZ_BOOL CResourceManager::Remove(
        IZ_UINT type,
        const CKey& key)
    {
        CRscTypeManager* mgr = GetRscTypeManager(type);
        VRETURN(mgr != IZ_NULL);

        IZ_BOOL result = mgr->Remove(key, this);

        return result;
    }

    // Remove a specified resource.
    IZ_BOOL CResourceManager::Remove(const CKey& key)
    {
        return Remove(0, key);
    }

    // Remove all resources.
    void CResourceManager::RemoveAll()
    {
        CStdList<RscTypeMgrHashItem>::Item* item = m_RscHash.GetOrderTop();

        while (item != IZ_NULL) {
            CRscTypeManager* mgr = item->GetData()->GetData();
            item = item->GetNext();

            mgr->RemoveAll(this);
        }
    }

    // Find a specified resource.
    CObject* CResourceManager::Find(
        IZ_UINT type,
        const CKey& key)
    {
        RscTypeMgrHashItem* item = m_RscHash.Find(type);
        VRETURN_NULL(item != IZ_NULL);

        CRscTypeManager* mgr = item->GetData();

        CResource* resource = mgr->Find(key);
        VRETURN_NULL(resource != IZ_NULL);

        return resource->GetObject();
    }

    // Find a specified resource.
    CObject* CResourceManager::Find(
        const CKey& key,
        IZ_UINT* type)
    {
        CStdList<RscTypeMgrHashItem>::Item* item = m_RscHash.GetOrderTop();
        VRETURN_NULL(item != IZ_NULL);

        while (item != IZ_NULL) {
            CRscTypeManager* mgr = item->GetData()->GetData();
            item = item->GetNext();

            CResource* resource = mgr->Find(key);
            if (resource != IZ_NULL) {
                if (type != IZ_NULL) {
                    *type = resource->GetType();
                }

                return resource->GetObject();
            }
        }

        return IZ_NULL;
    }

    // Return sum of registered resources.
    IZ_UINT CResourceManager::GetResourceNum()
    {
        IZ_UINT ret = 0;

        CStdList<RscTypeMgrHashItem>::Item* item = m_RscHash.GetOrderTop();
        IZ_ASSERT(item != IZ_NULL);

        while (item != IZ_NULL) {
            CRscTypeManager* mgr = item->GetData()->GetData();
            item = item->GetNext();

            ret += mgr->GetResourceNum();
        }

        return ret;
    }

    // Return sum of registered resources.
    IZ_UINT CResourceManager::GetResourceNumByType(IZ_UINT type)
    {
        RscTypeMgrHashItem* item = m_RscHash.Find(type);
        VRETURN_NULL(item != IZ_NULL);

        CRscTypeManager* mgr = item->GetData();

        return mgr->GetResourceNum();
    }

    CResource* CResourceManager::CreateResource(IZ_UINT type, const CKey& key)
    {
        CResource* resource = IZ_NULL;

        if (m_FreeRsc.GetDataNum() > 0) {
            CStdList<RscHash::Item>::Item* item = m_FreeRsc.GetOrderTop();
            IZ_ASSERT(item != IZ_NULL);

            resource = item->GetData()->GetData();
        }
        else {
            resource = CResource::CreateResource(
                m_Allocator,
                type,
                key);
        }

        return resource;
    }

    void CResourceManager::DeleteResource(CResource* resource)
    {
        resource->Clear();

        m_FreeRsc.Add(resource->GetHashItem());
    }

    CResourceManager::CRscTypeManager* CResourceManager::GetRscTypeManager(IZ_UINT type)
    {
        CRscTypeManager* ret = m_RscHash.FindData(type);

        if (ret == IZ_NULL) {
            ret = CRscTypeManager::Create(m_Allocator, type);
        }

        IZ_ASSERT(ret != IZ_NULL);

        if (ret != IZ_NULL) {
            m_RscHash.Add(ret->GetHashItem());
        }

        return ret;
    }

}   // namespace resource
}   // namespace izanagi
