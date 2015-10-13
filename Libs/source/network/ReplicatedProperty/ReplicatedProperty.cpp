#include "network/ReplicatedProperty/ReplicatedProperty.h"
#include "network/ReplicatedProperty/ReplicatedPropertyObject.h"

namespace izanagi {
namespace net {
    ReplicatedPropertyBase::ReplicatedPropertyBase()
    {
        m_isDirty = IZ_FALSE;
        m_listItem.Init(this);
    }

    IZ_BOOL ReplicatedPropertyBase::isDirty() const
    {
        return m_isDirty;
    }

    void ReplicatedPropertyBase::dirty()
    {
        m_isDirty = IZ_TRUE;
    }

    void ReplicatedPropertyBase::unDirty()
    {
        m_isDirty = IZ_FALSE;
    }

    CStdList<ReplicatedPropertyBase>::Item* ReplicatedPropertyBase::getListItem()
    {
        return &m_listItem;
    }

    IZ_UINT ReplicatedPropertyBase::getID() const
    {
        return m_id;
    }

    void ReplicatedPropertyBase::setID(IZ_UINT id)
    {
        m_id = id;
    }
}    // namespace net
}    // namespace izanagi
