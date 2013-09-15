#if !defined(__IZANAGI_RESOURCE_RESOURCE_MANAGER_H__)
#define __IZANAGI_RESOURCE_RESOURCE_MANAGER_H__

#include "izStd.h"

namespace izanagi
{
namespace resource
{
    class CResource;

    /**
     */
    class CResourceManager : public CObject {
        friend class CResource;

        class CRscTypeManager;

        enum {
            HASH_NUM = 5,
        };

        typedef CStdHash<CKey, CResource, HASH_NUM> RscHash;
        typedef RscHash::Item RscHashItem;

        typedef CStdHash<IZ_UINT, CRscTypeManager, HASH_NUM> RscTypeMgrHash;
        typedef RscTypeMgrHash::Item RscTypeMgrHashItem;

    public:
        /** Create an instance of CResourceManager.
         */
        static CResourceManager* CreateResourceManager(IMemoryAllocator* allocator);

    protected:
        CResourceManager();
        virtual ~CResourceManager();

        NO_COPIABLE(CResourceManager);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** Add a resource.
         */
        IZ_BOOL Add(
            CObject* obj,
            IZ_UINT type,
            const CKey& key);

        /** Remove a specified resource.
         */
        IZ_BOOL Remove(
            IZ_UINT type,
            const CKey& key);

        /** Remove a specified resource.
         */
        IZ_BOOL Remove(const CKey& key);

        /** Remove all resources.
         */
        void RemoveAll();

        /** Find a specified resource.
         */
        CObject* Find(
            IZ_UINT type,
            const CKey& key);

        /** Find a specified resource.
         */
        CObject* Find(
            const CKey& key,
            IZ_UINT* type);

        /** Return sum of registered resources.
         */
        IZ_UINT GetResourceNum();

        /** Return sum of registered resources.
         */
        IZ_UINT GetResourceNumByType(IZ_UINT type);

    private:
        CResource* CreateResource(IZ_UINT type, const CKey& key);

        void DeleteResource(CResource* resource);

        CRscTypeManager* GetRscTypeManager(IZ_UINT type);

    private:
        IMemoryAllocator* m_Allocator;

        CStdHash<IZ_UINT, CRscTypeManager, HASH_NUM> m_RscHash;
        RscHash m_FreeRsc;
    };
}   // namespace resource
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_RESOURCE_RESOURCE_MANAGER_H__)
