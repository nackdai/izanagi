#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__

#include <atomic>
#include "izStd.h"
#include "izSystem.h"

namespace izanagi {
namespace net {
    class ReplicatedObjectBase;
    class CClass;

    /** Manager for replicated properties.
     */
    class ReplicatedPropertyManager {
        friend class ReplicatedObjectBase;
        friend class ReplicatedObjectProxy;
        template <IZ_BOOL IS_SERVER> friend class ReplicatedPropertyManagerFactory;

    protected:
        static IMemoryAllocator* s_Allocator;
        static std::atomic<IZ_UINT64> s_ID;

        static IZ_UINT64 genID();

        static IZ_BOOL isInitialized();

    public:
        /** Begin replicated property system.
         */
        static ReplicatedPropertyManager* begin(
            IZ_BOOL isServer,
            IMemoryAllocator* allocator);

        /** Get ReplicatedPropertyManager.
         */
        static ReplicatedPropertyManager* get();

        /** End replicated property system.
         */
        static void end();

    protected:
        ReplicatedPropertyManager() {}
        virtual ~ReplicatedPropertyManager() {}

    public:
        using ObjectHash = CStdHash<IZ_UINT64, ReplicatedObjectBase, 4>;
        using HashItem = ObjectHash::Item;

        /** Update replicated property system.
         */
        PURE_VIRTUAL(void update());

        /** Get number of replicated object which is managed.
         */
        IZ_UINT getObjectNum();

        /** Get pointer to a replicated object by index.
         */
        ReplicatedObjectBase* getObject(IZ_UINT idx);

        /** Get if this is on server.
         */
        PURE_VIRTUAL(IZ_BOOL isServer());

    protected:
        using ReplicatedObjectCreator = std::function < ReplicatedObjectBase*(IMemoryAllocator*) >;

        virtual IZ_BOOL registerCreator(const CClass& clazz, ReplicatedObjectCreator func) { return IZ_FALSE; }

        virtual void add(ReplicatedObjectBase& obj) {}
        virtual void remove(ReplicatedObjectBase& obj) {}

    protected:
        ObjectHash m_hash;

        sys::CSpinLock m_locker;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
