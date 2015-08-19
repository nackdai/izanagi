#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__

#include <atomic>
#include "izStd.h"

namespace izanagi {
namespace net {
    class ReplicatedObjectBase;

    /**
     */
    class ReplicatedPropertyManager {
        friend class ReplicatedObjectBase;
        template <IZ_BOOL IS_SERVER> friend class ReplicatedPropertyManagerFactory;

        static IMemoryAllocator* s_Allocator;
        static std::atomic<IZ_UINT64> s_ID;

        static IZ_UINT64 genID();

        static ReplicatedPropertyManager* create(IZ_BOOL isServer);
        static void finish();
        static ReplicatedPropertyManager* get();

        static void updateManager();

        static void setAllocator(IMemoryAllocator* allocator);

    private:
        ReplicatedPropertyManager() {}
        virtual ~ReplicatedPropertyManager() {}

    public:
        typedef CStdHash<IZ_UINT64, ReplicatedObjectBase, 4> ObjectHash;
        typedef ObjectHash::Item HashItem;

        PURE_VIRTUAL(void update());

    protected:
        PURE_VIRTUAL(IZ_BOOL isServer());

        void add(ReplicatedObjectBase* obj);

    protected:
        ObjectHash m_hash;
    };

    /**
     */
    template <IZ_BOOL IS_SERVER>
    class ReplicatedPropertySystem {
    public:
        static void begin(IMemoryAllocator* allocator)
        {
            ReplicatedPropertyManager::setAllocator(allocator);
            ReplicatedPropertyManager::create(IS_SERVER);
        }

        static ReplicatedPropertySystem* get()
        {
            return ReplicatedPropertyManager::get();
        }

        static void end()
        {
            ReplicatedPropertyManager::finish();
        }

        static void update()
        {
            ReplicatedPropertyManager::updateManager();
        }

    private:
        ReplicatedPropertySystem();
        ~ReplicatedPropertySystem();
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
