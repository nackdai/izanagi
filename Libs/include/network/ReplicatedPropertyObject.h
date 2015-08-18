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
    protected:
        ReplicatedObjectBase();
        virtual ~ReplicatedObjectBase() {}

    private:
        void addReplicatedProperty(ReplicatedPropertyBase& prop);
        CStdList<ReplicatedObjectBase>::Item* getReplicatedObjectBaseListItem();

    protected:
        CStdList<ReplicatedPropertyBase> m_ReplicatedPropertyList;
        CStdList<ReplicatedObjectBase>::Item m_ReplicatedObjectBaseListItem;
    };

    /**
     */
    template <class BASE = NullClass>
    class ReplicatedObject : public ReplicatedObjectBase, BASE {
        template <typename _T> friend class ReplicatedProperty;

    protected:
        ReplicatedObject()
        {
            // TODO
            // m_ReplicatedObjectID
            // m_isServer
        }
        virtual ~ReplicatedObject() {}

    public:
        IZ_BOOL isServer() const
        {
            return m_isServer;
        }        

    protected:
        IZ_UINT64 m_ReplicatedObjectID;
        IZ_BOOL m_isServer;
    };

}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
