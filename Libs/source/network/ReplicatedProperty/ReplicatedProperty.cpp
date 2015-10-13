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

    //////////////////////////////////////////////

    template <typename _T>
    void ReplicatedProperty<_T>::set(const _T& rhs)
    {
        // TODO
        // クライアント側では値の変更を許さない？
        if (!isServer()) {
            return;
        }

        setForcibly(rhs);
    }
}    // namespace net
}    // namespace izanagi
