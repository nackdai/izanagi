#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__

#include <atomic>
#include "izStd.h"
#include "izSystem.h"
#include "network/ReplicatedProperty/ReplicatedPropertyObjectClass.h"

namespace izanagi {
namespace net {
    class ReplicatedObjectBase;
    class IPv4Endpoint;

    /** Manager for replicated properties.
     */
    class ReplicatedPropertyManager {
        friend class ReplicatedObjectBase;
        template <typename TYPE> friend class ReplicatedObjectProxy;
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
        virtual IZ_BOOL init(const IPv4Endpoint& ep) = 0;

        virtual ReplicatedObjectBase* create(const ReplicatedObjectClass& clazz) { return nullptr; }
        virtual void deleteObject(ReplicatedObjectBase* obj);

        virtual std::tuple<const ReplicatedObjectClass&, ReplicatedObjectBase*> pop();

        /** Update replicated property system.
         */
        PURE_VIRTUAL(void update());

        /** Get number of replicated object which is managed.
         */
        IZ_UINT getObjectNum();

        /** Get if this is on server.
         */
        PURE_VIRTUAL(IZ_BOOL isServer());

    protected:
        using ReplicatedObjectCreator = std::function < ReplicatedObjectBase*(IMemoryAllocator*) >;

        class ObjectCreatorBase;
        using CreatorHash = CStdHash < ReplicatedObjectClass, ObjectCreatorBase, 4 >;
        using CreatorHashItem = CreatorHash::Item;

        class ObjectCreatorBase {
        public:
            ObjectCreatorBase() {}
            virtual ~ObjectCreatorBase() {}

            virtual ReplicatedObjectBase* create(IMemoryAllocator* allocator) = 0;
            virtual void deleteObject(IMemoryAllocator* allocator, ReplicatedObjectBase* obj) = 0;

            CreatorHashItem m_item;
        };

        template <typename _T>
        class ObjectCreator : public ObjectCreatorBase {
        public:
            ObjectCreator() {}
            virtual ~ObjectCreator() {}

            virtual ReplicatedObjectBase* create(IMemoryAllocator* allocator) override
            {
                void* p = ALLOC(allocator, sizeof(_T));
                _T* ret = new(p)_T();
                return ret;
            }
            virtual void deleteObject(IMemoryAllocator* allocator, ReplicatedObjectBase* obj) override
            {
                FREE(allocator, obj);
            }
        };

        virtual void registerCreator(const ReplicatedObjectClass& clazz, ObjectCreatorBase* creator) {}

    protected:
        CStdList<ReplicatedObjectBase> m_list;

        sys::CSpinLock m_locker;
    };
}    // namespace net
}    // namespace izanagi

#define IZ_CREATE_REPLIACTED_OBJECT(mgr, clazz) (clazz *)mgr->create(clazz::Class##clazz())

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
