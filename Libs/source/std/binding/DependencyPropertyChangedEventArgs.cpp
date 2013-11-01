#include "std/dependency_property/DependencyPropertyChangedEventArgs.h"

namespace izanagi {
    DependencyPropertyChangedEventArgs::DependencyPropertyChangedEventArgs(
        const CValue& oldVal,
        const CValue& newVal,
        const DependencyProperty& prop)
        : m_Old(oldVal), m_New(newVal), m_Prop(prop)
    {
    }

    /**
     */
    const DependencyProperty& DependencyPropertyChangedEventArgs::GetProperty()
    {
        return m_Prop;
    }
}   // namespace izanagi
