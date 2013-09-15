#if !defined(__IZANAGI_RESOURCE_RESOURCE_H__)
#define __IZANAGI_RESOURCE_RESOURCE_H__

#include "izStd.h"
#include "resource/ResourceManager.h"

namespace izanagi
{
namespace resource
{
    class CResource : public CPlacementNew {
        friend class CResourceManager;

        static CResource* CreateResource(
            IMemoryAllocator* allocator,
            IZ_UINT type,
            const CKey& key);

    private:
        CResource(IZ_UINT type, const CKey& key);
        virtual ~CResource();

    private:
        void SetObject(CObject* object);
        CObject* GetObject();

        void Init(IZ_UINT type, const CKey& key);
        void Clear();

        IZ_UINT GetType() { return m_Type; }

    private:
        CResourceManager::RscHash::Item* GetHashItem() { return &m_HashItem; }

    private:
        CObject* m_Object;

        IZ_UINT m_Type;

        CResourceManager::RscHash::Item m_HashItem;
    };
}   // namespace resource
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_RESOURCE_RESOURCE_H__)
