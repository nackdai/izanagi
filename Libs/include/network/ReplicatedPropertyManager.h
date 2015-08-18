#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__

#include <atomic>
#include "ReplicatiedProperty.h"

namespace izanagi {
namespace net {
    class ReplicatedPropertyBase;
    template <typename _T> class ReplicatedProperty;

    class ReplicatedPropertyManager {
        template <typename _T> friend class ReplicatedProperty;

        static IZ_UINT64 s_ID;
        static ReplicatedPropertyManager s_instance;

        static ReplicatedPropertyManager& getInstance();

    private:
        ReplicatedPropertyManager();
        ~ReplicatedPropertyManager();

    public:
        IZ_BOOL start(
            IZ_BOOL isServer,
            IMemoryAllocator* allocator);

    private:
        inline IZ_UINT64 genID();

        template <typename _T>
        void init(ReplicatedProperty<_T>& prop);

        void send(IZ_UINT64 id, IZ_BOOL isReliable);

        IZ_BOOL isServer();

    private:
        struct SProperty;

        typedef CStdHash<IZ_UINT, SProperty, 4> Hash;
        typedef CStdList<SProperty> List;

        struct SProperty {
            IZ_UINT64 id;

            ReplicatedPropertyBase* ptr;

            Hash::Item hashItem;
            List::Item listItem;
        };

        IMemoryAllocator* m_allocator;

        Hash m_hash;

        List m_sendList;

        std::atomic<IZ_BOOL> m_isRunning;
        IZ_BOOL m_isServer;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_MANGER_H__)
