#include "network/ReplicatedProperty/ReplicatedPropertyObject.h"
#include "network/ReplicatedProperty/ReplicatedProperty.h"

namespace izanagi {
namespace net {
    ReplicatedObjectBase::ReplicatedObjectBase()
    {
        m_ReplicatedObjectID = ReplicatedPropertyManager::genID();

        m_isServer = ReplicatedPropertyManager::get()->isServer();

        m_hasDirtyReplicatedProperty = IZ_FALSE;

        ReplicatedPropertyManager::get()->add(*this);

        m_listItem.Init(this);
    }

    ReplicatedObjectBase::~ReplicatedObjectBase()
    {
        ReplicatedPropertyManager::get()->remove(*this);
    }

    void ReplicatedObjectBase::addReplicatedProperty(ReplicatedPropertyBase& prop)
    {
        m_ReplicatedPropertyList.AddTail(prop.getListItem());
    }

    IZ_UINT64 ReplicatedObjectBase::getReplicatedObjectID() const
    {
        return m_ReplicatedObjectID;
    }

    IZ_BOOL ReplicatedObjectBase::hasDirtyReplicatedProperty() const
    {
        return m_hasDirtyReplicatedProperty;
    }

    void ReplicatedObjectBase::dirtyReplicatedProperty()
    {
        m_hasDirtyReplicatedProperty = IZ_TRUE;;
    }

    void ReplicatedObjectBase::undirtyReplicatedProperty()
    {
        m_hasDirtyReplicatedProperty = IZ_FALSE;
    }

    CStdList<ReplicatedPropertyBase>::Item* ReplicatedObjectBase::getReplicatedPropertyListTopItem()
    {
        auto item = m_ReplicatedPropertyList.GetTop();
        return item;
    }

    CStdList<ReplicatedObjectBase>::Item* ReplicatedObjectBase::getListItem()
    {
        return &m_listItem;
    }
}    // namespace net
}    // namespace izanagi
