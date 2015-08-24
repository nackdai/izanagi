#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__

#include <atomic>
#include "izStd.h"
#include "izSystem.h"

namespace izanagi {
namespace net {
    class ReplicatedObjectBase;
    class CClass;

    /**
     */
    class ReplicatedPropertyManager {
        friend class ReplicatedObjectBase;
        template <IZ_BOOL IS_SERVER> friend class ReplicatedPropertyManagerFactory;

    protected:
        static IMemoryAllocator* s_Allocator;
        static std::atomic<IZ_UINT64> s_ID;

        static IZ_UINT64 genID();

        static IZ_BOOL isInitialized();

    public:
        static void begin(
            IZ_BOOL isServer,
            IMemoryAllocator* allocator);

        static ReplicatedPropertyManager* get();

        static void end();

    protected:
        ReplicatedPropertyManager() {}
        virtual ~ReplicatedPropertyManager() {}

    public:
        using ObjectHash = CStdHash<IZ_UINT64, ReplicatedObjectBase, 4>;
        using HashItem = ObjectHash::Item;

        PURE_VIRTUAL(void update());

        using ReplicatedObjectCreator = std::function < ReplicatedObjectBase*(IMemoryAllocator*) >;

        virtual IZ_BOOL registerCreator(const CClass& clazz, ReplicatedObjectCreator func) { return IZ_FALSE; }

        IZ_UINT getObjectNum();

        ReplicatedObjectBase* getObject(IZ_UINT idx);

        ReplicatedObjectBase* popObject();

        using OnCreateObject = std::function < void(const CClass& clazz, ReplicatedObjectBase*) > ;

        void setOnCreateObject(OnCreateObject func);

    protected:
        PURE_VIRTUAL(IZ_BOOL isServer());

        virtual void add(ReplicatedObjectBase& obj) {}
        virtual void remove(ReplicatedObjectBase& obj) {}

    protected:
        ObjectHash m_hash;

        sys::CSpinLock m_locker;

        OnCreateObject m_onCreateObject{ nullptr };
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
