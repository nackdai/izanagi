#include "network/ReplicatedPropertyObject.h"
#include "network/ReplicatedProperty.h"

namespace izanagi {
namespace net {
    ReplicatedObjectBase::ReplicatedObjectBase()
    {
        m_ReplicatedObjectID = ReplicatedPropertyManager::genID();

        m_isServer = ReplicatedPropertyManager::get()->isServer();

        m_isReplicatedPropertyDirty = IZ_FALSE;

        m_ReplicatedObjectHashItem.Init(m_ReplicatedObjectID, this);

        ReplicatedPropertyManager::get()->add(this);
    }

    void ReplicatedObjectBase::addReplicatedProperty(ReplicatedPropertyBase& prop)
    {
        m_ReplicatedPropertyList.AddTail(prop.getListItem());
    }

    ReplicatedPropertyManager::HashItem* ReplicatedObjectBase::getReplicatedObjectHashItem()
    {
        return &m_ReplicatedObjectHashItem;
    }

    IZ_UINT64 ReplicatedObjectBase::getReplicatedObjectID() const
    {
        return m_ReplicatedObjectID;
    }

    IZ_BOOL ReplicatedObjectBase::isDirtyReplicatedProperty() const
    {
        return m_isReplicatedPropertyDirty;
    }

    void ReplicatedObjectBase::dirtyReplicatedProperty()
    {
        m_isReplicatedPropertyDirty = IZ_TRUE;;
    }

    void ReplicatedObjectBase::undirtyReplicatedProperty()
    {
        m_isReplicatedPropertyDirty = IZ_FALSE;
    }

    CStdList<ReplicatedPropertyBase>::Item* ReplicatedObjectBase::getReplicatedPropertyListTopItem()
    {
        auto item = m_ReplicatedPropertyList.GetTop();
        return item;
    }
}    // namespace net
}    // namespace izanagi
