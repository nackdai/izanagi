#include "network/ReplicatedProperty/ReplicatedPropertyObject.h"
#include "network/ReplicatedProperty/ReplicatedProperty.h"

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

    IZ_BOOL CClass::is(const CClass& clazz)
    {
        return (*this == clazz);
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

        m_hasReplicatedPropertyDirty = IZ_FALSE;

        m_ReplicatedObjectHashItem.Init(m_ReplicatedObjectID, this);

        ReplicatedPropertyManager::get()->add(*this);
    }

    ReplicatedObjectBase::~ReplicatedObjectBase()
    {
        ReplicatedPropertyManager::get()->remove(*this);
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

    IZ_BOOL ReplicatedObjectBase::hasDirtyReplicatedProperty() const
    {
        return m_hasReplicatedPropertyDirty;
    }

    void ReplicatedObjectBase::dirtyReplicatedProperty()
    {
        m_hasReplicatedPropertyDirty = IZ_TRUE;;
    }

    void ReplicatedObjectBase::undirtyReplicatedProperty()
    {
        m_hasReplicatedPropertyDirty = IZ_FALSE;
    }

    CStdList<ReplicatedPropertyBase>::Item* ReplicatedObjectBase::getReplicatedPropertyListTopItem()
    {
        auto item = m_ReplicatedPropertyList.GetTop();
        return item;
    }
}    // namespace net
}    // namespace izanagi
