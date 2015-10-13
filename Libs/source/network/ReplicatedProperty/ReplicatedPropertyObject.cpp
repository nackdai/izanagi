#include "network/ReplicatedProperty/ReplicatedPropertyObject.h"
#include "network/ReplicatedProperty/ReplicatedProperty.h"

namespace izanagi {
namespace net {
    ReplicatedObjectBase::ReplicatedObjectBase()
    {
        m_ReplicatedObjectID = ReplicatedPropertyManager::genID();

        m_isServer = ReplicatedPropertyManager::get()->isServer();

        m_hasDirtyReplicatedProperty = IZ_FALSE;

        m_listItem.Init(this);
        m_listItemEx.Init(this);
    }

    ReplicatedObjectBase::~ReplicatedObjectBase()
    {
        ReplicatedPropertyManager::get()->remove(*this);
    }

    void ReplicatedObjectBase::addReplicatedProperty(ReplicatedPropertyBase& prop)
    {
        IZ_UINT idx = m_ReplicatedPropertyList.GetItemNum();
        prop.setID(idx);

        m_ReplicatedPropertyList.AddTail(prop.getListItem());
    }

    // Get id.
    IZ_UINT64 ReplicatedObjectBase::getReplicatedObjectID() const
    {
        return m_ReplicatedObjectID;
    }

    // Set id.
    void ReplicatedObjectBase::setReplicatedObjectID(IZ_UINT64 id)
    {
        m_ReplicatedObjectID = id;
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

    CStdList<ReplicatedObjectBase>::Item* ReplicatedObjectBase::getListItemEx()
    {
        return &m_listItemEx;
    }
}    // namespace net
}    // namespace izanagi
