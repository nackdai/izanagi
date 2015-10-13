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

    class ReplicatedPropertyManagerListener;

    /** Manager for replicated properties.
     */
    class ReplicatedPropertyManager {
        friend class ReplicatedObjectBase;
        template <typename TYPE> friend class ReplicatedObjectProxy;
        template <IZ_BOOL IS_SERVER> friend class ReplicatedPropertyManagerFactory;

    public:
        class ObjectCreatorBase;

    private:
        using CreatorHash = CStdHash < ReplicatedObjectClass, ObjectCreatorBase, 4 >;
        using CreatorHashItem = CreatorHash::Item;

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

        /** Get ReplicatedPropertyManagerListener.
         */
        static ReplicatedPropertyManagerListener* getListener();

        /** End replicated property system.
         */
        static void end();

    protected:
        ReplicatedPropertyManager() {}
        virtual ~ReplicatedPropertyManager() {}

    public:
        /** Initialize replicated property system.
         */
        virtual IZ_BOOL init(const IPv4Endpoint& ep) = 0;

        /** Create an object which has replicated properties.
         */
        ReplicatedObjectBase* create(const ReplicatedObjectClass& clazz);

        /** Delete an object which has replicated properties.
         */
        virtual void deleteObject(ReplicatedObjectBase* obj);

        using CreatedReplicatedObjectHandler = std::tuple<ReplicatedObjectClass, ReplicatedObjectBase*>;

        virtual CreatedReplicatedObjectHandler pop();

        void remove(ReplicatedObjectBase& obj);

        /** Update replicated property system.
         */
        PURE_VIRTUAL(void update());

        /** Get number of replicated object which is managed.
         */
        IZ_UINT getObjectNum();

        /** Get if this is on server.
         */
        PURE_VIRTUAL(IZ_BOOL isServer());

    public:
        using ReplicatedObjectCreator = std::function < ReplicatedObjectBase*(IMemoryAllocator*) >;

        // Class to create/delete an object.
        class ObjectCreatorBase {
            friend class ReplicatedPropertyManager;

        public:
            ObjectCreatorBase() {}
            virtual ~ObjectCreatorBase() {}

            virtual ReplicatedObjectBase* create(IMemoryAllocator* allocator) = 0;
            virtual void deleteObject(IMemoryAllocator* allocator, ReplicatedObjectBase* obj) = 0;

        private:
            CreatorHashItem m_item;
        };

        // Class to create/delete a specified object.
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

        // Regist creator to create/delete an object which have replicated properties.
        void registerCreator(const ReplicatedObjectClass& clazz, ObjectCreatorBase* creator);

    protected:
        virtual void OnCreate(ReplicatedObjectBase* cretatedObj) = 0;

    protected:
        sys::CSpinLock m_locker;
        CStdList<ReplicatedObjectBase> m_list;

        std::mutex m_hashCreatorLocker;
        CreatorHash m_hashCreator;
    };

    class ReplicatedPropertyManagerListener : public ReplicatedPropertyManager
    {
    protected:
        ReplicatedPropertyManagerListener() {}
        virtual ~ReplicatedPropertyManagerListener() {}
        
    public:
        virtual void accept(IZ_UINT acceptedNum, IZ_FLOAT timeout) = 0;
    };
}    // namespace net
}    // namespace izanagi

#define IZ_CREATE_REPLIACTED_OBJECT(mgr, clazz) (clazz *)mgr->create(clazz::Class##clazz())

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
