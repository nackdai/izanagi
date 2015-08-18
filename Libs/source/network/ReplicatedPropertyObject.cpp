#include "network/ReplicatedPropertyObject.h"
#include "network/ReplicatedProperty.h"

namespace izanagi {
namespace net {
    ReplicatedObjectBase::ReplicatedObjectBase()
    {
        m_ReplicatedObjectBaseListItem.Init(this);
    }

    void ReplicatedObjectBase::addReplicatedProperty(ReplicatedPropertyBase& prop)
    {
        m_ReplicatedPropertyList.AddTail(prop.getListItem());
    }

    CStdList<ReplicatedObjectBase>::Item* ReplicatedObjectBase::getReplicatedObjectBaseListItem()
    {
        return &m_ReplicatedObjectBaseListItem;
    }
}    // namespace net
}    // namespace izanagi
