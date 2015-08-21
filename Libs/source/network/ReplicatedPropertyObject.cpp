#include "network/ReplicatedPropertyObject.h"
#include "network/ReplicatedProperty.h"

namespace izanagi {
namespace net {

    CClass CClass::Invalid;

    IZ_BOOL CClass::isValid(const CClass& clazz)
    {
        return (clazz != Invalid);
    }

    CClass::CClass(const IZ_CHAR* name)
        : m_key{ name }, m_name{ const_cast<IZ_CHAR*>(name) }
    {
    }

    CClass::CClass(const CClass& rhs)
    {
        m_key = rhs.m_key;
        m_name = rhs.m_name;
    }

    const CClass& CClass::operator = (const CClass& rhs)
    {
        m_key = rhs.m_key;
        m_name = rhs.m_name;
        return *this;
    }

    IZ_BOOL CClass::operator == (const CClass& rhs) const
    {
        return m_key == rhs.m_key;
    }

    IZ_BOOL CClass::operator!=(const CClass& rhs) const
    {
        return !(*this == rhs);
    }

    CClass::operator const CKey&() const
    {
        return m_key;
    }

    ////////////////////////////////////////////////////

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
