#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__

#include "izDefs.h"
#include "izStd.h"
#include "network/ReplicatedPropertyManager.h"

namespace izanagi {
namespace net {
    class ReplicatedPropertyBase;

    /**
     */
    class ReplicatedObjectBase {
        friend class ReplicatedPropertyManager;
        friend class ReplicatedPropertyServer;
        friend class ReplicatedPropertyClient;
        template <typename _T> friend class ReplicatedProperty;

    protected:
        ReplicatedObjectBase();
        virtual ~ReplicatedObjectBase() {}

    private:
        void addReplicatedProperty(ReplicatedPropertyBase& prop);

        ReplicatedPropertyManager::HashItem* getReplicatedObjectHashItem();

    public:
        IZ_BOOL isServer() const
        {
            return m_isServer;
        }

    private:
        inline IZ_UINT64 getReplicatedObjectID() const;

        inline IZ_BOOL isDirtyReplicatedProperty() const;

        inline void dirtyReplicatedProperty();
        inline void undirtyReplicatedProperty();

        inline CStdList<ReplicatedPropertyBase>::Item* getReplicatedPropertyListTopItem();

    private:
        IZ_UINT64 m_ReplicatedObjectID;
        IZ_BOOL m_isServer;

        IZ_BOOL m_isReplicatedPropertyDirty;

        CStdList<ReplicatedPropertyBase> m_ReplicatedPropertyList;
        ReplicatedPropertyManager::HashItem m_ReplicatedObjectHashItem;
    };

    /**
     */
    template <class BASE = NullClass>
    class ReplicatedObject : public ReplicatedObjectBase, BASE {
    protected:
        ReplicatedObject() {}
        virtual ~ReplicatedObject() {}
    };

}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
