#include "network/ReplicatedProperty/ReplicatedPropertyObjectClass.h"

namespace izanagi {
namespace net {

    ReplicatedObjectClass ReplicatedObjectClass::Invalid;

    IZ_BOOL ReplicatedObjectClass::isValid(const ReplicatedObjectClass& clazz)
    {
        return (clazz != Invalid);
    }

    ReplicatedObjectClass::ReplicatedObjectClass(const IZ_CHAR* name)
        : m_key{ name }, m_name{ const_cast<IZ_CHAR*>(name) }
    {
    }

    ReplicatedObjectClass::ReplicatedObjectClass(IZ_UINT key)
        : m_key{ key }, m_name{ nullptr }
    {
    }

    ReplicatedObjectClass::ReplicatedObjectClass(const ReplicatedObjectClass& rhs)
    {
        m_key = rhs.m_key;
        m_name = rhs.m_name;
    }

    const ReplicatedObjectClass& ReplicatedObjectClass::operator = (const ReplicatedObjectClass& rhs)
    {
        m_key = rhs.m_key;
        m_name = rhs.m_name;
        return *this;
    }

    IZ_BOOL ReplicatedObjectClass::operator == (const ReplicatedObjectClass& rhs) const
    {
        return m_key == rhs.m_key;
    }

    IZ_BOOL ReplicatedObjectClass::operator!=(const ReplicatedObjectClass& rhs) const
    {
        return !(*this == rhs);
    }

    IZ_BOOL ReplicatedObjectClass::is(const ReplicatedObjectClass& clazz)
    {
        return (*this == clazz);
    }

    ReplicatedObjectClass::operator IZ_UINT() const
    {
        return m_key.GetValue();
    }
}    // namespace net
}    // namespace izanagi
